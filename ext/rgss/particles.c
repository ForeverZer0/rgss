#include "game.h"
#include "graphics.h"

VALUE rb_cEmitter;

GLuint RGSS_PARTICLE_SHADER;

#define MAX_PARTICLES (USHRT_MAX)

// #define PARTICLE_VERTEX_COUNT  (4)
// #define PARTICLE_VERTEX_SIZE   (SIZEOF_FLOAT * 4)
// #define PARTICLE_VERTICES_SIZE (PARTICLE_VERTEX_SIZE * PARTICLE_VERTEX_COUNT)
// #define PARTICLE_COLOR_SIZE
// #define PARTICLE_COLORS_SIZE   (PARTICLE_COLOR_SIZE * PARTICLE_VERTEX_COUNT)

#define PARTICLE_QUAD_SIZE  (SIZEOF_FLOAT * 4)
#define PARTICLE_COLOR_SIZE (sizeof(GLubyte) * 4)

typedef struct
{
    float min;
    float max;
} RGSS_Range;

typedef struct
{
    vec3 position;
    vec3 scale;
    vec3 velocity;
    GLubyte color[4]; 
    int life; /** Number of ticks left to live. */

    float zoom;
    float angular_velocity;
    float angle;
    float weight;
    float fade;
} RGSS_Particle;

typedef struct
{
    RGSS_Renderable base;
    VALUE viewport;
    VALUE spectrum;
    struct
    { // TODO: Do this for sprite, plane, etc
        VALUE value;
        GLuint id;
    } texture;



    GLuint count;      /** The number of particles that are "alive" and in use. */
    GLushort capacity; /** The maximum number of particles the emitter uses. */
    GLuint last_used;  /** The index of the last used particle, used for search optimizations. */
    RGSS_Range lifespan; /** A range determining the number of ticks particles will exist for. */
    RGSS_Particle *particles; /** An array of particles structures. */
    GLfloat *quads; /** A CPU buffer containing the offset/size for particles. */
    GLubyte *colors; /** A CPU buffer containing particle colors. */
    GLuint quad_vbo; /** The VBO containing the offsets and sizes of the particles. */
    GLuint color_vbo; /** The VBO containing the colors of the particles. */

    struct
    {
        GLuint id;
        int color;
        int flash;
        int tone;
        int hue;
        int opacity;
        int textured;
    } shader;

    RGSS_Range fade;
    RGSS_Range angular_velocity;
    RGSS_Range zoom;
    RGSS_Range force; /** A range determing the amount of force particles will initially have. */
    RGSS_Range rate; /** A range determining the number of particles to spawn each game tick. */
    vec2 friction; 
    float radius; /** The radius from the emitter's postion that particles will spawn. */
    float gravity; /** The amount of force applied on the y-axis. */
    float wind; /** The amount of force applied on the x-axis. */
} RGSS_Emitter;

static inline void RGSS_Range_Parse(VALUE value, RGSS_Range *range)
{
    if (range == NULL)
        return;

    if (NIL_P(value))
    {
        *range = (RGSS_Range){0.0f, 0.0f};
        return;
    }

    if (rb_obj_is_kind_of(value, rb_cRange))
    {
        VALUE begin, end;
        int inclusive;
        rb_range_values(value, &begin, &end, &inclusive);
        *range = (RGSS_Range){NUM2FLT(begin), NUM2FLT(end)};
        if (!inclusive)
            range->max--;
    }
    else if (rb_obj_is_kind_of(value, rb_cVec2) == Qtrue)
    {
        memcpy(range, DATA_PTR(value), sizeof(RGSS_Range));
    }
    else if (rb_obj_is_kind_of(value, rb_cIVec2) == Qtrue)
    {
        int *ivec = DATA_PTR(value);
        *range = (RGSS_Range){ivec[0], ivec[1]};
    }
    else
    {
        float v = NUM2FLT(value);
        *range = (RGSS_Range){v, v};
    }
}

