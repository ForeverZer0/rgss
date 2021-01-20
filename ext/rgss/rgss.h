#ifndef RGSS_H
#define RGSS_H 1

#include "cglm/cglm.h"
#include <errno.h>
#include <ruby.h>

#define NUM2FLT(v)      ((float)NUM2DBL(v))
#define RB_BOOL(expr)   ((expr) ? Qtrue : Qfalse)
#define CLASS_NAME(obj) rb_class2name(CLASS_OF(obj))
#define STR2SYM(str)    ID2SYM(rb_intern(str))

extern VALUE rb_mRGSS;
extern VALUE rb_eRGSSError;

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

extern VALUE rb_mGraphics;

extern VALUE rb_cBatch;
extern VALUE rb_cEntity;
extern VALUE rb_cRenderable;
extern VALUE rb_cViewport;
extern VALUE rb_cSprite;
extern VALUE rb_cPlane;
extern VALUE rb_cTexture;

extern VALUE rb_cFont;

extern VALUE rb_cTable;
extern VALUE rb_cColor;
extern VALUE rb_cTone;
extern VALUE rb_cIVec2;
extern VALUE rb_cPoint;
extern VALUE rb_cSize;
extern VALUE rb_cRect;
extern VALUE rb_cVec2;
extern VALUE rb_cVec3;
extern VALUE rb_cVec4;
extern VALUE rb_cMat3; // TODO ?
extern VALUE rb_cMat4; // TODO

void RGSS_Init_GLFW(VALUE parent);
void RGSS_Init_GL(VALUE parent);
void RGSS_Init_Image(VALUE parent);
void RGSS_Init_Game(VALUE parent);
void RGSS_Init_Graphics(VALUE parent);
void RGSS_Init_Input(VALUE parent);

void RGSS_Init_Table(VALUE parent);
void RGSS_Init_ColorAndTone(VALUE parent);
void RGSS_Init_PointAndSize(VALUE parent);
void RGSS_Init_Rect(VALUE parent);
void RGSS_Init_Vectors(VALUE parent);
void RGSS_Init_Mat4(VALUE parent);

void RGSS_Init_Batch(VALUE parent);
void RGSS_Init_Entity(VALUE parent);
void RGSS_Init_Font(VALUE parent);
void RGSS_Init_Texture(VALUE parent);

VALUE RGSS_Handle_Alloc(VALUE klass);

#define RGSS_HAS_FLAG(value, flag) (((value) & (flag)) != 0)

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

static inline VALUE RGSS_Rect_New(int x, int y, int w, int h)
{
    int *vec = xmalloc(sizeof(int) * 4);
    vec[0] = x;
    vec[1] = y;
    vec[2] = w;
    vec[3] = h;
    return Data_Wrap_Struct(rb_cRect, NULL, RUBY_DEFAULT_FREE, vec);
}

#define RGSS_Point_New(x, y)         RGSS_IVec2_New(rb_cPoint, x, y)
#define RGSS_Size_New(width, height) RGSS_IVec2_New(rb_cSize, width, height)

#if SIZEOF_VOIDP == SIZEOF_LONG
#define PTR2NUM(x) (LONG2NUM((long)(x)))
#define NUM2PTR(x) ((void *)(NUM2ULONG(x)))
#else
/* # error --->> Ruby/DL2 requires sizeof(void*) == sizeof(long) to be compiled. <<--- */
#define PTR2NUM(x) (LL2NUM((LONG_LONG)(x)))
#define NUM2PTR(x) ((void *)(NUM2ULL(x)))
#endif

#define RGSS_VEC2_SIZE (SIZEOF_FLOAT * 2)
#define RGSS_VEC3_SIZE (SIZEOF_FLOAT * 3)
#define RGSS_VEC4_SIZE (SIZEOF_FLOAT * 4)
#define RGSS_MAT4_SIZE (RGSS_VEC4_SIZE * 4)

#define RGSS_VEC2_ALIGN (SIZEOF_FLOAT * 2)
#define RGSS_VEC3_ALIGN (SIZEOF_FLOAT * 4)
#define RGSS_VEC4_ALIGN (SIZEOF_FLOAT * 4)
#define RGSS_MAT4_ALIGN (SIZEOF_FLOAT * 4)

#define RGSS_VEC2_NEW (RGSS_MALLOC_ALIGNED(RGSS_VEC2_SIZE, RGSS_VEC2_ALIGN))
#define RGSS_VEC3_NEW (RGSS_MALLOC_ALIGNED(RGSS_VEC3_SIZE, RGSS_VEC3_ALIGN))
#define RGSS_VEC4_NEW (RGSS_MALLOC_ALIGNED(RGSS_VEC4_SIZE, RGSS_VEC4_ALIGN))
#define RGSS_MAT4_NEW (RGSS_MALLOC_ALIGNED(RGSS_MAT4_SIZE, RGSS_MAT4_ALIGN))

