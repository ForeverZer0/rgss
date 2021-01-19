#include "game.h"
// #include <cairo/cairo.h>
#include <pango/pangoft2.h>
#include <pango/pangocairo.h>
#include FT_BITMAP_H

#include "glad.h"


#define RGSS_DEFAULT_FONT_SIZE 16



#define RGSS_IV_COLOR "@default_color"
#define RGSS_IV_SIZE "@default_size"

VALUE rb_cFont;









static unsigned int
create_texture (unsigned int width,
                unsigned int height,
                unsigned char *pixels)
{
    unsigned int texture_id;

    glGenTextures (1, &texture_id);
    glBindTexture (GL_TEXTURE_2D, texture_id);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D (GL_TEXTURE_2D,
                  0,
                  GL_RGBA,
                  width,
                  height,
                  0,
                  GL_BGRA,
                  GL_UNSIGNED_BYTE,
                  pixels);

    return texture_id;
}

static inline void get_text_size (PangoLayout *layout, unsigned int *width, unsigned int *height)
{
    pango_layout_get_size (layout, width, height);
    /* Divide by pango scale to get dimensions in pixels. */
    *width /= PANGO_SCALE;
    *height /= PANGO_SCALE;
}

static inline cairo_t *RGSS_Font_CreateCairo (int width, int height, int channels, cairo_surface_t** surf, unsigned char** buffer)
{
    *buffer = calloc (channels * width * height, sizeof (unsigned char));
    *surf = cairo_image_surface_create_for_data (*buffer, CAIRO_FORMAT_ARGB32, width, height, channels * width);
    return cairo_create(*surf);
}

static inline cairo_t *RGSS_Font_CreateContext(void)
{
    cairo_surface_t *temp_surface;
    cairo_t *context;

    temp_surface = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 0, 0);
    context = cairo_create (temp_surface);
    cairo_surface_destroy (temp_surface);

    return context;
}


static unsigned int RGSS_Font_CreateTexture (const char *text,
            const char *FONT,
             unsigned int *text_width,
             unsigned int *text_height,
             unsigned int *texture_id)
{
    cairo_t *layout_context;
    cairo_t *render_context;
    cairo_surface_t *temp_surface;
    cairo_surface_t *surface;
    unsigned char* surface_data = NULL;

    PangoFontDescription *desc;
    PangoLayout *layout;

    layout_context = RGSS_Font_CreateContext ();

    /* Create a PangoLayout, set the font and text */
    layout = pango_cairo_create_layout (layout_context);


    pango_layout_set_markup(layout, text, -1);
    // pango_layout_set_text (layout, text, -1);

    /* Load the font */
    desc = pango_font_description_from_string (FONT);
    pango_layout_set_font_description (layout, desc);
    pango_font_description_free (desc);

    /* Get text dimensions and create a context to render to */
    get_text_size (layout, text_width, text_height);
    render_context = RGSS_Font_CreateCairo (*text_width,
                                           *text_height,
                                           4,
                                           &surface,
                                           &surface_data);

    /* Render */
    cairo_set_source_rgba (render_context, 1, 1, 1, 1);
    pango_cairo_show_layout (render_context, layout);
    *texture_id = create_texture(*text_width, *text_height, surface_data);

    /* Clean up */
    free (surface_data);
    g_object_unref (layout);
    cairo_destroy (layout_context);
    cairo_destroy (render_context);
    cairo_surface_destroy (surface);
}





static VALUE RGSS_Font_TestTexture(VALUE klass, VALUE str, VALUE fontname, VALUE dimen)
{
    char *text = StringValueCStr(str);
    char *font = StringValueCStr(fontname);
    RGSS_Size *size = DATA_PTR(dimen);

    unsigned int w, h, id;

    RGSS_Font_CreateTexture(text, font, &w, &h, &id);

    return rb_ary_new_from_args(3, UINT2NUM(id), UINT2NUM(w), UINT2NUM(h));
}












typedef struct {

    PangoContext *context;
    PangoLayout *layout;
    PangoFontDescription *desc;
    PangoFontMap *map;

    double size;
    RGSS_Color color;
} RGSS_Font;

static void RGSS_Font_Free(void *data)
{
    RGSS_Font *font = data;

    if (font->desc)
        pango_font_description_free(font->desc);
    if (font->map)
        g_object_unref(font->map);
    if (font->layout)
        g_object_unref(font->layout);
    if (font->context)
        g_object_unref(font->context);
    
    xfree(data);
}

static VALUE RGSS_Font_GetDefaultColor(VALUE klass)
{
    VALUE color = rb_iv_get(klass, RGSS_IV_COLOR);
    if (rb_obj_is_kind_of(color, rb_cColor) != Qtrue)
    {
        color = RGSS_Color_New(rb_cColor, 1.0f, 1.0f, 1.0f, 1.0f);
        rb_iv_set(klass, RGSS_IV_COLOR, color);
    }
    return color;
}

