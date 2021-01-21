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


#define PARTICLE_QUAD_SIZE (SIZEOF_FLOAT * 4)
#define PARTICLE_COLOR_SIZE (sizeof(GLubyte) * 4)

typedef struct
{
    vec3 position;
    vec3 scale;
    vec3 velocity;
    GLubyte color[4];
    float angle;
    float weight;
    float life;
} RGSS_Particle;

static inline void RGSS_Emitter_ParseRange(VALUE value, vec2 range)
{
    if (NIL_P(value))
    {
        glm_vec2_zero(range);
        return;
    }

    if (rb_obj_is_kind_of(value, rb_cRange))
    {
        VALUE begin, end;
        int inclusive;
        rb_range_values(range, &begin, &end, &inclusive);

        range[0] = NUM2FLT(begin);
        range[1] = NUM2FLT(end);
        if (!inclusive)
            range[1]--;

        return;
    }

    float v = NUM2FLT(value);
    glm_vec2_fill(range, v);
}

static int RGSS_SortParticle(const void *p1, const void *p2)
{
    RGSS_Particle *a = (RGSS_Particle *)p1, *b = (RGSS_Particle *)p2;
    return (a->position[2] > b->position[2]);
}

typedef struct
{
    RGSS_Renderable base;
    VALUE viewport;
    struct
    {
        VALUE value;
        GLuint id;
    } texture;

    GLuint quad_vbo;
    GLuint color_vbo;

    GLuint count; /** The number of particles that are "alive" and in use. */
    GLushort capacity; /** The maximum number of particles the emitter uses. */
    GLuint last_used; /** The index of the last used particle, used for search optimizations. */
    
    GLfloat life;
    RGSS_Particle *particles;
    GLfloat *quads;
    GLubyte *colors;

    size_t quads_offset;
    size_t colors_offset;

    struct {
        GLuint id;
    } shader;

    vec3 gravity;
} RGSS_Emitter;

// static inline float RGSS_Deviate(vec2 range)
// {
//     if (isfinite(range[0]) && isfinite(range[1]))


//     {
//         float deviation = (range[1] - range[1]) / 2.0;
//         return range[0] + deviation + randf() * deviation - randf() * deviation;
//     }
//     return range[1];
// }

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

static void RGSS_Emitter_Mark(void *data)
{
    RGSS_Emitter *e = data;
    rb_gc_mark(e->texture.value);
    rb_gc_mark(e->viewport);
}

static VALUE RGSS_Emitter_Alloc(VALUE klass)
{
    RGSS_Emitter *e = ALLOC(RGSS_Emitter);
    memset(e, 0, sizeof(RGSS_Emitter));
    RGSS_Entity_Init(&e->base.entity);
    e->viewport = Qnil;
    e->texture.value = Qnil;
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

    if (e->texture.id)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, e->texture.id);
    }

    glUseProgram(e->shader.id);
    glBindVertexArray(e->base.vao);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL, e->count);
    glBindVertexArray(GL_NONE);

    return Qnil;
}

static inline GLuint RGSS_Emitter_GetIndex(RGSS_Emitter *e)
{
    // Start search from index of last used one
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
    return 0;
}

static inline void RGSS_Emitter_RenewParticle(RGSS_Emitter *e, RGSS_Particle *p)
{
    p->life = 5.0f; // This particle will live 5 seconds.
    // e->particles[index].position[2] = -20.0f;

    // float spread = 1.5f;
    // vec3 maindir, randomdir;
    // maindir[1] = 10.0f;

    // randomdir[0] = RGSS_Rand() + 1.0f;
    // randomdir[1] = RGSS_Rand() + 1.0f;

    glm_vec3_copy(e->base.entity.position, p->position);
    glm_vec3_copy(e->base.entity.velocity, p->velocity); // Calculate from range
    glm_vec3_copy(e->base.entity.scale, p->scale);

    p->velocity[0] *= (RGSS_Rand() * 60.0f) - 30.0f;
    p->velocity[1] *= (RGSS_Rand() * 60.0f) - 30.0f;

    // glm_vec3_addadd(maindir, randomdir, e->particles[index].velocity);

    // Very bad way to generate a random color
    p->color[0] = rand() % 256;
    p->color[1] = rand() % 256;
    p->color[2] = rand() % 256;
    p->color[3] = (rand() % 256) / 3;
}

