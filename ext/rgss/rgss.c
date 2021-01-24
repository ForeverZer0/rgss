#include "rgss.h"
#include <ruby/io.h>
#include <stdint.h>
#include <time.h>

VALUE rb_mRGSS;
VALUE rb_eRGSSError;
VALUE RGSS_LOGGER;

ID RGSS_ID_UPDATE_VERTICES;
ID RGSS_ID_BATCH;
ID RGSS_ID_RENDER;
ID RGSS_ID_UPDATE;
ID RGSS_ID_ADD;

struct xoshiro256ss_state
{
    uint64_t s[4];
};

struct xoshiro256ss_state RGSS_RAND_STATE;

static inline uint64_t rol64(uint64_t x, int k)
{
    return (x << k) | (x >> (64 - k));
}

uint64_t xoshiro256ss(struct xoshiro256ss_state *state)
{
    uint64_t *s = state->s;
    uint64_t const result = rol64(s[1] * 5, 7) * 9;
    uint64_t const t = s[1] << 17;

    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];

    s[2] ^= t;
    s[3] = rol64(s[3], 45);

    return result;
}

static inline double to_double(uint64_t x)
{
    const union {
        uint64_t i;
        double d;
    } u = {.i = UINT64_C(0x3FF) << 52 | x >> 12};
    return u.d - 1.0;
}

void RGSS_Log(RGSS_LOG_LEVEL level, const char *format, ...)
{
    if (!RTEST(RGSS_LOGGER))
        RGSS_LOGGER = rb_const_get(rb_mRGSS, rb_intern("Log"));

    va_list args;
    va_start(args, format);
    VALUE msg = rb_vsprintf(format, args);
    va_end(args);

    rb_funcall(RGSS_LOGGER, RGSS_ID_ADD, 2, INT2NUM(level), msg);
}

VALUE RGSS_Handle_Alloc(VALUE klass)
{
    return Data_Wrap_Struct(klass, NULL, RUBY_NEVER_FREE, NULL);
}

static VALUE RGSS_Radians(VALUE rgss, VALUE degrees)
{
    return DBL2NUM(NUM2DBL(degrees) * (M_PI / 180.0));
}

static VALUE RGSS_Degrees(VALUE rgss, VALUE radians)
{
    return DBL2NUM(NUM2DBL(radians) * (180.0 / M_PI));
}

inline float RGSS_Rand()
{
    uint64_t r = xoshiro256ss(&RGSS_RAND_STATE);
    return (float)to_double(r);
}

static VALUE RGSS_Random(int argc, VALUE *argv, VALUE rgss)
{

    if (argc == 0)
        return DBL2NUM(to_double(xoshiro256ss(&RGSS_RAND_STATE)));

    if (argc != 2)
        rb_raise(rb_eArgError, "wrong number of arguments (given %d, expected 0 or 2)", argc);

    int64_t lower = NUM2LL(argv[0]);
    int64_t upper = NUM2LL(argv[1]);
    if (lower > upper)
        rb_raise(rb_eArgError, "upper bound cannot be less than lower bound");

    int64_t n = (int64_t)((xoshiro256ss(&RGSS_RAND_STATE) % (upper - lower + 1)) + lower);
    return LL2NUM(n);
}

#define LOOKUPS_PER_DEGREE 10
#define NUM_LOOKUP_VALUES  (360 * LOOKUPS_PER_DEGREE)
#define LOOKUP_PRECISION   (1.0f / LOOKUPS_PER_DEGREE)
float RGSS_SIN_LOOKUP[NUM_LOOKUP_VALUES];

inline float RGSS_FastSin(float degrees)
{
    // Normalize to 0..360 (i.e. 0..2PI)
    degrees = fmod(degrees, 360.0f);
    if (degrees < 0.0f)
        degrees += 360.0f;

    int index = (int)(degrees * LOOKUPS_PER_DEGREE);
    return RGSS_SIN_LOOKUP[index % NUM_LOOKUP_VALUES];
}

