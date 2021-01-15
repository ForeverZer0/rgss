#ifndef RB_RGSS_H
#define RB_RGSS_H 1

#include <errno.h>
#include <ruby.h>
#include "cglm/cglm.h"

#define NUM2FLT(v)      ((float)NUM2DBL(v))
#define RB_BOOL(expr)   ((expr) ? Qtrue : Qfalse)
#define CLASS_NAME(obj) rb_class2name(CLASS_OF(obj))
#define STR2SYM(str)    ID2SYM(rb_intern(str))

#define RGSS_DEBUG 1

extern VALUE rb_mRGSS;

extern VALUE rb_mGLFW;
extern VALUE rb_cWindow;
extern VALUE rb_cMonitor;
extern VALUE rb_cCursor;
extern VALUE rb_cVideoMode;
extern VALUE rb_cGamepad;
extern VALUE rb_cGammaRamp;

extern VALUE rb_mGL;
extern VALUE rb_mAL;
extern VALUE rb_mALC;
extern VALUE rb_cSound;
extern VALUE rb_cImage;


extern VALUE rb_cEntity;
extern VALUE rb_cTrueType;

extern VALUE rb_cColor;
extern VALUE rb_cPoint;
extern VALUE rb_cSize;
extern VALUE rb_cRect;
extern VALUE rb_cVec2;
extern VALUE rb_cVec3;
extern VALUE rb_cVec4;
extern VALUE rb_cMat3; // TODO
extern VALUE rb_cMat4; // TODO

void RGSS_Init_GLFW(VALUE parent);
void RGSS_Init_GL(VALUE parent);
void RGSS_Init_Image(VALUE parent);
void RGSS_Init_Game(VALUE parent);
void RGSS_Init_Graphics(VALUE parent);
void RGSS_Init_Input(VALUE parent);

void RGSS_Init_ColorAndTone(VALUE parent);
void RGSS_Init_PointAndSize(VALUE parent);
void RGSS_Init_Rect(VALUE parent);
void RGSS_Init_Vec2(VALUE parent);
void RGSS_Init_Vec3(VALUE parent);
void RGSS_Init_Vec4(VALUE parent);
void RGSS_Init_Mat4(VALUE parent);

void RGSS_Init_Batch(VALUE parent);
void RGSS_Init_Entity(VALUE parent);
void RGSS_Init_TrueType(VALUE parent);

VALUE RGSS_Handle_Alloc(VALUE klass);

typedef vec4 RGSS_Color;
typedef vec4 RGSS_Tone;

typedef struct
{
    int x, y;
} RGSS_Point;

typedef struct
{
    int width, height;
} RGSS_Size;

typedef union {
    struct
    {
        int x, y, width, height;
    };
    struct
    {
        RGSS_Point location;
        RGSS_Size size;
    };
} RGSS_Rect;

static inline VALUE RGSS_IVec2_New(VALUE klass, int v0, int v1)
{
    int *vec = xmalloc(sizeof(int) * 2);
    vec[0] = v0;
    vec[1] = v1;
    return Data_Wrap_Struct(klass, NULL, RUBY_DEFAULT_FREE, vec);
}

static inline VALUE RGSS_IVec4_New(VALUE klass, int x, int y, int w, int h)
{
    int *vec = xmalloc(sizeof(int) * 4);
    vec[0] = x;
    vec[1] = y;
    vec[2] = w;
    vec[3] = h;
    return Data_Wrap_Struct(klass, NULL, RUBY_DEFAULT_FREE, vec);
}

#define RGSS_Point_New(x, y)               RGSS_IVec2_New(rb_cPoint, x, y)
#define RGSS_Size_New(width, height)       RGSS_IVec2_New(rb_cSize, width, height)
#define RGSS_Rect_New(x, y, width, height) RGSS_IVec4_New(rb_cRect, x, y, width, height)

#if SIZEOF_VOIDP == SIZEOF_LONG
#define PTR2NUM(x) (LONG2NUM((long)(x)))
#define NUM2PTR(x) ((void *)(NUM2ULONG(x)))
#else
/* # error --->> Ruby/DL2 requires sizeof(void*) == sizeof(long) to be compiled. <<--- */
#define PTR2NUM(x) (LL2NUM((LONG_LONG)(x)))
#define NUM2PTR(x) ((void *)(NUM2ULL(x)))
#endif

#define RGSS_VEC2_SIZE (sizeof(float) * 2)
#define RGSS_VEC3_SIZE (sizeof(float) * 3)
#define RGSS_VEC4_SIZE (sizeof(float) * 4)
#define RGSS_MAT3_SIZE (RGSS_VEC3_SIZE * 3)
#define RGSS_MAT4_SIZE (RGSS_VEC4_SIZE * 4)

#define RGSS_VEC2_ALIGN (sizeof(float) * 2)
#define RGSS_VEC3_ALIGN (sizeof(float) * 4)
#define RGSS_VEC4_ALIGN (sizeof(float) * 4)
#define RGSS_MAT3_ALIGN (sizeof(float) * 4)
#define RGSS_MAT4_ALIGN (sizeof(float) * 4)