static inline float RGSS_Range_Sample(RGSS_Range *range)
{
    if (isfinite(range->min) && isfinite(range->max))
    {
        float deviation = (range->max - range->min) * 0.5f;
        return range->min + deviation + RGSS_Rand() * deviation - RGSS_Rand() * deviation;
    }
    return range->max;
}

static int RGSS_SortParticle(const void *p1, const void *p2)
{
    RGSS_Particle *a = (RGSS_Particle *)p1, *b = (RGSS_Particle *)p2;
    return (a->position[2] > b->position[2]);
}

static VALUE RGSS_Emitter_GetTexture(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    return e->texture.value;
}

static VALUE RGSS_Emitter_SetTexture(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    e->texture.value = value;

    if (RTEST(value))
    {
        RGSS_Texture *tex = DATA_PTR(value);
        e->texture.id = tex->id;
        e->base.entity.size[0] = (float)tex->width;
        e->base.entity.size[1] = (float)tex->height;
    }
    else
    {
        e->texture.id = GL_NONE;
        glm_vec3_zero(e->base.entity.size);
    }
    return value;
}

static void RGSS_Emitter_Free(void *data)
{
    if (data == NULL)
        return;
    RGSS_Emitter *e = data;
    if (e->particles)
        xfree(e->particles);
    if (e->quads)
        xfree(e->quads);
    if (e->colors)
        xfree(e->colors);
    xfree(data);
}

static VALUE RGSS_Emitter_SetWidth(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    if (e->texture.id != GL_NONE)
    {
        rb_warn("cannot alter size of textured particle emitter");
        return value;
    }
    return rb_call_super(1, &value);
}

static VALUE RGSS_Emitter_SetHeight(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    if (e->texture.id != GL_NONE)
    {
        rb_warn("cannot alter size of textured particle emitter");
        return value;
    }
    return rb_call_super(1, &value);
}

static VALUE RGSS_Emitter_SetSize(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    if (e->texture.id != GL_NONE)
    {
        rb_warn("cannot alter size of textured particle emitter");
        return value;
    }
    return rb_call_super(1, &value);
}

static void RGSS_Emitter_Mark(void *data)
{
    RGSS_Emitter *e = data;
    rb_gc_mark(e->texture.value);
    rb_gc_mark(e->viewport);
    rb_gc_mark(e->spectrum);
}

static VALUE RGSS_Emitter_Alloc(VALUE klass)
{
    RGSS_Emitter *e = ALLOC(RGSS_Emitter);
    memset(e, 0, sizeof(RGSS_Emitter));
    RGSS_Entity_Init(&e->base.entity);
    e->viewport = Qnil;
    e->texture.value = Qnil;
    e->spectrum = Qnil;
    return Data_Wrap_Struct(klass, RGSS_Emitter_Mark, RGSS_Emitter_Free, e);
}

static VALUE RGSS_Emitter_Dispose(VALUE self)
{
    rb_call_super(0, NULL);
    RGSS_Emitter *e = DATA_PTR(self);
    if (e->quad_vbo)
    {
        glDeleteBuffers(1, &e->quad_vbo);
        e->quad_vbo = GL_NONE;
    }
    if (e->color_vbo)
    {
        glDeleteBuffers(1, &e->color_vbo);
        e->color_vbo = GL_NONE;
    }

    if (e->particles)
    {
        xfree(e->particles);
        e->particles = NULL;
    }
    if (e->quads)
    {
        xfree(e->quads);
        e->quads = NULL;
    }
    if (e->colors)
    {
        xfree(e->colors);
        e->colors = NULL;
    }
    return Qnil;
}

static void RGSS_Emitter_CreateStorage(RGSS_Emitter *e, int max)
{
    size_t size = sizeof(RGSS_Particle) * max;

    // Storage for particle structures
    e->particles = xmalloc(size);
    if (e->particles == NULL)
        rb_raise(rb_eNoMemError, "out of memory");
    memset(e->particles, 0, size);
    for (int i = 0; i < max; i++)
        e->particles[i].life = -1.0f;

    // Storage for vertices
    e->quads = xmalloc(PARTICLE_QUAD_SIZE * max);
    if (e->quads == NULL)
        rb_raise(rb_eNoMemError, "out of memory");
    memset(e->quads, 0, PARTICLE_QUAD_SIZE * max);

    // Storage for colors
    e->colors = xmalloc(PARTICLE_COLOR_SIZE * max);
    if (e->colors == NULL)
        rb_raise(rb_eNoMemError, "out of memory");
    memset(e->colors, 1, PARTICLE_COLOR_SIZE * max);
}