#define RGSS_VEC2_WRAP(v) Data_Wrap_Struct(rb_cVec2, NULL, free, v)
#define RGSS_VEC3_WRAP(v) Data_Wrap_Struct(rb_cVec3, NULL, free, v)
#define RGSS_VEC4_WRAP(v) Data_Wrap_Struct(rb_cVec4, NULL, free, v)
#define RGSS_MAT4_WRAP(m) Data_Wrap_Struct(rb_cMat4, NULL, free, m)

extern const char *SPRITE_VERT_SRC;
extern const char *SPRITE_FRAG_SRC;

static inline void *RGSS_MALLOC_ALIGNED(size_t size, size_t alignment)
{
    void *mem = NULL;
    int err = posix_memalign(&mem, alignment, size);
    switch (err)
    {
        case EINVAL:
            rb_raise(rb_eRGSSError, "invalid alignment specified");
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

static inline void *RGSS_ValuePointer(VALUE value)
{
    if (value == Qnil)
        return NULL;

    // TODO: Check for Fiddle::Pointer type

    if (RB_TYPE_P(value, T_DATA))
        return DATA_PTR(value);

    if (RB_TYPE_P(value, T_STRING))
        return StringValuePtr(value);

    if (RB_INTEGER_TYPE_P(value))
        return NUM2PTR(value);

    return NULL;
}

static inline void RGSS_SizeNotEmpty(int width, int height)
{
    if (width < 1)
        rb_raise(rb_eArgError, "width must be greater than 0 (given $d)", width);
    if (height < 1)
        rb_raise(rb_eArgError, "height must be greater than 0 (given %d)", height);
}

static inline void RGSS_PackColor(RGSS_Color color, unsigned int *value)
{
    unsigned char *rgba = (unsigned char *)value;
    rgba[0] = (unsigned char)roundf(color[0] * 255.0f);
    rgba[1] = (unsigned char)roundf(color[1] * 255.0f);
    rgba[2] = (unsigned char)roundf(color[2] * 255.0f);
    rgba[3] = (unsigned char)roundf(color[3] * 255.0f);
}

static inline void RGSS_UnpackColor(unsigned int value, RGSS_Color color)
{
    unsigned char *rgba = (unsigned char *)&value;
    color[0] = (float)rgba[0] / 255.0f;
    color[1] = (float)rgba[1] / 255.0f;
    color[2] = (float)rgba[2] / 255.0f;
    color[3] = (float)rgba[3] / 255.0f;
}

VALUE RGSS_Color_New(VALUE klass, float r, float g, float b, float a);


#define RB2PTR(value) RGSS_ValuePointer(value)

#define RGSS_DEFINE_DUMP(type)                                                                                         \
    static VALUE type##_Dump(int argc, VALUE *argv, VALUE self)                                                        \
    {                                                                                                                  \
        return rb_str_new((const char *)DATA_PTR(self), sizeof(type));                                                 \
    }

#define RGSS_DEFINE_LOAD_EX(type, mark_func, free_func)                                                                \
    static VALUE type##_Load(VALUE klass, VALUE str)                                                                   \
    {                                                                                                                  \
        const char *bin = StringValuePtr(str);                                                                         \
        type *obj = ALLOC(type);                                                                                       \
        memcpy(obj, bin, sizeof(type));                                                                                \
        return Data_Wrap_Struct(klass, mark_func, free_func, obj);                                                     \
    }

#define RGSS_DEFINE_LOAD(type) RGSS_DEFINE_LOAD_EX(type, NULL, RUBY_DEFAULT_FREE)

#define RGSS_DEFINE_MARSHAL(type)                                                                                      \
    RGSS_DEFINE_DUMP(type)                                                                                             \
    RGSS_DEFINE_LOAD(type)

#define RGSS_DEFINE_MARSHAL_EX(type, mark_func, free_func)                                                             \
    RGSS_DEFINE_DUMP(type)                                                                                             \
    RGSS_DEFINE_LOAD_EX(type, mark_func, free_func)

#define RGSS_ASSERT_UNFROZEN(value)                                                                                    \
    if (rb_obj_frozen_p(value))                                                                                        \
    rb_raise(rb_eFrozenError, "cannot modify frozen %s", CLASS_NAME(self))

#define RGSS_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define RGSS_MIN(a, b) (((a) < (b)) ? (a) : (b))

/**
 * @brief Read the context of the specified file descriptor into a buffer, raising an appropriate Ruby excpetion on failure.
 * @param[in] fd A valid file descriptor to read.
 * @return A null-terminated buffer containing the file text. The pointer must be freed when done with it.
 */
char *RGSS_ReadFileDescriptorText(int fd);

/**
 * @brief Read the context of the specified file into a buffer, raising an appropriate Ruby excpetion on failure.
 * @param[in] path The path of the file to read.
 * @return A null-terminated buffer containing the file text The pointer must be freed when done with it.
 */
char *RGSS_ReadFileText(const char *path);

/**
 * @brief Read the context of the specified Ruby file into a buffer, raising an appropriate Ruby excpetion on failure.
 * @param[in] source A Ruby File instance, a String path, or an Integer file descriptor.
 * @return A null-terminated buffer containing the file text. The pointer must be freed when done with it.
 */
char *RGSS_ReadFileTextRB(VALUE source);

/**
 * @brief Read the contents of the file specified file into a buffer, raising an appropriate Ruby excpetion on failure.
 * @param[in] path The path of the file to read.
 * @param[in,out] bufsize The number of bytes contained in the returned buffer.
 * @return A buffer containing the file contents. The pointer must be freed when done with it.
 */
void *RGSS_ReadFile(const char *path, long *bufsize);

/**
 * @brief Creates a new Ruby Image instance.
 * @param[in] width The width of the image, in pixels.
 * @param[in] height The height of the image, in pixels.
 * @param[in] pixels A pointer to a tightly packed RGBA pixel data, 4 bytes per pixel. The memory must not be
 * freed, and should persist for the lifetime of the object.
 * @return The Ruby instance of the image.
 */
VALUE RGSS_Image_New(int width, int height, unsigned char *pixels);

/**
 * @brief Creates a new Ruby Image instance from the specified image file.
 * @param[in] path The path to an image file.
 * @return The Ruby instance of the image.
 */
VALUE RGSS_Image_NewFromFile(const char *path);

/**
 * @brief Saves an image in PNG format to the specified filename.
 * @param[in] filename The path of the file where the image will be saved.
 * @param[in] width The width of the image, in pixels.
 * @param[in] height The height of the image, in pixels.
 * @param[in] pixels A pointer to a tightly packed RGBA pixel data, 4 bytes per pixel.
 * @return @c 0 on failure, otherwise a non-zero integer.
 */
int RGSS_Image_SavePNG(const char *filename, int width, int height, unsigned char *pixels);

/**
 * @brief Saves an image in JPG format to the specified filename.
 * @param[in] filename The path of the file where the image will be saved.
 * @param[in] width The width of the image, in pixels.
 * @param[in] height The height of the image, in pixels.
 * @param[in] pixels A pointer to a tightly packed RGBA pixel data, 4 bytes per pixel.
 * @param[in] quality A value between @c 0 and @c 100 indicating the quality level to save the image as.
 * @return @c 0 on failure, otherwise a non-zero integer.
 */
int RGSS_Image_SaveJPG(const char *filename, int width, int height, unsigned char *pixels, int quality);

/**
 * @brief Loads an image file.
 * @param[in] path The path to the image file.
 * @param[in,out] width The width of the image, in pixels;
 * @param[in,out] height The height of the image, in pixels;
 * @param[in,out] pixels A pointer to recieve the address of the images in pixels, a tightly packed array of RGBA data, 4 bytes per pixel.
 */
void RGSS_Image_Load(const char *path, int *width, int *height, unsigned char **pixels);

/**
 * @brief Strongly-typed names for logger severity levels.
 */
typedef enum
{
    RGSS_LOG_DEBUG,  /** Low-level information for developers. */
    RGSS_LOG_INFO,   /** Generic (useful) information about system operation. */
    RGSS_LOG_WARN,   /** A warning. */
    RGSS_LOG_ERROR,  /** A handleable error condition. */
    RGSS_LOG_FATAL,  /** An unhandleable error that results in a program crash. */
    RGSS_LOG_UNKNOWN /** An unknown message that should always be logged. */
} RGSS_LOG_LEVEL;

/**
 * @brief Adds a log entry.
 * @param[in] level The severity level of the message.
 * @param[in] format A format string for the log message.
 */
void RGSS_Log(RGSS_LOG_LEVEL level, const char *format, ...);

/**
 * @brief Logs a fatal error message.
 * @param[in] format A format string for the log message.
 */
#define RGSS_LogFatal(format, ...) RGSS_Log(RGSS_LOG_FATAL, format, ##__VA_ARGS__)

/**
 * @brief Logs an error message.
 * @param[in] format A format string for the log message.
 */
#define RGSS_LogError(format, ...) RGSS_Log(RGSS_LOG_ERROR, format, ##__VA_ARGS__)

/**
 * @brief Logs a warning message.
 * @param[in] format A format string for the log message.
 */
#define RGSS_LogWarn(format, ...)  RGSS_Log(RGSS_LOG_WARN, format, ##__VA_ARGS__)

/**
 * @brief Logs a general informational message.
 * @param[in] format A format string for the log message.
 */
#define RGSS_LogInfo(format, ...)  RGSS_Log(RGSS_LOG_INFO, format, ##__VA_ARGS__)

/**
 * @brief Logs a low-level debug message.
 * @param[in] format A format string for the log message.
 */
#define RGSS_LogDebug(format, ...) RGSS_Log(RGSS_LOG_DEBUG, format, ##__VA_ARGS__)

#endif /* RGSS_H */