#define RGSS_VEC2_NEW (RGSS_MALLOC_ALIGNED(RGSS_VEC2_SIZE, RGSS_VEC2_ALIGN))
#define RGSS_VEC3_NEW (RGSS_MALLOC_ALIGNED(RGSS_VEC3_SIZE, RGSS_VEC3_ALIGN))
#define RGSS_VEC4_NEW (RGSS_MALLOC_ALIGNED(RGSS_VEC4_SIZE, RGSS_VEC4_ALIGN))
#define RGSS_MAT3_NEW (RGSS_MALLOC_ALIGNED(RGSS_MAT3_SIZE, RGSS_MAT3_ALIGN))
#define RGSS_MAT4_NEW (RGSS_MALLOC_ALIGNED(RGSS_MAT4_SIZE, RGSS_MAT4_ALIGN))

#define VEC2_WRAP(v) Data_Wrap_Struct(rb_cVec2, NULL, free, v)
#define VEC3_WRAP(v) Data_Wrap_Struct(rb_cVec3, NULL, free, v)
#define VEC4_WRAP(v) Data_Wrap_Struct(rb_cVec4, NULL, free, v)
#define MAT3_WRAP(m) Data_Wrap_Struct(rb_cMat3, NULL, free, m)
#define MAT4_WRAP(m) Data_Wrap_Struct(rb_cMat4, NULL, free, m)

static inline void *RGSS_MALLOC_ALIGNED(size_t size, size_t alignment)
{
    void *mem = NULL;
    int err = posix_memalign(&mem, alignment, size);
    switch (err)
    {
        case EINVAL:
            rb_raise(rb_eRuntimeError,
                     "alignment %zu was not a power of two, or was not a multiple of sizeof(void *) == %zu", alignment,
                     sizeof(void *));
        case ENOMEM:
            rb_raise(rb_eNoMemError, "out of memory");
    }
    return mem;
}

static inline VALUE RGSS_Vec2_New(float x, float y)
{
    float *v = RGSS_VEC2_NEW;
    v[0] = x;
    v[1] = y;
    return Data_Wrap_Struct(rb_cVec2, NULL, free, v);
}

static inline VALUE RGSS_Vec3_New(float x, float y, float z)
{
    float *v = RGSS_VEC3_NEW;
    v[0] = x;
    v[1] = y;
    v[2] = z;
    return Data_Wrap_Struct(rb_cVec3, NULL, free, v);
}

static inline VALUE RGSS_Vec4_New(float x, float y, float z, float w)
{
    float *v = RGSS_VEC4_NEW;
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = w;
    return Data_Wrap_Struct(rb_cVec4, NULL, free, v);
}

static inline void *rpg_value_ptr(VALUE value)
{
    if (value == Qnil)
        return NULL;
        
    if (RB_TYPE_P(value, T_DATA))
        return DATA_PTR(value);

    if (RB_TYPE_P(value, T_STRING))
        return StringValuePtr(value);

    if (RB_INTEGER_TYPE_P(value))
        return NUM2PTR(value);

    return NULL;
}

VALUE RGSS_Color_New(VALUE klass, float r, float g, float b, float a);
void RGSS_Image_Load(const char *path, int *width, int *height, unsigned char **pixels);

#define RB2PTR(value) rpg_value_ptr(value)

#define RGSS_DEFINE_DUMP(type)                                                                                          \
    static VALUE type##_Dump(int argc, VALUE *argv, VALUE self)                                                        \
    {                                                                                                                  \
        return rb_str_new((const char *)DATA_PTR(self), sizeof(type));                                                 \
    }

#define RGSS_DEFINE_LOAD_EX(type, mark_func, free_func)                                                                 \
    static VALUE type##_Load(VALUE klass, VALUE str)                                                                   \
    {                                                                                                                  \
        const char *bin = StringValuePtr(str);                                                                         \
        type *obj = ALLOC(type);                                                                                       \
        memcpy(obj, bin, sizeof(type));                                                                                \
        return Data_Wrap_Struct(klass, mark_func, free_func, obj);                                                     \
    }

#define RGSS_DEFINE_LOAD(type) RGSS_DEFINE_LOAD_EX(type, NULL, RUBY_DEFAULT_FREE)

#define RGSS_DEFINE_MARSHAL(type)                                                                                       \
    RGSS_DEFINE_DUMP(type)                                                                                              \
    RGSS_DEFINE_LOAD(type)

#define RGSS_DEFINE_MARSHAL_EX(type, mark_func, free_func)                                                              \
    RGSS_DEFINE_DUMP(type)                                                                                              \
    RGSS_DEFINE_LOAD_EX(type, mark_func, free_func)

#define RGSS_ASSERT_UNFROZEN(value)                                                                                     \
    if (rb_obj_frozen_p(value))                                                                                        \
    rb_raise(rb_eFrozenError, "cannot modify frozen %s", CLASS_NAME(self))

#define RGSS_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define RGSS_MIN(a, b) (((a) < (b)) ? (a) : (b))

#endif /* RB_RGSS_H */