char *RGSS_ReadFileDescriptorText(int fd)
{
    long len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    char *buffer = xmalloc(len + 1);
    if (buffer == NULL)
        rb_raise(rb_eNoMemError, "out of memory");

    if (read(fd, buffer, len) != len)
        rb_raise(rb_eIOError, "failed to read file");

    buffer[len] = '\0';
    return buffer;
}

char *RGSS_ReadFileText(const char *path)
{
    VALUE file = rb_file_open(path, "rb");
    rb_io_t *io;

    GetOpenFile(file, io);
    char *buffer = RGSS_ReadFileDescriptorText(io->fd);
    rb_io_close(file);

    return buffer;
}

void *RGSS_ReadFile(const char *path, long *bufsize)
{
    VALUE file = rb_file_open(path, "rb");
    rb_io_t *io;

    GetOpenFile(file, io);

    long len = lseek(io->fd, 0, SEEK_END);
    lseek(io->fd, 0, SEEK_SET);

    char *buffer = xmalloc(len);
    if (buffer == NULL)
        rb_raise(rb_eNoMemError, "out of memory");

    if (read(io->fd, buffer, len) != len)
        rb_raise(rb_eIOError, "failed to read file");

    if (bufsize)
        *bufsize = len;
    rb_io_close(file);
    return buffer;
}

char *RGSS_ReadFileTextRB(VALUE source)
{
    if (NIL_P(source))
        rb_raise(rb_eArgError, "file cannot be nil");

    if (FIXNUM_P(source))
        return RGSS_ReadFileDescriptorText(NUM2INT(source));

    VALUE file;
    int close;

    if (RB_TYPE_P(source, T_FILE))
    {
        file = source;
        close = false;
    }
    else if (RB_TYPE_P(source, T_STRING))
    {
        file = rb_file_open(StringValueCStr(source), "rb");
        close = true;
    }
    else
    {
        rb_raise(rb_eTypeError, "%s is not a String, File, or Integer", CLASS_NAME(source));
    }

    rb_io_t *io;
    GetOpenFile(source, io);
    char *buffer = RGSS_ReadFileDescriptorText(io->fd);
    if (close)
        rb_io_close(file);

    return buffer;
}

inline void RGSS_ParseRect(int argc, VALUE *argv, RGSS_Rect *rect)
{
    switch (argc)
    {
        case 1: {
            if (rb_obj_is_kind_of(argv[0], rb_cRect) != Qtrue)
                rb_raise(rb_eTypeError, "%s is not a Rect", CLASS_NAME(argv[0]));
            memcpy(rect, DATA_PTR(argv[0]), sizeof(RGSS_Rect));
            break;
        }
        case 2: {
            if (rb_obj_is_kind_of(argv[0], rb_cIVec2) != Qtrue)
                rb_raise(rb_eTypeError, "%s is not a IVec2", CLASS_NAME(argv[0]));
            if (rb_obj_is_kind_of(argv[1], rb_cIVec2) != Qtrue)
                rb_raise(rb_eTypeError, "%s is not a IVec2", CLASS_NAME(argv[1]));

            memcpy(&rect->location, DATA_PTR(argv[0]), sizeof(RGSS_Point));
            memcpy(&rect->size, DATA_PTR(argv[1]), sizeof(RGSS_Size));
            break;
        }
        case 4: {
            rect->x = NUM2INT(argv[0]);
            rect->y = NUM2INT(argv[1]);
            rect->width = NUM2INT(argv[2]);
            rect->height = NUM2INT(argv[3]);
            break;
        }
    }
}

