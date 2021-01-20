#include "rgss.h"
#include "ruby/io.h"

VALUE rb_mRGSS;
VALUE rb_eRGSSError;
VALUE RGSS_LOGGER;

ID RGSS_ID_UPDATE_VERTICES;
ID RGSS_ID_BATCH;
ID RGSS_ID_RENDER;
ID RGSS_ID_UPDATE;
ID RGSS_ID_SEND;
ID RGSS_ID_ADD;


void RGSS_Log(RGSS_LOG_LEVEL level, const char *format, ...)
{
    if (!RTEST(RGSS_LOGGER))
        RGSS_LOGGER = rb_const_get(rb_mRGSS, rb_intern("Log"));

    va_list args;
    va_start(args, format);
    VALUE msg = rb_sprintf(format, args);
    va_end(args);

    rb_funcall(RGSS_LOGGER, RGSS_ID_ADD, 2, INT2NUM(level), msg);    
}

VALUE RGSS_Handle_Alloc(VALUE klass)
{
    return Data_Wrap_Struct(klass, NULL, RUBY_NEVER_FREE, NULL);
}

static VALUE RGSS_Radians(VALUE math, VALUE degrees)
{
    return DBL2NUM(NUM2DBL(degrees) * (M_PI / 180.0));
}

static VALUE RGSS_Degrees(VALUE math, VALUE radians)
{
    return DBL2NUM(NUM2DBL(radians) * (180.0 / M_PI));
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
        rb_raise(rb_eTypeError, "file is not a String, File, or Integer");
    }

    rb_io_t *io;
    GetOpenFile(source, io);
    char *buffer = RGSS_ReadFileDescriptorText(io->fd);
    if (close)
        rb_io_close(file);

    return buffer;
}

void Init_rgss(void)
{
    rb_mRGSS = rb_define_module("RGSS");
    rb_eRGSSError = rb_define_class_under(rb_mRGSS, "RGSSError", rb_eStandardError);

    rb_define_module_function1(rb_mRGSS, "degrees", RGSS_Degrees, 1);
    rb_define_module_function1(rb_mRGSS, "radians", RGSS_Radians, 1);

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
    RGSS_ID_SEND = rb_intern("send");
    RGSS_ID_UPDATE = rb_intern("update");
    RGSS_ID_ADD = rb_intern("add");
}