static VALUE RGSS_Font_SetDefaultColor(VALUE klass, VALUE color)
{
    if (rb_obj_is_kind_of(color, rb_cColor) != Qtrue)
        color = RGSS_Color_New(rb_cColor, 1.0f, 1.0f, 1.0f, 1.0f);

    rb_iv_set(klass, RGSS_IV_COLOR, color);
    return color;
}

static VALUE RGSS_Font_GetDefaultSize(VALUE klass)
{
    VALUE size = rb_iv_get(klass, RGSS_IV_SIZE);
    if (!FIXNUM_P(size))
    {
        size = INT2NUM(RGSS_DEFAULT_FONT_SIZE);
        rb_iv_set(klass, RGSS_IV_SIZE, size);
    }
    return size;
}

static VALUE RGSS_Font_SetDefaultSize(VALUE klass, VALUE size)
{
    if (!FIXNUM_P(size))
        size = INT2NUM(RGSS_DEFAULT_FONT_SIZE);
    
    rb_iv_set(klass, RGSS_IV_SIZE, size);
    return size;
}

static VALUE RGSS_Font_Alloc(VALUE klass)
{
    RGSS_Font *font = ALLOC(RGSS_Font);
    memset(font, 0, sizeof(RGSS_Font));
    return Data_Wrap_Struct(klass, NULL, RGSS_Font_Free, font);
}

static VALUE RGSS_Font_GetSize(VALUE self)
{
    RGSS_Font *font = DATA_PTR(self);
    return INT2NUM(font->size);
}

static VALUE RGSS_Font_SetSize(VALUE self, VALUE size)
{
    RGSS_Font *font = DATA_PTR(self);
    font->size = NUM2INT(size);
    pango_font_description_set_size(font->desc, font->size);
    return size;
}

static VALUE RGSS_Font_GetColor(VALUE self)
{
    RGSS_Font *font = DATA_PTR(self);
    float* color = RGSS_VEC4_NEW;
    glm_vec4_copy(font->color, color);
    return Data_Wrap_Struct(rb_cColor, NULL, free, color);
}

static VALUE RGSS_Font_SetColor(VALUE self, VALUE color)
{
    RGSS_Font *font = DATA_PTR(self);
    if (!RTEST(color))
        color = RGSS_Font_GetDefaultColor(rb_cFont);
    
    glm_vec4_copy(DATA_PTR(color), font->color);
    return color;
}

static VALUE RGSS_Font_Measure(int argc, VALUE *argv, VALUE self)
{   
    VALUE text, fit_x, fit_y, opts;
    rb_scan_args(argc, argv, "12:", &text, &fit_x, &fit_y, &opts);
    if (text == Qnil)
        return RGSS_Size_New(0, 0);

    RGSS_Font *font = DATA_PTR(self);
    int align, plain, w, h;
    RGSS_ParseOpt(opts, "align", PANGO_ALIGN_LEFT, &align);
    RGSS_ParseOpt(opts, "plain", false, &plain);
    w = RTEST(fit_x) ? NUM2INT(fit_x) : INT_MAX;
    h = RTEST(fit_y) ? NUM2INT(fit_y) : INT_MAX;

    pango_layout_set_width(font->layout, PANGO_SCALE * w);
    pango_layout_set_height(font->layout, PANGO_SCALE * h);
    pango_layout_set_alignment(font->layout, align);

    char *str = StringValueCStr(text);
    if (plain)
        pango_layout_set_text(font->layout, str, -1);
    else
        pango_layout_set_markup(font->layout, str, -1);

    RGSS_Size *size = ALLOC(RGSS_Size);
    pango_layout_get_pixel_size(font->layout, &size->width, &size->height);
    return Data_Wrap_Struct(rb_cSize, NULL, RUBY_DEFAULT_FREE, size);
}

