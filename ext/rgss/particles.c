#include "game.h"
#include "graphics.h"

VALUE rb_cEmitter;

GLuint RGSS_PARTICLE_SHADER;

#define PARTICLE_QUAD_SIZE  (SIZEOF_FLOAT * 4)
#define PARTICLE_COLOR_SIZE (sizeof(GLubyte) * 4)

#define EMITTER_GET_RANGE(name, field)                                                                                 \
    static VALUE RGSS_Emitter_Get##name(VALUE self)                                                                    \
    {                                                                                                                  \
        RGSS_Emitter *e = DATA_PTR(self);                                                                              \
        return rb_range_new(DBL2NUM(e->field.min), DBL2NUM(e->field.max), true);                                       \
    }

#define EMITTER_SET_RANGE(name, field)                                                                                 \
    static VALUE RGSS_Emitter_Set##name(VALUE self, VALUE value)                                                       \
    {                                                                                                                  \
        RGSS_Emitter *e = DATA_PTR(self);                                                                              \
        RGSS_Range_Parse(value, &e->field);                                                                            \
        return value;                                                                                                  \
    }

/**
 * @brief Base structure defining a range of values by a minimum and maximum.
 */
typedef struct
{
    float min; /** The minimum value of the range (inclusive). */
    float max; /** The maximum value of the range (inclusive). */
} RGSS_Range;

/**
 * @brief Container for particle data.
 */
typedef struct
{
    vec3 position;    /** The current position of the particle. */
    vec3 scale;       /** The current scale of the particle. */
    vec3 velocity;    /** The current velcoity of the particle. */
    GLubyte color[4]; /** An RGBA color for the vertex color. */
    vec2 size;        /** The size of the particle. */
    int life;         /** Number of ticks left to live. */
    float growth;     /** Scaling factor applied to the base particle size. */
    float rotation;   /** The rotation to be applied each tick. */
    float angle;      /** The current angle of rotation, in degrees. */
    float fade;       /** The amount of opacity change to apply each tick. */
    float opacity;    /** The current opacity as a float. */
    int depth;        /** The depth of particle, used for determining render order; */
} RGSS_Particle;

/**
 * @brief Structure holding all the configuration on how particles will be emitted.
 */
