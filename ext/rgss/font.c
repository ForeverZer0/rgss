#include "game.h"
// #include <cairo/cairo.h>
#include <pango/pangocairo.h>
#include "pango/pangofc-font.h"
#include "glad.h"

#define RGSS_DEFAULT_FONT_SIZE 16

#define RGSS_IV_COLOR "@default_color"
#define RGSS_IV_SIZE  "@default_size"


typedef struct {
    PangoFontDescription *desc;
    PangoLayout *layout;
    cairo_t *context;
    RGSS_Color color;
} RGSS_NewFont;

VALUE rb_cFont;

static unsigned int create_texture(unsigned int width, unsigned int height, unsigned char *pixels)
{
    unsigned int texture_id;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

    return texture_id;
}

static inline void get_text_size(PangoLayout *layout, unsigned int *width, unsigned int *height)
{
    pango_layout_get_size(layout, width, height);
    /* Divide by pango scale to get dimensions in pixels. */
    *width /= PANGO_SCALE;
    *height /= PANGO_SCALE;
}

static inline cairo_t *RGSS_Font_CreateCairo(int width, int height, int channels, cairo_surface_t **surf,
                                             unsigned char **buffer)
{
    *buffer = calloc(channels * width * height, sizeof(unsigned char));
    *surf = cairo_image_surface_create_for_data(*buffer, CAIRO_FORMAT_ARGB32, width, height, channels * width);
    return cairo_create(*surf);
}

static unsigned int RGSS_Font_CreateTexture(const char *text, RGSS_NewFont *font, unsigned int *text_width,
                                            unsigned int *text_height, unsigned int *texture_id)
{
    cairo_t *render_context;
    cairo_surface_t *surface;
    unsigned char *surface_data = NULL;

    pango_layout_set_markup(font->layout, text, -1);
    // pango_layout_set_text (layout, text, -1);


    /* Get text dimensions and create a context to render to */
    get_text_size(font->layout, text_width, text_height);
    render_context = RGSS_Font_CreateCairo(*text_width, *text_height, 4, &surface, &surface_data);

    /* Render */
    cairo_set_source_rgba(render_context, 1, 1, 1, 1);
    pango_cairo_show_layout(render_context, font->layout);
    *texture_id = create_texture(*text_width, *text_height, surface_data);

    /* Clean up */
    free(surface_data);
    cairo_destroy(render_context);
    cairo_surface_destroy(surface);
}

#if 0
static VALUE RGSS_Font_TestTexture(VALUE klass, VALUE str, VALUE fontname, VALUE dimen)
{
    char *text = StringValueCStr(str);
    char *font = StringValueCStr(fontname);
    RGSS_Size *size = DATA_PTR(dimen);

    unsigned int w, h, id;


    RGSS_Font_CreateTexture(text, NULL, &w, &h, &id);

    return rb_ary_new_from_args(3, UINT2NUM(id), UINT2NUM(w), UINT2NUM(h));
}
#endif


static VALUE RGSS_Font_ToString(VALUE self)
{
    RGSS_NewFont *font = DATA_PTR(self);
    char *str = pango_font_description_to_string(font->desc);
    if (str == NULL)
        return Qnil;

    VALUE value = rb_str_new_cstr(str);
    g_free(str);
    return value;
}

static VALUE RGSS_Font_Measure(int argc, VALUE *argv, VALUE self)
{
    VALUE text, width, height, opts;
    rb_scan_args(argc, argv, "12:", &text, &width, &height, &opts);

    int w = RTEST(width) ? NUM2INT(width) : -1;
    int h = RTEST(height) ? NUM2INT(height) : -1;

    int plain, align;
    RGSS_ParseOpt(opts, "plain", false, &plain);
    RGSS_ParseOpt(opts, "align", PANGO_ALIGN_LEFT, &align);

    RGSS_NewFont *font = DATA_PTR(self);
    pango_layout_set_width(font->layout, w * PANGO_SCALE);
    pango_layout_set_height(font->layout, h * PANGO_SCALE);
    pango_layout_set_alignment(font->layout, align);
    
    text = StringValue(text);
    char *str = StringValuePtr(text);
    int length = RSTRING_LEN(text);

    if (plain)
        pango_layout_set_text(font->layout, str, length);
    else
        pango_layout_set_markup(font->layout, str, length);

    RGSS_Size *size = ALLOC(RGSS_Size);
    pango_layout_get_pixel_size(font->layout, &size->width, &size->height);
    return Data_Wrap_Struct(rb_cSize, NULL, RUBY_DEFAULT_FREE, size);
}