static void RGSS_Emitter_VertexSetup(RGSS_Emitter *e, int max)
{
    // Create the vertex array
    glGenVertexArrays(1, &e->base.vao);
    glBindVertexArray(e->base.vao);

    // Initialize VBO for static vertex data for billboard
    glGenBuffers(1, &e->base.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, e->base.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(RGSS_QUAD_VERTICES), RGSS_QUAD_VERTICES, GL_STATIC_DRAW);

    // Create EBO
    glGenBuffers(1, &e->base.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, e->base.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RGSS_QUAD_INDICES), RGSS_QUAD_INDICES, GL_STATIC_DRAW);

    // Initialize empty VBO to contain particle quads.
    glGenBuffers(1, &e->quad_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, e->quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, PARTICLE_QUAD_SIZE * e->capacity, NULL, GL_STREAM_DRAW);

    // Confirm the VBO was created successfully
    GLint bufsize;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufsize);
    if (bufsize != PARTICLE_QUAD_SIZE * e->capacity)
        rb_raise(rb_eRGSSError, "GPU memory allocation failed");

    // Initialize empty VBO to contain particle colors.
    glGenBuffers(1, &e->color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, e->color_vbo);
    glBufferData(GL_ARRAY_BUFFER, PARTICLE_COLOR_SIZE * e->capacity, NULL, GL_STREAM_DRAW);

    // Confirm the VBO was created successfully
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufsize);
    if (bufsize != PARTICLE_COLOR_SIZE * e->capacity)
        rb_raise(rb_eRGSSError, "GPU memory allocation failed");

    // Configure static vertex data layout
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, e->base.vbo);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    // Configure the streaming VBO used for particle quads/sizes
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, e->quad_vbo);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    // Configure the streaming VBO used for particle colors
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, e->color_vbo);
    glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, NULL);

    // Configure all the data is advanced in vertex shader
    glVertexAttribDivisor(0, 0); // Always reuse
    glVertexAttribDivisor(1, 1); // One per quad
    glVertexAttribDivisor(2, 1); // One per color

    // Unbind the VAO
    glBindVertexArray(GL_NONE);
}

static VALUE RGSS_Emitter_Render(VALUE self, VALUE alpha)
{
    RGSS_Emitter *e = DATA_PTR(self);
    if (!e->base.visible || e->base.opacity < FLT_EPSILON)
        return Qnil;

    // Configure blending state
    glBlendEquation(e->base.blend.op);
    glBlendFunc(e->base.blend.src, e->base.blend.dst);

    // Setup shader
    glUseProgram(e->shader.id);
    glUniform4fv(e->shader.color, 1, e->base.color);
    glUniform4fv(e->shader.tone, 1, e->base.tone);
    glUniform4fv(e->shader.flash, 1, e->base.flash_color);
    glUniform1f(e->shader.hue, e->base.hue);
    glUniform1f(e->shader.opacity, e->base.opacity);

    if (e->texture.id)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, e->texture.id);
        glUniform1i(e->shader.textured, GL_TRUE);
    }
    else
    {
        glUniform1i(e->shader.textured, GL_FALSE);
    }

    // Render the particles
    glBindVertexArray(e->base.vao);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL, e->count);
    glBindVertexArray(GL_NONE);

    return Qnil;
}

static inline GLuint RGSS_Emitter_GetIndex(RGSS_Emitter *e)
{
    // Start search from index of previous location
    for (GLuint i = e->last_used; i < e->capacity; i++)
    {
        if (e->particles[i].life < 0)
        {
            e->last_used = i;
            return i;
        }
    }

    // If none were found, then search from beginning
    for (GLuint i = 0; i < e->last_used; i++)
    {
        if (e->particles[i].life < 0)
        {
            e->last_used = i;
            return i;
        }
    }
    // No available particles, override the first one
    // RGSS_LogDebug("Maximum particle limit reached, consider increasing maximum particles or decreasing emission
    // rate");
    return 0;
}