typedef struct
{
    RGSS_Renderable base; /** The base Renderable structure. */
    VALUE viewport;       /** The Viewport object associated with this instance. */
    VALUE spectrum;       /** A Ruby value for possible colors for particles. */
    struct
    {
        VALUE value; /** The Ruby value of the texture. */
        GLuint id;   /** The OpenGL name of the texture. */
        vec2 size;
    } texture;
    GLuint count;             /** The number of particles that are "alive" and in use. */
    GLushort capacity;        /** The maximum number of particles the emitter uses. */
    GLuint last_used;         /** The index of the last used particle, used for search optimizations. */
    RGSS_Particle *particles; /** An array of particles structures. */
    GLfloat *quads;           /** A CPU buffer containing the offset/size for particles. */
    GLubyte *colors;          /** A CPU buffer containing particle colors. */
    float *angles;            /** A CPU buffer containing the particle angles. */
    GLuint quad_vbo;          /** The VBO containing the offsets and sizes of the particles. */
    GLuint color_vbo;         /** The VBO containing the colors of the particles. */
    GLuint angle_vbo;         /** The VBO containing the angles of the particles. */
    RGSS_Range lifespan;      /** A range determining the number of ticks particles will exist for. */
    RGSS_Range direction;     /** A range determining the initial direction (in degrees) of particles. */
    RGSS_Range size;          /** A range indicating the initial size of particles. */
    RGSS_Range fade;          /** A range indicating the speed at which particles lose opacity. */
    RGSS_Range rotation; /** A range determining the amount of rotation (in degrees) that particles will have applied.*/
    RGSS_Range growth;   /** The amount of scaling to apply through the particles lifetime. */
    RGSS_Range force;    /** A range determing the amount of force particles will initially have. */
    RGSS_Range rate;     /** A range determining the number of particles to spawn each game tick. */
    RGSS_Range frequency; /** A range indicating the number of ticks between each emission. */
    vec2 friction;        /** The amount of "inverse" of the particle's force and decreases it velocity. */
    float radius;         /** The radius from the emitter's postion that particles will spawn. */
    float gravity;        /** The amount of force applied on the y-axis. */
    float wind;           /** The amount of force applied on the x-axis. */
    int paused;           /** Flag indicating if emitter has been suspended from creating new particles. */
    int order;            /** The amount to change on the z-axis each tick to determine render ordering. */
    int interval;         /** The number of ticks to wait until the next emission. */
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

static inline float RGSS_Range_Rand(RGSS_Range *range)
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
    return a->depth > b->depth;
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
        e->texture.size[0] = (float)tex->width;
        e->texture.size[1] = (float)tex->height;
    }
    else
    {
        e->texture.id = GL_NONE;
        glm_vec3_zero(e->texture.size);
        // for (int i = 0; i < e->count; i++)
        //     glm_vec2_one(e->particles[i].scale);
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
    if (e->angles)
        xfree(e->angles);
    xfree(data);
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
    if (e->angle_vbo)
    {
        glDeleteBuffers(1, &e->angle_vbo);
        e->angle_vbo = GL_NONE;
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
    if (e->angles)
    {
        xfree(e->angles);
        e->angles = NULL;
    }
    return Qnil;
}

static void *RGSS_Emitter_CreateStorage(RGSS_Emitter *e, size_t item_size)
{
    size_t size = item_size * e->capacity;
    void *ptr = xmalloc(size);
    if (ptr == NULL)
        rb_raise(rb_eNoMemError, "out of memory");
    memset(ptr, 0, size);
    return ptr;
}

static void RGSS_Emitter_VertexSetup(RGSS_Emitter *e, int max)
{
    // Create the vertex array
    glGenVertexArrays(1, &e->base.vao);
    glBindVertexArray(e->base.vao);

    // Create buffers
    e->base.vbo = RGSS_CreateBuffer(GL_ARRAY_BUFFER, sizeof(RGSS_QUAD_VERTICES), RGSS_QUAD_VERTICES, GL_STATIC_DRAW);
    e->base.ebo =
        RGSS_CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, sizeof(RGSS_QUAD_INDICES), RGSS_QUAD_INDICES, GL_STATIC_DRAW);
    e->quad_vbo = RGSS_CreateBuffer(GL_ARRAY_BUFFER, PARTICLE_QUAD_SIZE * e->capacity, NULL, GL_STREAM_DRAW);
    e->color_vbo = RGSS_CreateBuffer(GL_ARRAY_BUFFER, PARTICLE_COLOR_SIZE * e->capacity, NULL, GL_STREAM_DRAW);
    e->angle_vbo = RGSS_CreateBuffer(GL_ARRAY_BUFFER, SIZEOF_FLOAT * e->capacity, NULL, GL_STREAM_DRAW);

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

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, e->angle_vbo);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);

    // Configure all the data is advanced in vertex shader
    glVertexAttribDivisor(0, 0); // Always reuse
    glVertexAttribDivisor(1, 1); // One per quad
    glVertexAttribDivisor(2, 1); // One per color
    glVertexAttribDivisor(3, 1); // One per color

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
    glUseProgram(RGSS_GRAPHICS.particle_shader.id);
    glUniform4fv(RGSS_GRAPHICS.particle_shader.color, 1, e->base.color);
    glUniform4fv(RGSS_GRAPHICS.particle_shader.tone, 1, e->base.tone);
    glUniform4fv(RGSS_GRAPHICS.particle_shader.flash, 1, e->base.flash_color);
    glUniform1f(RGSS_GRAPHICS.particle_shader.hue, e->base.hue);
    glUniform1f(RGSS_GRAPHICS.particle_shader.opacity, e->base.opacity);

    glActiveTexture(GL_TEXTURE0);
    if (e->texture.id)
    {
        glBindTexture(GL_TEXTURE_2D, e->texture.id);
        glUniform1i(RGSS_GRAPHICS.particle_shader.textured, GL_TRUE);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
        glUniform1i(RGSS_GRAPHICS.particle_shader.textured, GL_FALSE);
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
        RGSS_PackColor(vec, (unsigned int *)&p->color);
        return;
    }

    rb_raise(rb_eTypeError, "%s is not a Color, Array, Range, or NilClass", CLASS_NAME(e->spectrum));

