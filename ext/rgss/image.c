#include "graphics.h"

VALUE rb_cImage;

#define STBI_NO_PSD 1
#define STBI_NO_HDR 1
#define STBI_NO_PIC 1
#define STBI_NO_PNM 1

// stb_image.h macros
#define STBI_ASSERT RUBY_ASSERT
#define STBI_FREE xfree
#define STBI_MALLOC xmalloc
#define STBI_REALLOC xrealloc
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// stb_image_write.h macros
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_ASSERT RUBY_ASSERT
#define STBIW_MALLOC xmalloc
#define STBIW_REALLOC xrealloc
#define STBIW_FREE xfree
#include "stb_image_write.h"

#define JPEG_QUALITY 95
#define BYTES_PER_PIXEL 4
#define COMPONENT_COUNT 4

static void RGSS_Image_Free(void *img)
{
    if (img)
    {
        RGSS_Image *image = img;
        if (image->pixels)
            stbi_image_free(image->pixels);
        xfree(image);
    }
}

int RGSS_Image_SavePNG(const char *filename, int width, int height, unsigned char *pixels)
{
    RUBY_ASSERT_MESG(filename != NULL, "invalid filename specified");
    RGSS_SizeNotEmpty(width, height);
    return stbi_write_png(filename, width, height, COMPONENT_COUNT, pixels, width * COMPONENT_COUNT);
}

int RGSS_Image_SaveJPG(const char *filename, int width, int height, unsigned char *pixels, int quality)
{
    RUBY_ASSERT_MESG(filename != NULL, "invalid filename specified");
    RGSS_SizeNotEmpty(width, height);
    quality = RGSS_MAX(0, RGSS_MIN(100, quality));
    return stbi_write_jpg(filename, width, height, COMPONENT_COUNT, pixels, quality);
}

VALUE RGSS_Image_New(int width, int height, unsigned char *pixels)
{
    RGSS_SizeNotEmpty(width, height);
    if (pixels == NULL)
        rb_raise(rb_eArgError, "pixel data cannot be nil");

    RGSS_Image *img = ALLOC(RGSS_Image);
    img->width = width;
    img->height = height;
    img->pixels = pixels;
    return Data_Wrap_Struct(rb_cImage, NULL, RGSS_Image_Free, img);
}

VALUE RGSS_Image_NewFromFile(const char *path)
{
    RGSS_Image *img = ALLOC(RGSS_Image);
    RGSS_Image_Load(path, &img->width, &img->height, &img->pixels);
    return Data_Wrap_Struct(rb_cImage, NULL, RGSS_Image_Free, img);
}

void RGSS_Image_Load(const char *path, int *width, int *height, unsigned char **pixels)
{
    // TODO: Confirm path

    *pixels = stbi_load(path, width, height, NULL, COMPONENT_COUNT);
    if (pixels == NULL)
        rb_raise(rb_eRGSSError, "failed to load image");
}

static VALUE RGSS_Image_Alloc(VALUE klass)
{
    RGSS_Image *image = ALLOC(RGSS_Image);
    memset(image, 0, sizeof(RGSS_Image));
    return Data_Wrap_Struct(klass, NULL, RGSS_Image_Free, image);
}

static VALUE RGSS_Image_GetWidth(VALUE self)
{
    RGSS_Image *image = DATA_PTR(self);
    return INT2NUM(image->width);
}

static VALUE RGSS_Image_GetHeight(VALUE self)
{
    RGSS_Image *image = DATA_PTR(self);
    return INT2NUM(image->height);
}

static VALUE RGSS_Image_GetPixels(VALUE self)
{
    RGSS_Image *image = DATA_PTR(self);
    if (image->pixels)
    {
        size_t size = image->width * image->height * BYTES_PER_PIXEL;
        return rb_str_new((char*) image->pixels, size);
    }
    return Qnil;
}

static VALUE RGSS_Image_Dispose(VALUE self)
{
    RGSS_Image *image = DATA_PTR(self);
    if (image->pixels)
    {
        xfree(image->pixels);
        image->pixels = NULL;
    }
    return Qnil;
}

static VALUE RGSS_Image_IsDisposed(VALUE self)
{
    RGSS_Image *image = DATA_PTR(self);
    return RB_BOOL(image->pixels == NULL);
}