static inline void RGSS_Emitter_SetParticleColor(RGSS_Emitter *e, RGSS_Particle *p)
{
    VALUE c;
    if (NIL_P(e->spectrum))
    {
        int r = rand();
        memcpy(p->color, &r, 4);
        p->color[3] = 255;
        return;
    }

    if (rb_obj_is_kind_of(e->spectrum, rb_cColor))
    {
        c = e->spectrum;
        goto RESULT;
    }

    if (RB_TYPE_P(e->spectrum, T_ARRAY))
    {
        long len = rb_array_len(e->spectrum);
        if (len == 0)
        {
            memset(p->color, 255, 4);
            return;
        }
        c = rb_ary_entry(e->spectrum, rand() % len);
        goto RESULT;
    }

    if (rb_obj_is_kind_of(e->spectrum, rb_cRange))
    {
        VALUE a, b;
        int inclusive;
        rb_range_values(e->spectrum, &a, &b, &inclusive);
        
        vec4 vec;
        glm_vec4_lerp(DATA_PTR(a), DATA_PTR(b), RGSS_Rand(), vec);
        RGSS_PackColor(vec, (unsigned int*) &p->color);
        return;
    }

    rb_raise(rb_eTypeError, "%s is not a Color, Array, Range, or NilClass", CLASS_NAME(e->spectrum));

    RESULT:
    RGSS_PackColor(DATA_PTR(c), (unsigned int*) &p->color);
}

static inline void RGSS_Emitter_RenewParticle(RGSS_Emitter *e, RGSS_Particle *p)
{
    
    // Select initial starting point of particle, using uniform (radial) distribution.
    // https://programming.guide/random-point-within-circle.html
    float a = RGSS_Rand() * 2.0f * GLM_PI;
    float r = e->radius * sqrtf(RGSS_Rand());
    p->position[0] = e->base.entity.position[0] + (r * cosf(a));
    p->position[1] = e->base.entity.position[1] + (r * sinf(a));

    // Configure initial speed and direction
    float direction = RGSS_Rand() * 360.0f;
    float speed = RGSS_Range_Sample(&e->force);
    p->velocity[0] = RGSS_FastSin(direction) * speed;
    p->velocity[1] = RGSS_FastCos(direction) * speed;


    p->angle = RGSS_Rand() * 360.0f;

    glm_vec3_copy(e->base.entity.scale, p->scale); // TODO: Multiply by "growth" variable

    p->angular_velocity = RGSS_Range_Sample(&e->angular_velocity);
    p->fade = RGSS_Range_Sample(&e->fade);
    // p->friction = RGSS_Range_Sample(&e->friction);
    p->zoom = RGSS_Range_Sample(&e->zoom);

    p->life = (int)roundf(RGSS_Range_Sample(&e->lifespan));
    RGSS_Emitter_SetParticleColor(e, p);
}

static VALUE RGSS_Emitter_GetSpectrum(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    return e->spectrum;
}

static VALUE RGSS_Emitter_SetSpectrum(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    e->spectrum = value;
    return value;
}

