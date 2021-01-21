#ifndef RGSS_GAME_H
#define RGSS_GAME_H 1

#include "glad.h"
#include "rgss.h"

#define GLFW_INCLUDE_NONE 1
#include <GLFW/glfw3.h>

#include "uthash.h"
#include "vec.h"

#define RGSS_ASSERT_GAME (RUBY_ASSERT_MESG(RGSS_GAME.window != NULL, "Game has not been initialized"))

#define VEC_ATTR_READER(type, field, offset)                                                                           \
    static VALUE type##_get_##field(VALUE self)                                                                        \
    {                                                                                                                  \
        return DBL2NUM(((float *)DATA_PTR(self))[offset]);                                                             \
    }
#define VEC_ATTR_WRITER(type, field, offset)                                                                           \
    static VALUE type##_set_##field(VALUE self, VALUE value)                                                           \
    {                                                                                                                  \
        ((float *)DATA_PTR(self))[offset] = NUM2FLT(value);                                                            \
        return value;                                                                                                  \
    }
#define VEC_ATTR_ACCESSOR(type, field, offset)                                                                         \
    VEC_ATTR_READER(type, field, offset)                                                                               \
    VEC_ATTR_WRITER(type, field, offset)

#define CREATE_ALLOC_FUNC(type, align, size)                                                                           \
    static VALUE type##_alloc(VALUE klass)                                                                             \
    {                                                                                                                  \
        float *v = RGSS_MALLOC_ALIGNED(size, align);                                                                   \
        return Data_Wrap_Struct(klass, NULL, free, v);                                                                 \
    }

typedef enum
{
    RGSS_FLIP_NONE = 0x00,
    RGSS_FLIP_X = 0x01,
    RGSS_FLIP_Y = 0x02,
    RGSS_FLIP_BOTH = (RGSS_FLIP_X | RGSS_FLIP_Y)
} RGSS_Flip;

typedef struct
{
    GLuint id;
    GLuint fbo;
    int width;
    int height;
} RGSS_Texture;

typedef struct
{
    GLenum op;
    GLenum src;
    GLenum dst;
} RGSS_Blend;

typedef struct RGSS_Entity
{
    vec4 *model;
    vec3 position;
    vec3 velocity;
    vec3 scale;
    vec3 pivot;
    float angle;
    int depth;
    vec3 size;
} RGSS_Entity;

typedef struct RGSS_Renderable
{
    RGSS_Entity entity;
    GLuint vao, vbo, ebo;
    RGSS_Color color;
    RGSS_Tone tone;
    float opacity;
    int visible;
    float hue;
    RGSS_Flip flip;
    RGSS_Color flash_color;
    int flash_duration;
    RGSS_Blend blend;
    VALUE parent;
} RGSS_Renderable;

typedef struct
{
    ID id;
    vec_int_t keys;
    vec_int_t mouse_buttons;
    vec_int_t gamepad_buttons;
    UT_hash_handle hh;
} RGSS_Mapping;

typedef struct
{
    int invalid;
    vec_t(VALUE) items;
} RGSS_Batch;

// typedef struct
// {
//     int updated;
//     vec_t(VALUE) items;
// } RGSS_Batch;

typedef struct
{
    GLFWwindow *window;
    int debug;
    int auto_ortho;
    double speed;
    int rect[4];
    struct
    {
        double target_tps;
        double last;
        double time_count;
        double fps;
        double fps_count;
        double ticks;
        double tick_count;
        uint64_t total_frames;
        uint64_t total_ticks;
    } time;
    struct
    {
        vec4 *projection;
        vec2 resolution;
        vec2 ratio;
        GLuint ubo;
        RGSS_Rect viewport;
        RGSS_Color color;
        RGSS_Batch batch;
        struct
        {
            GLuint id;
            GLint model;
            GLint hue;
            GLint color;
            GLint tone;
            GLint flash;
            GLint opacity;
        } shader;
    } graphics;
    struct
    {
        struct
        {
            char key[GLFW_KEY_LAST + 1];
            char mouse[GLFW_MOUSE_BUTTON_LAST + 1];
            char gamepad[GLFW_GAMEPAD_BUTTON_LAST + 1];
            double scroll_x, scroll_y;
        } state;
        RGSS_Mapping *bindings;
        GLFWcursor *cursor;
    } input;
} RGSS_Game;

void RGSS_Graphics_Init(GLFWwindow *window, int width, int height, int vsync);
void RGSS_Graphics_Deinit(GLFWwindow *window);
void RGSS_Graphics_Render(double alpha);

void RGSS_Input_Init(GLFWwindow *window);
void RGSS_Input_Deinit(GLFWwindow *window);
void RGSS_Input_Update(void);

VALUE RGSS_Graphics_Restore(VALUE graphics);


extern RGSS_Game RGSS_GAME;

static inline void RGSS_ParseOpt(VALUE opts, const char *name, int ifnone, int *result)
{
    if (result == NULL)
        return;

    if (opts == Qnil)
        *result = ifnone;
    else
    {
        VALUE opt = rb_hash_aref(opts, STR2SYM(name));
        if (FIXNUM_P(opt))
            *result = NUM2INT(opt);
        else
            *result = (opt == Qnil) ? ifnone : RTEST(opt);
    }
}


int RGSS_Batch_Sort(const void *obj1, const void *obj2);
VALUE RGSS_Batch_Add(VALUE self, VALUE obj);
VALUE RGSS_Batch_Remove(VALUE self, VALUE obj);
VALUE RGSS_Batch_Invalidate(VALUE self);

extern ID RGSS_ID_UPDATE_VERTICES;
extern ID RGSS_ID_BATCH;
extern ID RGSS_ID_RENDER;
extern ID RGSS_ID_UPDATE;

#define ATTR_READER(type, attr, field, to_ruby)                                                                        \
    static VALUE type##_Get##attr(VALUE self)                                                                          \
    {                                                                                                                  \
        return to_ruby(((type *)DATA_PTR(self))->field);                                                               \
    }

#define ATTR_WRITER(type, attr, field, to_c)                                                                           \
    static VALUE type##_Set##attr(VALUE self, VALUE value)                                                             \
    {                                                                                                                  \
        ((type *)DATA_PTR(self))->field = to_c(value);                                                                 \
        return value;                                                                                                  \
    }

#define ATTR_ACCESSOR(type, attr, field, to_ruby, to_c)                                                                \
    ATTR_READER(type, attr, field, to_ruby)                                                                            \
    ATTR_WRITER(type, attr, field, to_c)

#define DEFINE_ACCESSOR(klass, type, attr, name) \
rb_define_method0(klass,  name, type##_##Get##attr, 0);\
rb_define_method1(klass, name"=" , type##_##Set##attr, 1)





#endif /* RGSS_GAME_H */