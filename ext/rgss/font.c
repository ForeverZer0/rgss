#if 1

#include "game.h"
#include "ruby/io.h"

#define STB_TRUETYPE_IMPLEMENTATION 1
#define STBTT_STATIC 1
#define STBTT_malloc(x, u) ((void)(u),xmalloc(x))
#define STBTT_free(x,u)    ((void)(u),xfree(x))
#define STBTT_assert(x)    RUBY_ASSERT(x)

#include "stb_rect_pack.h" 
#include "stb_truetype.h"

VALUE rb_cTrueType;

typedef struct {
    stbtt_fontinfo info;
    unsigned char *buffer;
} RGSS_TrueType;

static void RGSS_TrueType_Free(void *data)
{
    if (data == NULL)
        return;

    RGSS_TrueType *font = data;
    if (font->buffer)
        xfree(font->buffer);

    xfree(font);
}

static VALUE RGSS_TrueType_Alloc(VALUE klass)
{
    RGSS_TrueType *font = ALLOC(RGSS_TrueType);
    memset(font, 0, sizeof(RGSS_TrueType));
    return Data_Wrap_Struct(klass, NULL, RGSS_TrueType_Free, font);
}

static unsigned char* RGSS_TrueType_ReadFile(int fd)
{
    if (fd == -1)
        rb_raise(rb_eRuntimeError, "invalid file descriptor");

    size_t len, result;

    len = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    void *buffer = xmalloc(len);
    if (buffer == NULL)
        rb_raise(rb_eNoMemError, "failed to allocate file read buffer");

    result = read(fd, buffer, len);
    if (result != len)
        rb_raise(rb_eRuntimeError, "failed to read file");

    return buffer;
}

static VALUE RGSS_TrueType_Initialize(VALUE self, VALUE source)
{
    rb_io_t *fptr;
    VALUE file;
    int close;

    if (RB_TYPE_P(source, T_STRING))
    {
        file = rb_file_open(StringValueCStr(source), "rb");
        close = 1;
    }
    else if (RB_TYPE_P(source, T_FILE))
    {
        file = source;
        close = 0;
    }
    else
    {
        rb_raise(rb_eTypeError, "%s is not a File or String", CLASS_NAME(source));
    }

    GetOpenFile(file, fptr);
    unsigned char *buffer = RGSS_TrueType_ReadFile(fptr->fd); 
    if (close)
        rb_io_close(file);
    
    RGSS_TrueType *font = DATA_PTR(self);

    font->buffer = buffer;
    stbtt_InitFont(&font->info, buffer, stbtt_GetFontOffsetForIndex(buffer, 0));

    return self;
}

static VALUE RGSS_TrueType_Scale(int argc, VALUE *argv, VALUE self)
{
    VALUE height, opts;
    rb_scan_args(argc, argv, "1:", &height, &opts);

    int pixel = 1;
    if (RTEST(opts))
    {
        VALUE mode = rb_hash_aref(opts, STR2SYM("mode"));
        if (mode == STR2SYM("pixel"))
            pixel = 1;
        else if (mode == STR2SYM("em"))
            pixel = 0;
        else 
            rb_raise(rb_eArgError, "invalid scaling mode specified");
    }

    float value;
    RGSS_TrueType *font = DATA_PTR(self);

    if (pixel)
        value = stbtt_ScaleForPixelHeight(&font->info, NUM2FLT(height));
    else
        value = stbtt_ScaleForMappingEmToPixels(&font->info, NUM2FLT(height));

    return DBL2NUM(value);
}


void RGSS_Init_TrueType(VALUE parent)
{
    rb_cTrueType = rb_define_class_under(parent, "TrueType", rb_cObject);
    rb_define_alloc_func(rb_cTrueType, RGSS_TrueType_Alloc);


    rb_define_method1(rb_cTrueType, "initialize", RGSS_TrueType_Initialize, 1);
    rb_define_methodm1(rb_cTrueType, "scale", RGSS_TrueType_Scale, -1);
}

#endif /*  */