static VALUE RGSS_Font_Add(VALUE klass, VALUE path)
{
    char *file = StringValueCStr(path);
    FcBool status = FcConfigAppFontAddFile(FcConfigGetCurrent(), file);
    return RB_BOOL(status);
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

static VALUE RGSS_Font_GetColor(VALUE self)
{
    RGSS_NewFont *font = DATA_PTR(self);
    float *color = RGSS_VEC4_NEW;
    glm_vec4_copy(font->color, color);
    return Data_Wrap_Struct(rb_cColor, NULL, free, color);
}

static VALUE RGSS_Font_SetColor(VALUE self, VALUE color)
{
    RGSS_NewFont *font = DATA_PTR(self);
    if (NIL_P(color))
        color = RGSS_Font_GetDefaultColor(rb_cFont);

    glm_vec4_copy(DATA_PTR(color), font->color);
    return color;
}

static VALUE RGSS_Font_GetSize(VALUE self)
{
    RGSS_NewFont *font = DATA_PTR(self);
    gint size = pango_font_description_get_size(font->desc);
    return INT2NUM(size / PANGO_SCALE);
}

static VALUE RGSS_Font_SetSize(VALUE self, VALUE size)
{
    RGSS_NewFont *font = DATA_PTR(self);
    if (NIL_P(size))
        size = RGSS_Font_GetDefaultSize(rb_cFont);
    pango_font_description_set_size(font->desc, NUM2INT(size) * PANGO_SCALE);
    return size;
}

static VALUE RGSS_Font_GetFamily(VALUE self)
{
    RGSS_NewFont *font = DATA_PTR(self);
    const char *family = pango_font_description_get_family(font->desc);
    return family ? rb_str_new_cstr(family) : Qnil;
}

static VALUE RGSS_Font_GetWeight(VALUE self)
{
    RGSS_NewFont *font = DATA_PTR(self);
    PangoWeight value = pango_font_description_get_weight(font->desc);
    return INT2NUM(value);
}

static VALUE RGSS_Font_SetWeight(VALUE self, VALUE value)
{
    RGSS_NewFont *font = DATA_PTR(self);
    pango_font_description_set_weight(font->desc, NUM2INT(value));
    return value;
}

static VALUE RGSS_Font_GetStyle(VALUE self)
{
    RGSS_NewFont *font = DATA_PTR(self);
    PangoStyle value = pango_font_description_get_style(font->desc);
    return INT2NUM(value);
}

static VALUE RGSS_Font_SetStyle(VALUE self, VALUE value)
{
    RGSS_NewFont *font = DATA_PTR(self);
    pango_font_description_set_style(font->desc, NUM2INT(value));
    return value;
}

static VALUE RGSS_Font_GetGravity(VALUE self)
{
    RGSS_NewFont *font = DATA_PTR(self);
    PangoGravity value = pango_font_description_get_gravity(font->desc);
    return INT2NUM(value);
}

static VALUE RGSS_Font_SetGravity(VALUE self, VALUE value)
{
    RGSS_NewFont *font = DATA_PTR(self);
    pango_font_description_set_gravity(font->desc, NUM2INT(value));
    return value;
}

static VALUE RGSS_Font_GetStretch(VALUE self)
{
    RGSS_NewFont *font = DATA_PTR(self);
    PangoStretch value = pango_font_description_get_stretch(font->desc);
    return INT2NUM(value);
}

static VALUE RGSS_Font_SetStretch(VALUE self, VALUE value)
{
    RGSS_NewFont *font = DATA_PTR(self);
    pango_font_description_set_stretch(font->desc, NUM2INT(value));
    return value;
}

static void RGSS_Font_Free(void *data)
{
    RGSS_NewFont *font = data;
    if (font->context)
        cairo_destroy(font->context);
    if (font->layout)
        g_object_unref(font->layout);
    if (font->desc)
        pango_font_description_free(font->desc);
    xfree(data);
}

static VALUE RGSS_Font_Alloc(VALUE klass)
{
    RGSS_NewFont *font = ALLOC(RGSS_NewFont);
    memset(font, 0, sizeof(RGSS_NewFont));
    return Data_Wrap_Struct(klass, NULL, RGSS_Font_Free, font);
}

static VALUE RGSS_Font_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE description, opts;
    rb_scan_args(argc, argv, "1:", &description, &opts);
    if (NIL_P(description))
        rb_raise(rb_eArgError, "font description cannot be nil");


    RGSS_NewFont *font = DATA_PTR(self);

    // TODO: Build font description string based on opts (do in Ruby?)



    cairo_surface_t *temp = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
    font->context = cairo_create(temp);
    cairo_surface_destroy(temp);
    if (font->context == NULL)
        rb_raise(rb_eRGSSError, "failed to create context for font");

    font->layout = pango_cairo_create_layout(font->context);
    if (font->layout == NULL)
        rb_raise(rb_eRGSSError, "failed to create layout for font");

    font->desc = pango_font_description_from_string(StringValueCStr(description));
    if (font->desc == NULL)
        rb_raise(rb_eRGSSError, "failed to create font from description");
    pango_layout_set_font_description(font->layout, font->desc);


    return self;
}



























// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#if 0
typedef struct
{

    PangoContext *context;
    PangoLayout *layout;
    PangoFontDescription *desc;
    PangoFontMap *map;

    double size;
    RGSS_Color color;
} RGSS_Font;










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

    RGSS_SizeNotEmpty(size->width, size->height);
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

#endif

void RGSS_Init_Font(VALUE parent)
{
    rb_cFont = rb_define_class_under(parent, "Font", rb_cObject);
    rb_define_alloc_func(rb_cFont, RGSS_Font_Alloc);

    rb_define_singleton_method1(rb_cFont, "add_file", RGSS_Font_Add, 1);
   rb_define_singleton_method0(rb_cFont, "default_color", RGSS_Font_GetDefaultColor, 0);
    rb_define_singleton_method1(rb_cFont, "default_color=", RGSS_Font_SetDefaultColor, 1);
    rb_define_singleton_method0(rb_cFont, "default_size", RGSS_Font_GetDefaultSize, 0);
    rb_define_singleton_method1(rb_cFont, "default_size=", RGSS_Font_SetDefaultSize, 1);

    rb_define_methodm1(rb_cFont, "initialize", RGSS_Font_Initialize, -1);
    rb_define_method0(rb_cFont, "to_s", RGSS_Font_ToString, 0);
    rb_define_method0(rb_cFont, "to_str", RGSS_Font_ToString, 0);
    rb_define_method0(rb_cFont, "family", RGSS_Font_GetFamily, 0);
    rb_define_methodm1(rb_cFont, "measure", RGSS_Font_Measure, -1);
    DEFINE_ACCESSOR(rb_cFont, RGSS_Font, Size, "size");
    DEFINE_ACCESSOR(rb_cFont, RGSS_Font, Color, "color");
    DEFINE_ACCESSOR(rb_cFont, RGSS_Font, Style, "style");
    DEFINE_ACCESSOR(rb_cFont, RGSS_Font, Weight, "weight");
    DEFINE_ACCESSOR(rb_cFont, RGSS_Font, Gravity, "gravity");
    DEFINE_ACCESSOR(rb_cFont, RGSS_Font, Stretch, "stretch");

    
    


    // rb_define_methodm1(rb_cFont, "bake", RGSS_Font_Bake, -1);
    
    // rb_define_singleton_method3(rb_cFont, "test_texture", RGSS_Font_TestTexture, 3);

    rb_define_const(rb_cFont, "ALIGN_LEFT", INT2NUM(PANGO_ALIGN_LEFT));
    rb_define_const(rb_cFont, "ALIGN_CENTER", INT2NUM(PANGO_ALIGN_CENTER));
    rb_define_const(rb_cFont, "ALIGN_RIGHT", INT2NUM(PANGO_ALIGN_RIGHT));
    rb_define_const(rb_cFont, "ALIGN_TOP", INT2NUM(PANGO_ALIGN_LEFT));
    rb_define_const(rb_cFont, "ALIGN_BOTTOM", INT2NUM(PANGO_ALIGN_RIGHT));

    rb_define_const(rb_cFont, "STYLE_NORMAL", INT2NUM(PANGO_STYLE_NORMAL));
    rb_define_const(rb_cFont, "STYLE_OBLIQUE", INT2NUM(PANGO_STYLE_OBLIQUE));
    rb_define_const(rb_cFont, "STYLE_ITALIC", INT2NUM(PANGO_STYLE_ITALIC));
    rb_define_const(rb_cFont, "WEIGHT_THIN", INT2NUM(PANGO_WEIGHT_THIN));
    rb_define_const(rb_cFont, "WEIGHT_ULTRALIGHT", INT2NUM(PANGO_WEIGHT_ULTRALIGHT));
    rb_define_const(rb_cFont, "WEIGHT_LIGHT", INT2NUM(PANGO_WEIGHT_LIGHT));
    rb_define_const(rb_cFont, "WEIGHT_SEMILIGHT", INT2NUM(PANGO_WEIGHT_SEMILIGHT));
    rb_define_const(rb_cFont, "WEIGHT_BOOK", INT2NUM(PANGO_WEIGHT_BOOK));
    rb_define_const(rb_cFont, "WEIGHT_NORMAL", INT2NUM(PANGO_WEIGHT_NORMAL));
    rb_define_const(rb_cFont, "WEIGHT_MEDIUM", INT2NUM(PANGO_WEIGHT_MEDIUM));
    rb_define_const(rb_cFont, "WEIGHT_SEMIBOLD", INT2NUM(PANGO_WEIGHT_SEMIBOLD));
    rb_define_const(rb_cFont, "WEIGHT_BOLD", INT2NUM(PANGO_WEIGHT_BOLD));
    rb_define_const(rb_cFont, "WEIGHT_ULTRABOLD", INT2NUM(PANGO_WEIGHT_ULTRABOLD));
    rb_define_const(rb_cFont, "WEIGHT_HEAVY", INT2NUM(PANGO_WEIGHT_HEAVY));
    rb_define_const(rb_cFont, "WEIGHT_ULTRAHEAVY", INT2NUM(PANGO_WEIGHT_ULTRAHEAVY));
    rb_define_const(rb_cFont, "VARIANT_NORMAL", INT2NUM(PANGO_VARIANT_NORMAL));
    rb_define_const(rb_cFont, "VARIANT_SMALL_CAPS", INT2NUM(PANGO_VARIANT_SMALL_CAPS));
    rb_define_const(rb_cFont, "STRETCH_ULTRA_CONDENSED", INT2NUM(PANGO_STRETCH_ULTRA_CONDENSED));
    rb_define_const(rb_cFont, "STRETCH_EXTRA_CONDENSED", INT2NUM(PANGO_STRETCH_EXTRA_CONDENSED));
    rb_define_const(rb_cFont, "STRETCH_CONDENSED", INT2NUM(PANGO_STRETCH_CONDENSED));
    rb_define_const(rb_cFont, "STRETCH_SEMI_CONDENSED", INT2NUM(PANGO_STRETCH_SEMI_CONDENSED));
    rb_define_const(rb_cFont, "STRETCH_NORMAL", INT2NUM(PANGO_STRETCH_NORMAL));
    rb_define_const(rb_cFont, "STRETCH_SEMI_EXPANDED", INT2NUM(PANGO_STRETCH_SEMI_EXPANDED));
    rb_define_const(rb_cFont, "STRETCH_EXPANDED", INT2NUM(PANGO_STRETCH_EXPANDED));
    rb_define_const(rb_cFont, "STRETCH_EXTRA_EXPANDED", INT2NUM(PANGO_STRETCH_EXTRA_EXPANDED));
    rb_define_const(rb_cFont, "STRETCH_ULTRA_EXPANDED", INT2NUM(PANGO_STRETCH_ULTRA_EXPANDED));

// rb_define_const(rb_cFont, "MASK_FAMILY", INT2NUM(PANGO_FONT_MASK_FAMILY));
// rb_define_const(rb_cFont, "MASK_STYLE", INT2NUM(PANGO_FONT_MASK_STYLE));
// rb_define_const(rb_cFont, "MASK_VARIANT", INT2NUM(PANGO_FONT_MASK_VARIANT));
// rb_define_const(rb_cFont, "MASK_WEIGHT", INT2NUM(PANGO_FONT_MASK_WEIGHT));
// rb_define_const(rb_cFont, "MASK_STRETCH", INT2NUM(PANGO_FONT_MASK_STRETCH));
// rb_define_const(rb_cFont, "MASK_SIZE", INT2NUM(PANGO_FONT_MASK_SIZE));
// rb_define_const(rb_cFont, "MASK_GRAVITY", INT2NUM(PANGO_FONT_MASK_GRAVITY));
// rb_define_const(rb_cFont, "MASK_VARIATIONS", INT2NUM(PANGO_FONT_MASK_VARIATIONS));


   
}