static VALUE RGSS_Emitter_Update(VALUE self, VALUE delta)
{
    // rb_call_super(1, &delta);

    RGSS_Emitter *e = DATA_PTR(self);
    float d = NUM2FLT(delta);

	int new = (int)(d*10000.0);
    if (new > (int)(0.016f*10000.0))
        new = (int)(0.016f*10000.0);

    for (int i = 0; i < new; i++)
    {
        GLuint index = RGSS_Emitter_GetIndex(e);
        RGSS_Emitter_RenewParticle(e, &e->particles[index]);
    }

    RGSS_Particle *p;
    int count = 0;
    for (int i = 0; i < e->capacity; i++)
    {
        p = &e->particles[i]; // shortcut
        if (p->life > 0.0f)
        {
            // Decrease life
            p->life -= d;
            if (p->life > 0.0f)
            {
                // Add velocity to particle
                glm_vec3_muladds(p->velocity, d, p->position);
   
                // Fill the GPU buffer
                e->quads[4 * count + 0] = p->position[0];
                e->quads[4 * count + 1] = p->position[1];
                e->quads[4 * count + 2] = e->base.entity.size[0] * p->scale[0]; 
                e->quads[4 * count + 3] = e->base.entity.size[1] * p->scale[1];

                e->colors[4 * count + 0] = p->color[0];
                e->colors[4 * count + 1] = p->color[1];
                e->colors[4 * count + 2] = p->color[2];
                e->colors[4 * count + 3] = p->color[3];
            }
            count++;
        }
    }

    e->count = count;
    qsort(e->particles, e->capacity, sizeof(RGSS_Particle), RGSS_SortParticle);


    glBindBuffer(GL_ARRAY_BUFFER, e->quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, e->capacity * PARTICLE_QUAD_SIZE, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * PARTICLE_QUAD_SIZE, e->quads);

    glBindBuffer(GL_ARRAY_BUFFER, e->color_vbo);
    glBufferData(GL_ARRAY_BUFFER, e->capacity* PARTICLE_COLOR_SIZE, NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * PARTICLE_COLOR_SIZE, e->colors);

    return Qnil;
}

static VALUE RGSS_Emitter_GetGravity(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    return RGSS_Vec3_New(e->gravity[0], e->gravity[1], e->gravity[3]);
}

static VALUE RGSS_Emitter_SetGravity(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    RGSS_ValueToVec3(value, e->gravity);
    return value;
}

static VALUE RGSS_Emitter_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE max, viewport, texture, opts;
    rb_scan_args(argc, argv, "12:", &max, &viewport, &texture, &opts);
    rb_call_super(1, &viewport);

    RGSS_Emitter *e = DATA_PTR(self);
    if (rb_obj_is_kind_of(viewport, rb_cViewport) == Qtrue)
        e->viewport = viewport;

    e->life = 4.0f;
    e->gravity[1] = -9.81f;

    // Initialize memory for all the particles
    e->capacity = NUM2USHORT(max);
    if (e->capacity > MAX_PARTICLES)
        rb_raise(rb_eArgError, "max particles out of range (given %u, expected 0...%u)", e->capacity, MAX_PARTICLES);

    // Initialize buffers and setup vertex array
    RGSS_Emitter_CreateStorage(e, e->capacity);
    RGSS_Emitter_VertexSetup(e, e->capacity);

    const char *v = "/home/eric/open_rpg/lib/rgss/shaders/particles-vert.glsl";
    const char *f = "/home/eric/open_rpg/lib/rgss/shaders/particles-frag.glsl";
    e->shader.id = RGSS_CreateProgramFromFile(v, f, NULL);

    return self;
}

void RGSS_Init_Particles(VALUE parent)
{
    rb_cEmitter = rb_define_class_under(rb_mRGSS, "Emitter", rb_cRenderable);
    rb_define_alloc_func(rb_cEmitter, RGSS_Emitter_Alloc);

    rb_define_methodm1(rb_cEmitter, "initialize", RGSS_Emitter_Initialize, -1);
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Texture, "texture");

    rb_define_method0(rb_cEmitter, "dispose", RGSS_Emitter_Dispose, 0);
    rb_define_method1(rb_cEmitter, "update", RGSS_Emitter_Update, 1);
    rb_define_method1(rb_cEmitter, "render", RGSS_Emitter_Render, 1);
}