static VALUE RGSS_Emitter_Update(VALUE self, VALUE delta)
{
    // rb_call_super(1, &delta);

    RGSS_Emitter *e = DATA_PTR(self);
    float d = NUM2FLT(delta);

    int gen_count = (int) RGSS_Range_Sample(&e->rate);
    for (int i = 0; i < gen_count; i++)
    {
        GLuint index = RGSS_Emitter_GetIndex(e);
        RGSS_Emitter_RenewParticle(e, &e->particles[index]);
    }

    RGSS_Particle *p;
    int count = 0;
    for (int i = 0; i < e->capacity; i++)
    {
        p = &e->particles[i]; // shortcut
        if (--p->life > 0)
        {
            // Resize.
            glm_vec2_adds(p->scale, p->zoom * d, p->scale);

            // Fade out.
            p->color[3] -= p->fade * d;

            // Early out of this iteration has 0 opacity or scales to nothing
            if (p->color[3] == 0 || p->scale[0] < FLT_EPSILON || p->scale[1] < FLT_EPSILON)
            {
                p->life = 0;
                continue;
            }

            // Rotate.
            p->angle += p->angular_velocity * d;
            // glm_vec3_rotate(p->position, glm_rad(p->angle), RGSS_AXIS_Z);
        

            p->velocity[0] *= 1.0 - e->friction[0] * d;
            p->velocity[1] *= 1.0 - e->friction[1] * d;
            p->velocity[0] += e->wind * d;
            p->velocity[1] += e->gravity * d; // TOOD: Combine with above

            // Translate
            glm_vec3_muladds(p->velocity, d,  p->position);

            // Fill the GPU buffer
            e->quads[4 * count + 0] = p->position[0];
            e->quads[4 * count + 1] = p->position[1];
            e->quads[4 * count + 2] = e->base.entity.size[0] * p->scale[0];
            e->quads[4 * count + 3] = e->base.entity.size[1] * p->scale[1];

            e->colors[4 * count + 0] = p->color[0];
            e->colors[4 * count + 1] = p->color[1];
            e->colors[4 * count + 2] = p->color[2];
            e->colors[4 * count + 3] = p->color[3];
            count++;
        }
    }

    e->count = count;
    qsort(e->particles, e->capacity, sizeof(RGSS_Particle), RGSS_SortParticle);

    glBindBuffer(GL_ARRAY_BUFFER, e->quad_vbo);
    glBufferData(
        GL_ARRAY_BUFFER, e->capacity * PARTICLE_QUAD_SIZE, NULL,
        GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * PARTICLE_QUAD_SIZE, e->quads);

    glBindBuffer(GL_ARRAY_BUFFER, e->color_vbo);
    glBufferData(
        GL_ARRAY_BUFFER, e->capacity * PARTICLE_COLOR_SIZE, NULL,
        GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * PARTICLE_COLOR_SIZE, e->colors);

    return Qnil;
}

ATTR_ACCESSOR(RGSS_Emitter, Gravity, gravity, DBL2NUM, NUM2FLT)
ATTR_ACCESSOR(RGSS_Emitter, Wind, wind, DBL2NUM, NUM2FLT)
ATTR_ACCESSOR(RGSS_Emitter, Radius, radius, DBL2NUM, NUM2FLT)

static VALUE RGSS_Emitter_GetLifespan(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    return rb_range_new(DBL2NUM(e->lifespan.min), DBL2NUM(e->lifespan.max), true);
}

static VALUE RGSS_Emitter_SetLifespan(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    RGSS_Range_Parse(value, &e->lifespan);
    return value;
}

static VALUE RGSS_Emitter_GetForce(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    return rb_range_new(DBL2NUM(e->force.min), DBL2NUM(e->force.max), true);
}

static VALUE RGSS_Emitter_SetForce(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    RGSS_Range_Parse(value, &e->force);
    return value;
}

static VALUE RGSS_Emitter_GetRate(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    return rb_range_new(DBL2NUM(e->rate.min), DBL2NUM(e->rate.max), true);
}

static VALUE RGSS_Emitter_SetRate(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    RGSS_Range_Parse(value, &e->rate);
    return value;
}

static VALUE RGSS_Emitter_GetZoom(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    return rb_range_new(DBL2NUM(e->zoom.min), DBL2NUM(e->zoom.max), true);
}

static VALUE RGSS_Emitter_SetZoom(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    RGSS_Range_Parse(value, &e->zoom);
    return value;
}

static VALUE RGSS_Emitter_GetFade(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    return rb_range_new(DBL2NUM(e->fade.min), DBL2NUM(e->fade.max), true);
}

static VALUE RGSS_Emitter_SetFade(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    RGSS_Range_Parse(value, &e->fade);
    return value;
}

static VALUE RGSS_Emitter_GetAngularVelocity(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    return rb_range_new(DBL2NUM(e->angular_velocity.min), DBL2NUM(e->angular_velocity.max), true);
}