static VALUE RGSS_Font_Bake(int argc, VALUE *argv, VALUE self)
{
    VALUE text, bounds, opts;
    rb_scan_args(argc, argv, "2:", &text, &bounds, &opts);
    RGSS_Font *font = DATA_PTR(self);
    RGSS_Size *size = DATA_PTR(bounds);
    char *markup = StringValueCStr(text);

    if (size->width < 1)
        rb_raise(rb_eArgError, "width must be greater than 0");
    if (size->height < 1)
        rb_raise(rb_eArgError, "height must be greater than 0");

    int align;
    int plain;
    RGSS_ParseOpt(opts, "align", PANGO_ALIGN_LEFT, &align);
    RGSS_ParseOpt(opts, "plain", false, &plain);

    pango_layout_set_width(font->layout, PANGO_SCALE * size->width);
    pango_layout_set_height(font->layout, PANGO_SCALE * size->height);
    pango_layout_set_alignment(font->layout, align);

    FT_Bitmap bmp;
    FT_Bitmap_New(&bmp);

    long bufsize = size->width * size->height;
    bmp.width = size->width;
    bmp.rows = size->height;
    bmp.pixel_mode = FT_PIXEL_MODE_GRAY;
    bmp.num_grays = 256;
    bmp.buffer = xmalloc(bufsize);
    bmp.pitch = size->width;
    memset(bmp.buffer, 0, bufsize);

    if (plain)
        pango_layout_set_text(font->layout, markup, -1);
    else
        pango_layout_set_markup(font->layout, markup, -1);

    pango_ft2_render_layout(&bmp, font->layout, 0, 0); // TODO: Adjust coords for justification

    return rb_str_new(bmp.buffer, bufsize);
}

static VALUE RGSS_Font_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE name, pixel_size, opts;
    rb_scan_args(argc, argv, "11:", &name, &pixel_size, &opts);
    RGSS_Font *font = DATA_PTR(self);

    if (NIL_P(name))
        rb_raise(rb_eArgError, "family name cannot be nil");

    char *family = StringValueCStr(name);
    int size = RTEST(pixel_size) ? NUM2INT(pixel_size) : RGSS_DEFAULT_FONT_SIZE;

    char buffer[256];
    if (sprintf(buffer, "%s %d", family, size) < 0)
        rb_raise(rb_eArgError, "invalid font description");

    font->map = pango_ft2_font_map_new();
    if (font->map == NULL)
        rb_raise(rb_eRuntimeError, "failed to create font map");
    
    font->context = pango_font_map_create_context(font->map);
    if (font->context == NULL)
        rb_raise(rb_eRuntimeError, "failed to create context for font map");

    font->layout = pango_layout_new(font->context);
    if (font->layout == NULL)
        rb_raise(rb_eRuntimeError, "failed to create layout for font context");

    font->desc = pango_font_description_from_string(buffer);
    if (font->desc == NULL)
        rb_raise(rb_eRuntimeError, "failed to create font from description");

    pango_layout_set_font_description(font->layout, font->desc);
    pango_font_map_load_font(font->map, font->context, font->desc);

    return self;
}

static VALUE RGSS_Font_Add(VALUE klass, VALUE path)
{
    char *file = StringValueCStr(path);
    FcBool fontAddStatus = FcConfigAppFontAddFile(FcConfigGetCurrent(), file);
    return RB_BOOL(fontAddStatus);
}

void RGSS_Init_Font(VALUE parent)
{
    rb_cFont = rb_define_class_under(parent, "Font", rb_cObject);
    rb_define_alloc_func(rb_cFont, RGSS_Font_Alloc);

    rb_define_methodm1(rb_cFont, "initialize", RGSS_Font_Initialize, -1);
    rb_define_methodm1(rb_cFont, "bake", RGSS_Font_Bake, -1);
    rb_define_methodm1(rb_cFont, "measure", RGSS_Font_Measure, -1);

    rb_define_method0(rb_cFont, "size", RGSS_Font_GetSize, 0);
    rb_define_method1(rb_cFont, "size=", RGSS_Font_SetSize, 1);
    rb_define_method0(rb_cFont, "color", RGSS_Font_GetColor, 0);
    rb_define_method1(rb_cFont, "color=", RGSS_Font_SetColor, 1);

    rb_define_singleton_method0(rb_cFont, "default_color", RGSS_Font_GetDefaultColor, 0);
    rb_define_singleton_method1(rb_cFont, "default_color=", RGSS_Font_SetDefaultColor, 1);
    rb_define_singleton_method0(rb_cFont, "default_size", RGSS_Font_GetDefaultSize, 0);
    rb_define_singleton_method1(rb_cFont, "default_size=", RGSS_Font_SetDefaultSize, 1);

    rb_define_singleton_method1(rb_cFont, "add_file", RGSS_Font_Add, 1);

    rb_define_singleton_method3(rb_cFont, "test_texture", RGSS_Font_TestTexture, 3);

    rb_define_const(rb_cFont, "ALIGN_LEFT", INT2NUM(PANGO_ALIGN_LEFT));
    rb_define_const(rb_cFont, "ALIGN_CENTER", INT2NUM(PANGO_ALIGN_CENTER));
    rb_define_const(rb_cFont, "ALIGN_RIGHT", INT2NUM(PANGO_ALIGN_RIGHT));
    rb_define_const(rb_cFont, "ALIGN_TOP", INT2NUM(PANGO_ALIGN_LEFT));
    rb_define_const(rb_cFont, "ALIGN_BOTTOM", INT2NUM(PANGO_ALIGN_RIGHT));
}