static VALUE RGSS_Image_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE x, y, blob;
    rb_scan_args(argc, argv, "12", &x, &y, &blob);

    RGSS_Image *image = DATA_PTR(self);

    switch (argc)
    {
        case 1:
        {
            RGSS_Image_Load(StringValueCStr(x), &image->width, &image->height, &image->pixels);
            break;
        }
        case 2:
        case 3:
        {
            image->width = NUM2INT(x);
            image->height = NUM2INT(y);
            RGSS_SizeNotEmpty(image->width, image->height);

            size_t size = image->width * image->height * BYTES_PER_PIXEL;
            image->pixels = xmalloc(size);
            if (image->pixels == NULL)
                rb_raise(rb_eNoMemError, "out of memory");

            if (RTEST(blob))
            {
                long len = RSTRING_LEN(blob);
                if (len != size)
                    rb_raise(rb_eArgError, "invalid blob length (given %d bytes, expected %u bytes)", len, size);
                memcpy(image->pixels, StringValuePtr(blob), size);
            }
            else
            {
                memset(image->pixels, 0, size);
            }
            
            break;
        }
    }


    return self;
}

static VALUE RGSS_Image_GetAddress(VALUE self)
{
    RGSS_Image *image = DATA_PTR(self);
    return PTR2NUM(image->pixels);
}



static VALUE RGSS_Image_Save(int argc, VALUE *argv, VALUE self)
{
    VALUE path, format, opts;
    rb_scan_args(argc, argv, "11:", &path, &format, &opts);

    RGSS_Image *image = DATA_PTR(self);
    if (image->pixels == NULL)
        rb_raise(rb_eRuntimeError, "disposed image");

    VALUE type = RTEST(format) ? format : STR2SYM("png");

    int result;
    int flip = RTEST(opts) && RTEST(rb_hash_aref(opts, STR2SYM("flip")));
    stbi_flip_vertically_on_write(flip);
    const char *file = StringValueCStr(path);

    if (type == STR2SYM("png"))
    {
        stbi_write_png_compression_level = 8;
        if (RTEST(opts))
        {
            VALUE cmp = rb_hash_aref(opts, STR2SYM("compression"));
            if (RTEST(cmp))
                stbi_write_png_compression_level = RGSS_MAX(0, RGSS_MIN(9, NUM2INT(cmp)));
        }

        int stride = image->width * BYTES_PER_PIXEL;
        result = stbi_write_png(file, image->width, image->height, COMPONENT_COUNT, image->pixels, stride);
    }
    else if (type == STR2SYM("jpg") || type == STR2SYM("jpeg"))
    {
        int quality = JPEG_QUALITY;
        if (RTEST(opts))
        {
            VALUE q = rb_hash_aref(opts, STR2SYM("quality"));
            if (RTEST(q))
                quality = RGSS_MAX(0, RGSS_MIN(100, NUM2INT(q)));
        }

        result = stbi_write_jpg(file, image->width, image->height, COMPONENT_COUNT, image->pixels, quality);
    }
    else if (type == STR2SYM("bmp"))
    {
        result = stbi_write_bmp(file, image->width, image->height, COMPONENT_COUNT, image->pixels);
    }
    else
    {
        rb_raise(rb_eArgError, "invalid image format specified");
    }

    if (!result)
        rb_raise(rb_eRuntimeError, "failed to save image");

    return self;
}

void RGSS_Init_Image(VALUE parent)
{
    rb_cImage = rb_define_class_under(parent, "Image", rb_cObject);
    rb_define_alloc_func(rb_cImage, RGSS_Image_Alloc);

    rb_define_methodm1(rb_cImage, "initialize", RGSS_Image_Initialize, -1);
    rb_define_method0(rb_cImage, "dispose", RGSS_Image_Dispose, 0);
    rb_define_method0(rb_cImage, "disposed?", RGSS_Image_IsDisposed, 0);

    rb_define_method0(rb_cImage, "width", RGSS_Image_GetWidth, 0);
    rb_define_method0(rb_cImage, "height", RGSS_Image_GetHeight, 0);
    rb_define_method0(rb_cImage, "pixels", RGSS_Image_GetPixels, 0);
    rb_define_method0(rb_cImage, "address", RGSS_Image_GetAddress, 0);
    rb_define_methodm1(rb_cImage, "save", RGSS_Image_Save, -1);

    rb_define_alias(rb_cImage, "columns", "width");
    rb_define_alias(rb_cImage, "rows", "height");
    rb_define_alias(rb_cImage, "blob", "pixels");
}