static VALUE RGSS_Emitter_SetAngularVelocity(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    RGSS_Range_Parse(value, &e->angular_velocity);
    return value;
}

static VALUE RGSS_Emitter_GetFriction(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    return RGSS_Vec2_New(e->friction[0], e->friction[1]);
}

static VALUE RGSS_Emitter_SetFriction(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    if (rb_obj_is_kind_of(value, rb_cVec2))
        memcpy(e->friction, DATA_PTR(value), RGSS_VEC2_SIZE);
    else
        glm_vec2_fill(e->friction, NUM2FLT(value));
    return value;
}

static VALUE RGSS_Emitter_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE max, viewport, texture, opts;
    rb_scan_args(argc, argv, "12:", &max, &viewport, &texture, &opts);

    RGSS_Emitter *e = DATA_PTR(self);
    RGSS_Renderable_Init(&e->base);
    rb_call_super(1, &viewport);

    if (rb_obj_is_kind_of(viewport, rb_cViewport) == Qtrue)
        e->viewport = viewport;

    e->radius = 64.0f;
    e->lifespan = (RGSS_Range){30, 60}; // TODO
    e->gravity = -9.81f;
    e->rate = (RGSS_Range){100.0f, 200.0f};
    e->zoom = (RGSS_Range) { 1.0f, 1.0f };

    // Initialize memory for all the particles
    e->capacity = NUM2USHORT(max);
    if (e->capacity > MAX_PARTICLES)
        rb_raise(rb_eArgError, "max particles out of range (given %u, expected 0...%u)", e->capacity, MAX_PARTICLES);

    // Initialize buffers and setup vertex array
    RGSS_Emitter_CreateStorage(e, e->capacity);
    RGSS_Emitter_VertexSetup(e, e->capacity);

    // TODO:
    const char *v = "/home/eric/open_rpg/lib/rgss/shaders/particles-vert.glsl";
    const char *f = "/home/eric/open_rpg/lib/rgss/shaders/particles-frag.glsl";
    GLuint id = RGSS_CreateProgramFromFile(v, f, NULL);

    e->shader.id = id;
    e->shader.color = glGetUniformLocation(id, "color");
    e->shader.tone = glGetUniformLocation(id, "tone");
    e->shader.flash = glGetUniformLocation(id, "flash");
    e->shader.hue = glGetUniformLocation(id, "hue");
    e->shader.opacity = glGetUniformLocation(id, "opacity");
    e->shader.textured = glGetUniformLocation(id, "textured");

    if (rb_block_given_p())
    {
        // Makes setting to a variable a little more elegant.
        rb_yield(self);
    }

    return self;
}

void RGSS_Init_Particles(VALUE parent)
{
    rb_cEmitter = rb_define_class_under(rb_mRGSS, "Emitter", rb_cRenderable);
    rb_define_alloc_func(rb_cEmitter, RGSS_Emitter_Alloc);

    rb_define_methodm1(rb_cEmitter, "initialize", RGSS_Emitter_Initialize, -1);
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Texture, "texture");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Gravity, "gravity");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Wind, "wind");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Lifespan, "lifespan");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Spectrum, "spectrum");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Friction, "friction");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Radius, "radius");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Rate, "rate");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Force, "force");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, AngularVelocity, "angular_velocity");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Fade, "fade");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Zoom, "zoom");

    rb_define_method0(rb_cEmitter, "dispose", RGSS_Emitter_Dispose, 0);
    rb_define_method1(rb_cEmitter, "update", RGSS_Emitter_Update, 1);
    rb_define_method1(rb_cEmitter, "render", RGSS_Emitter_Render, 1);

    rb_define_method1(rb_cEmitter, "width=", RGSS_Emitter_SetWidth, 1);
    rb_define_method1(rb_cEmitter, "height=", RGSS_Emitter_SetHeight, 1);
    rb_define_method1(rb_cEmitter, "size=", RGSS_Emitter_SetSize, 1);

    rb_define_const(rb_cEmitter, "EARTH_GRAVITY", DBL2NUM(9.81));
}