RESULT:
    RGSS_PackColor(DATA_PTR(c), (unsigned int *)&p->color);
}

static inline void RGSS_Emitter_RenewParticle(RGSS_Emitter *e, RGSS_Particle *p)
{
    // Select initial starting point of particle, using uniform (radial) distribution.
    // https://programming.guide/random-point-within-circle.html
    float a = RGSS_Rand() * 2.0f * GLM_PI;
    float r = e->radius * sqrtf(RGSS_Rand());
    p->position[0] = e->base.entity.position[0] + (r * cosf(a));
    p->position[1] = e->base.entity.position[1] + (r * sinf(a));
    p->depth = 0;

    // Configure initial speed and direction
    float direction = glm_rad(RGSS_Range_Rand(&e->direction));
    float speed = RGSS_Range_Rand(&e->force);
    p->velocity[0] = sinf(direction) * speed;
    p->velocity[1] = cosf(direction) * speed;

    float sz = RGSS_Range_Rand(&e->size);
    p->size[0] = sz;
    p->size[1] = sz;

    if (e->texture.id != 0)
    {
        p->scale[0] = sz / e->texture.size[0];
        p->scale[1] = sz / e->texture.size[1];
    }
    else
    {
        glm_vec2_one(p->scale);
    }
    glm_vec2_mul(p->scale, e->base.entity.scale, p->scale);

    p->angle = RGSS_Rand() * 360.0f; // TODO
    p->rotation = RGSS_Range_Rand(&e->rotation);
    p->fade = RGSS_Range_Rand(&e->fade);
    p->growth = RGSS_Range_Rand(&e->growth);
    p->life = (int)roundf(RGSS_Range_Rand(&e->lifespan));
    RGSS_Emitter_SetParticleColor(e, p);
    p->opacity = p->color[3];
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

static inline void RGSS_Emitter_Emit(RGSS_Emitter *e)
{
    if (e->paused)
        return;

    if (e->interval > 0)
    {
        e->interval--;
        return;
    }

    if (e->interval == 0)
        e->interval = (int)RGSS_Range_Rand(&e->frequency);

    int count = (int)RGSS_Range_Rand(&e->rate);
    for (int i = 0; i < count; i++)
    {
        GLuint index = RGSS_Emitter_GetIndex(e);
        RGSS_Emitter_RenewParticle(e, &e->particles[index]);
    }
}

static VALUE RGSS_Emitter_Update(VALUE self, VALUE delta)
{
    rb_call_super(1, &delta);

    RGSS_Emitter *e = DATA_PTR(self);
    float d = NUM2FLT(delta);
    RGSS_Emitter_Emit(e);

    RGSS_Particle *p;
    int count = 0;
    for (int i = 0; i < e->capacity; i++)
    {
        p = &e->particles[i];
        if (--p->life > 0)
        {
            // Resize.
            glm_vec2_adds(p->scale, p->growth * d, p->scale);

            // Fade out.
            p->opacity -= p->fade * d;
            p->color[3] = (unsigned char)ceilf(p->opacity);

            // Early out of this iteration has 0 opacity or scales to nothing
            if (p->opacity < FLT_EPSILON || p->scale[0] < FLT_EPSILON || p->scale[1] < FLT_EPSILON)
            {
                p->life = -1;
                continue;
            }

            // Rotate.
            p->angle += p->rotation * d;

            // Normalize friction to game delta
            p->velocity[0] *= 1.0f - ((e->friction[0] * d) / RGSS_GAME.time.tps);
            p->velocity[1] *= 1.0f - ((e->friction[1] * d) / RGSS_GAME.time.tps);

            p->velocity[0] += e->wind * d;
            p->velocity[1] += e->gravity * d;

            // Move on Z-axis to determine draw order.
            p->depth += e->order;

            // Translate
            glm_vec3_muladds(p->velocity, d, p->position);

            // Fill the GPU buffers
            e->quads[4 * count + 0] = p->position[0];
            e->quads[4 * count + 1] = p->position[1];
            e->quads[4 * count + 2] = p->size[0] * p->scale[0];
            e->quads[4 * count + 3] = p->size[1] * p->scale[1];

            e->colors[4 * count + 0] = p->color[0];
            e->colors[4 * count + 1] = p->color[1];
            e->colors[4 * count + 2] = p->color[2];
            e->colors[4 * count + 3] = p->color[3];

            e->angles[count] = glm_rad(p->angle);

            count++;
        }
    }

    // Store how many particles will be rendered, then sort the particles for proper render order.
    e->count = count;
    qsort(e->particles, e->capacity, sizeof(RGSS_Particle), RGSS_SortParticle);

    // Orphan existing buffers before refilling with updated data.
    // https://www.khronos.org/opengl/wiki/Buffer_Object_Streaming#Buffer_re-specification

    glBindBuffer(GL_ARRAY_BUFFER, e->quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, e->capacity * PARTICLE_QUAD_SIZE, NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * PARTICLE_QUAD_SIZE, e->quads);

    glBindBuffer(GL_ARRAY_BUFFER, e->color_vbo);
    glBufferData(GL_ARRAY_BUFFER, e->capacity * PARTICLE_COLOR_SIZE, NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * PARTICLE_COLOR_SIZE, e->colors);

    glBindBuffer(GL_ARRAY_BUFFER, e->angle_vbo);
    glBufferData(GL_ARRAY_BUFFER, e->capacity * SIZEOF_FLOAT, NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * SIZEOF_FLOAT, e->angles);

    return Qnil;
}

ATTR_ACCESSOR(RGSS_Emitter, Gravity, gravity, DBL2NUM, NUM2FLT)
ATTR_ACCESSOR(RGSS_Emitter, Wind, wind, DBL2NUM, NUM2FLT)
ATTR_READER(RGSS_Emitter, Radius, radius, DBL2NUM)

static VALUE RGSS_Emitter_SetRadius(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    if (NIL_P(value))
    {
        // Distance from the center of screen to corner (full resolution coverage)
        e->radius = glm_vec2_norm(RGSS_GAME.graphics.resolution) * 0.5f;
    }
    else
    {
        e->radius = NUM2FLT(value);
    }
    glm_vec2_fill(e->base.entity.size, e->radius * 2);
    return value;
}

EMITTER_GET_RANGE(Lifespan, lifespan)
EMITTER_GET_RANGE(Direction, direction)
EMITTER_GET_RANGE(Force, force)
EMITTER_GET_RANGE(Rate, rate)
EMITTER_GET_RANGE(Growth, growth)
EMITTER_GET_RANGE(Fade, fade)
EMITTER_GET_RANGE(Rotation, rotation)
EMITTER_GET_RANGE(Frequency, frequency)
EMITTER_GET_RANGE(ParticleSize, size)

EMITTER_SET_RANGE(Lifespan, lifespan)
EMITTER_SET_RANGE(Force, force)
EMITTER_SET_RANGE(Rate, rate)
EMITTER_SET_RANGE(Growth, growth)
EMITTER_SET_RANGE(Fade, fade)
EMITTER_SET_RANGE(Rotation, rotation)
EMITTER_SET_RANGE(Frequency, frequency)
EMITTER_SET_RANGE(ParticleSize, size)

ATTR_READER(RGSS_Emitter, Interval, interval, INT2NUM)
ATTR_READER(RGSS_Emitter, Capacity, capacity, INT2NUM)
ATTR_ACCESSOR(RGSS_Emitter, Order, order, INT2NUM, NUM2INT)

static VALUE RGSS_Emitter_SetDirection(VALUE self, VALUE value)
{
    RGSS_Emitter *e = DATA_PTR(self);
    if (NIL_P(value))
        e->direction = (RGSS_Range){0.0f, 360.0f};
    else
        RGSS_Range_Parse(value, &e->direction);
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

static int RGSS_Emitter_KeyParse(VALUE key, VALUE value, VALUE self)
{
    if (rb_type(key) != T_SYMBOL)
        return ST_CONTINUE;

    VALUE symname = rb_sym2str(key);
    VALUE name = rb_sprintf("%s=", StringValueCStr(symname));

    ID id = rb_intern_str(name);
    if (rb_respond_to(self, id))
        rb_funcall2(self, id, 1, &value);

    return ST_CONTINUE;
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

    // Set some sane defaults
    e->radius = 1.0f;
    e->capacity = NUM2INT(max);
    e->lifespan = (RGSS_Range){RGSS_GAME.time.tps * 3, RGSS_GAME.time.tps * 5};
    e->force = (RGSS_Range){10, 100};
    e->rate = (RGSS_Range){3, 5};
    e->size = (RGSS_Range){1.0f, 1.0f};
    e->direction = (RGSS_Range){0.0f, 360.0f};
    e->interval = -1;
    e->frequency = (RGSS_Range){-1.0f, -1.0f};
    e->order = -1;

    // Initialize buffers
    e->particles = RGSS_Emitter_CreateStorage(e, sizeof(RGSS_Particle));
    e->quads = RGSS_Emitter_CreateStorage(e, PARTICLE_QUAD_SIZE);
    e->colors = RGSS_Emitter_CreateStorage(e, PARTICLE_COLOR_SIZE);
    e->angles = RGSS_Emitter_CreateStorage(e, SIZEOF_FLOAT);

    RGSS_Emitter_VertexSetup(e, e->capacity);

    if (RTEST(opts))
        rb_hash_foreach(opts, RGSS_Emitter_KeyParse, self);

    if (rb_block_given_p())
        rb_yield(self);

    return self;
}

static VALUE RGSS_Emitter_Pause(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    e->paused = true;
    return self;
}

static VALUE RGSS_Emitter_Resume(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    e->paused = false;
    return self;
}

static VALUE RGSS_Emitter_IsPaused(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    return RB_BOOL(e->paused);
}

static VALUE RGSS_Emitter_FullScreen(VALUE self)
{
    RGSS_Emitter *e = DATA_PTR(self);
    float hypot = glm_vec2_norm(RGSS_GRAPHICS.resolution);
    e->radius = hypot * 0.5f;
    glm_vec2_fill(e->base.entity.size, hypot);
    glm_vec2_scale(RGSS_GRAPHICS.resolution, 0.5f, e->base.entity.position);
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
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Rotation, "rotation");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Fade, "fade");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Growth, "growth");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Direction, "direction");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Frequency, "frequency");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, ParticleSize, "particle_size");
    DEFINE_ACCESSOR(rb_cEmitter, RGSS_Emitter, Order, "order");

    rb_define_method0(rb_cEmitter, "capacity", RGSS_Emitter_GetCapacity, 0);
    rb_define_method0(rb_cEmitter, "interval", RGSS_Emitter_GetInterval, 0);
    rb_define_method0(rb_cEmitter, "pause", RGSS_Emitter_Pause, 0);
    rb_define_method0(rb_cEmitter, "paused?", RGSS_Emitter_IsPaused, 0);
    rb_define_method0(rb_cEmitter, "resume", RGSS_Emitter_Resume, 0);
    rb_define_method0(rb_cEmitter, "dispose", RGSS_Emitter_Dispose, 0);
    rb_define_method1(rb_cEmitter, "update", RGSS_Emitter_Update, 1);
    rb_define_method1(rb_cEmitter, "render", RGSS_Emitter_Render, 1);
    rb_define_method0(rb_cEmitter, "fullscreen", RGSS_Emitter_FullScreen, 0);

    rb_define_const(rb_cEmitter, "EARTH_GRAVITY", DBL2NUM(9.81));
    rb_define_const(rb_cEmitter, "ORDER_YOUNGEST", INT2NUM(-1));
    rb_define_const(rb_cEmitter, "ORDER_OLDEST", INT2NUM(1));
}