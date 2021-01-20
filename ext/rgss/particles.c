#include "game.h"

VALUE rb_cEmitter;
GLuint RGSS_PARTICLE_SHADER;

typedef struct
{
    vec2 position;
    vec2 center;
    vec2 velocity;
    RGSS_Color color;

    // Rate of change
    float fade;
    float scale;
    float zoom;
    float friction;
    float angle;
    float angular_velocity;
    float time_to_live;
} RGSS_Particle;

typedef struct
{
    RGSS_Renderable base;
    VALUE viewport;
    struct
    {
        VALUE value;
        GLuint id;
        GLfloat left, right, top, bottom;
    } texture;

    GLfloat gravity;

 // Generating particles.
    vec2 angular_velocity;
    vec2 center_x;
    vec2 center_y;
    RGSS_Color color;
    vec2 fade;
    vec2 friction;
    vec2 offset; // Distance from origin to spawn.
    vec2 scale;
    vec2 speed;
    vec2 time_to_live;
    vec2 zoom;

    // When to emit.
    vec2 interval;
    GLfloat time_until_emit;

    // Managing the particles themselves.
    GLuint count; // Current number of active particles.
    GLuint max_particles; // No more will be created if max hit.
    GLuint next_particle_index; // Next place to create a new particle (either dead or oldest living).

    RGSS_Particle* particles;

    GLuint *colors;
    vec2 *coords;
    vec2 *vertices;

    GLsizei offset_color;
    GLsizei offset_coords;
    GLsizei offset_vertices;
} RGSS_Emitter;

static void RGSS_Emitter_Free(void *data)
{
    if (data == NULL)
        return;

    RGSS_Emitter *e = data;
    xfree(e->particles);

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
    e->viewport = Qnil;
    e->texture.value = Qnil;
    return Data_Wrap_Struct(klass, RGSS_Emitter_Mark, RGSS_Emitter_Free, e);
}

static VALUE RGSS_Emitter_Initialize(int argc, VALUE *argv, VALUE self)
{

}

void RGSS_Init_Particles(VALUE parent)
{
    rb_cEmitter = rb_define_class_under(rb_mRGSS, "Emitter", rb_cObject);
    rb_define_alloc_func(rb_cEmitter, RGSS_Emitter_Alloc);

    rb_define_methodm1(rb_cEmitter, "initialize", RGSS_Emitter_Initialize, -1);
}