void Init_rgss(void)
{
    // Initialize state for RNG
    // TODO: Dump/load seed
    time_t t;
    srand((unsigned)time(&t));
    RGSS_RAND_STATE.s[0] = (unsigned)rand();
    RGSS_RAND_STATE.s[1] = (unsigned)rand();
    RGSS_RAND_STATE.s[2] = (unsigned)rand();
    RGSS_RAND_STATE.s[3] = (unsigned)rand();

    rb_mRGSS = rb_define_module("RGSS");
    rb_eRGSSError = rb_define_class_under(rb_mRGSS, "RGSSError", rb_eStandardError);

    rb_define_module_function1(rb_mRGSS, "degrees", RGSS_Degrees, 1);
    rb_define_module_function1(rb_mRGSS, "radians", RGSS_Radians, 1);
    rb_define_module_functionm1(rb_mRGSS, "rand", RGSS_Random, -1);

    RGSS_Init_GL(rb_mRGSS);
    RGSS_Init_GLFW(rb_mRGSS);
    RGSS_Init_Game(rb_mRGSS);
    RGSS_Init_Graphics(rb_mRGSS);
    RGSS_Init_Input(rb_mRGSS);

    RGSS_Init_Batch(rb_mGraphics);
    RGSS_Init_Image(rb_mRGSS);
    RGSS_Init_Table(rb_mRGSS);
    RGSS_Init_ColorAndTone(rb_mRGSS);
    RGSS_Init_PointAndSize(rb_mRGSS);
    RGSS_Init_Rect(rb_mRGSS);
    RGSS_Init_Vectors(rb_mRGSS);
    RGSS_Init_Mat4(rb_mRGSS);
    RGSS_Init_Entity(rb_mRGSS);
    RGSS_Init_Texture(rb_mRGSS);
    RGSS_Init_Font(rb_mRGSS);
    RGSS_Init_Particles(rb_mRGSS);

    rb_define_const(rb_mRGSS, "SIZEOF_VOIDP", INT2NUM(SIZEOF_VOIDP));
    rb_define_const(rb_mRGSS, "SIZEOF_CHAR", INT2NUM(1));
    rb_define_const(rb_mRGSS, "SIZEOF_SHORT", INT2NUM(SIZEOF_SHORT));
    rb_define_const(rb_mRGSS, "SIZEOF_INT", INT2NUM(SIZEOF_INT));
    rb_define_const(rb_mRGSS, "SIZEOF_LONG", INT2NUM(SIZEOF_LONG));
    rb_define_const(rb_mRGSS, "SIZEOF_LONG_LONG", INT2NUM(SIZEOF_LONG_LONG));
    rb_define_const(rb_mRGSS, "SIZEOF_FLOAT", INT2NUM(SIZEOF_FLOAT));
    rb_define_const(rb_mRGSS, "SIZEOF_DOUBLE", INT2NUM(SIZEOF_DOUBLE));
    rb_define_const(rb_mRGSS, "SIZEOF_SIZE_T", INT2NUM(SIZEOF_SIZE_T));
    rb_define_const(rb_mRGSS, "SIZEOF_SSIZE_T", INT2NUM(SIZEOF_SSIZE_T));
    rb_define_const(rb_mRGSS, "SIZEOF_PTRDIFF_T", INT2NUM(SIZEOF_PTRDIFF_T));
    rb_define_const(rb_mRGSS, "SIZEOF_INTPTR_T", INT2NUM(SIZEOF_INTPTR_T));
    rb_define_const(rb_mRGSS, "SIZEOF_UINTPTR_T", INT2NUM(SIZEOF_UINTPTR_T));

    RGSS_ID_UPDATE_VERTICES = rb_intern("update_vertices");
    RGSS_ID_BATCH = rb_intern("batch");
    RGSS_ID_RENDER = rb_intern("render");
    RGSS_ID_UPDATE = rb_intern("update");
    RGSS_ID_ADD = rb_intern("add");

    for (int i = 0; i < NUM_LOOKUP_VALUES; i++)
    {
        float angle = (float)i / LOOKUPS_PER_DEGREE;
        RGSS_SIN_LOOKUP[i] = sinf(glm_rad(angle + LOOKUP_PRECISION));
    }

    // Ensure the cardinal directions are 100% accurate.
    for (int i = 0; i <= 270; i += 90)
    {
        RGSS_SIN_LOOKUP[i * LOOKUPS_PER_DEGREE] = sinf(glm_rad(i));
    }
}
