#ifndef RGSS_GAME_H
#define RGSS_GAME_H 1

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
#define VEC_ATTR_ACCESSOR(type, field, offset)                                                                             \
    VEC_ATTR_READER(type, field, offset)                                                                               \
    VEC_ATTR_WRITER(type, field, offset)

#define CREATE_ALLOC_FUNC(type, align, size)                                                                           \
    static VALUE type##_alloc(VALUE klass)                                                                             \
    {                                                                                                                  \
        float *v = RGSS_MALLOC_ALIGNED(size, align);                                                                    \
        return Data_Wrap_Struct(klass, NULL, free, v);                                                                 \
    }

typedef struct
{
    ID id;
    vec_int_t keys;
    vec_int_t mouse_buttons;
    vec_int_t gamepad_buttons;
    UT_hash_handle hh;
} RGSS_Mapping;

// typedef struct
// {
//     int updated;
//     vec_t(VALUE) items;
// } RGSS_Batch;

typedef struct
{
    GLFWwindow *window;
    int debug;
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
        uint32_t ubo;
        vec2 resolution;
        vec2 ratio;
        RGSS_Rect viewport;
        RGSS_Color color;
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

void RGSS_Image_Free(void *img);
void RGSS_Image_SavePNG(const char *filename, int width, int height, unsigned char *pixels);

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

static inline char *RGSS_FileRead(VALUE path)
{
    if (NIL_P(path))
        rb_raise(rb_eArgError, "path cannot be nil");

    char *filename = StringValueCStr(path);
    struct stat st;
    if (stat(filename, &st) != 0)
    {
        VALUE enoent = rb_const_get(rb_mErrno, rb_intern("ENOENT"));
        rb_raise(enoent, filename);
    }

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
        rb_raise(rb_eIOError, "failed to open %s", filename);

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buffer = xmalloc(len + 1);
    if (buffer == NULL)
        rb_raise(rb_eNoMemError, "out of memory");

    size_t rd = fread(buffer, 1, len, fp);
    if (rd != len)
        rb_raise(rb_eIOError, "failed to read %s", filename);

    buffer[0] = '\0';
    return buffer;
}

#endif /* RGSS_GAME_H */