#include "rgss.h"

VALUE rb_cColor;
VALUE rb_cTone;

RGSS_DEFINE_MARSHAL(RGSS_Color)

VALUE RGSS_Color_New(VALUE klass, float r, float g, float b, float a)
{
    float *color = RGSS_VEC4_NEW;
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
    glm_vec4_clamp(color, 0.0f, 1.0f);
    return Data_Wrap_Struct(klass, NULL, free, color);
}

static VALUE RGSS_Color_Alloc(VALUE klass)
{
    float *color = RGSS_VEC4_NEW;
    glm_vec4_zero(color);
    return Data_Wrap_Struct(klass, NULL, free, color);
}

static VALUE RGSS_Color_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE r, g, b, a;
    rb_scan_args(argc, argv, "31", &r, &g, &b, &a);

    float *color = DATA_PTR(self);
    color[0] = NUM2FLT(r);
    color[1] = NUM2FLT(g);
    color[2] = NUM2FLT(b);
    color[3] = RTEST(a) ? NUM2FLT(a) : 1.0f;

    glm_vec4_clamp(color, 0.0f, 1.0f);
    return self;
}

static VALUE RGSS_Tone_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE r, g, b, a;
    rb_scan_args(argc, argv, "31", &r, &g, &b, &a);

    float *tone = DATA_PTR(self);
    tone[0] = NUM2FLT(r);
    tone[1] = NUM2FLT(g);
    tone[2] = NUM2FLT(b);
    tone[3] = RTEST(a) ? glm_clamp(NUM2FLT(a), 0.0f, 1.0f) : 1.0f;

    glm_vec3_clamp(tone, -1.0f, 1.0f);
    return self;
}

static VALUE RGSS_Color_GetCSS(VALUE self)
{
    float *color = DATA_PTR(self);
    unsigned char r = (unsigned char) roundf(color[0] * 255.0f);        
    unsigned char g = (unsigned char) roundf(color[1] * 255.0f);
    unsigned char b = (unsigned char) roundf(color[2] * 255.0f);
    return rb_sprintf("#%02X%02X%02X", r, g, b);
}

static VALUE RGSS_Color_GetRed(VALUE self)
{
    return DBL2NUM(((float *)DATA_PTR(self))[0]);
}

static VALUE RGSS_Color_GetGreen(VALUE self)
{
    return DBL2NUM(((float *)DATA_PTR(self))[1]);
}

static VALUE RGSS_Color_GetBlue(VALUE self)
{
    return DBL2NUM(((float *)DATA_PTR(self))[2]);
}

static VALUE RGSS_Color_GetAlpha(VALUE self)
{
    return DBL2NUM(((float *)DATA_PTR(self))[3]);
}

static VALUE RGSS_Color_Equal(VALUE self, VALUE other)
{
    if (CLASS_OF(self) != CLASS_OF(other))
        return Qfalse;

    float *v1 = DATA_PTR(self), *v2 = DATA_PTR(other);
    return RB_BOOL(glm_vec4_eqv(v1, v2));
}

static VALUE RGSS_Color_Mix(VALUE klass, VALUE from, VALUE to, VALUE amount)
{
    float *c1 = DATA_PTR(from), *c2 = DATA_PTR(to);
    float t = glm_clamp(NUM2FLT(amount), 0.0f, 1.0f);

    float *result = RGSS_VEC4_NEW;

    glm_vec4_lerp(c1, c2, t, result);
    return Data_Wrap_Struct(klass, NULL, free, result);
}

static VALUE RGSS_Color_Inspect(VALUE self)
{
    float *c = DATA_PTR(self);
    return rb_sprintf("<%.4f, %.4f, %.4f, %.4f>", c[0], c[1], c[2], c[3]);
}

static VALUE RGSS_Color_Clone(VALUE self)
{
    float *c1 = DATA_PTR(self), *c2 = RGSS_VEC4_NEW;
    glm_vec4_copy(c1, c2);
    return Data_Wrap_Struct(CLASS_OF(self), NULL, free, c2);
}

static VALUE RGSS_Color_ToArray(VALUE self)
{
    float *vec = DATA_PTR(self);
    return rb_ary_new_from_args(4, DBL2NUM(vec[0]), DBL2NUM(vec[1]), DBL2NUM(vec[2]), DBL2NUM(vec[3]));
}

static VALUE RGSS_Color_ToHash(VALUE self)
{
    float *vec = DATA_PTR(self);
    VALUE hash = rb_hash_new();

    rb_hash_aset(hash, STR2SYM("red"), DBL2NUM(vec[0]));
    rb_hash_aset(hash, STR2SYM("green"), DBL2NUM(vec[1]));
    rb_hash_aset(hash, STR2SYM("blue"), DBL2NUM(vec[2]));
    rb_hash_aset(hash, STR2SYM("alpha"), DBL2NUM(vec[3]));

    return hash;
}

static VALUE RGSS_Tone_ToHash(VALUE self)
{
    float *vec = DATA_PTR(self);
    VALUE hash = rb_hash_new();

    rb_hash_aset(hash, STR2SYM("red"), DBL2NUM(vec[0]));
    rb_hash_aset(hash, STR2SYM("green"), DBL2NUM(vec[1]));
    rb_hash_aset(hash, STR2SYM("blue"), DBL2NUM(vec[2]));
    rb_hash_aset(hash, STR2SYM("gray"), DBL2NUM(vec[3]));

    return hash;
}

static VALUE RGSS_Color_GetHue(VALUE self)
{
    float *color = DATA_PTR(self);

    if (glm_eq(color[0], color[1]) && glm_eq(color[1], color[2]))
    {
        return DBL2NUM(0.0);
    }

    float max = glm_max(color[0], glm_max(color[1], color[2]));
    float min = glm_min(color[0], glm_min(color[1], color[2]));
    float delta = max - min, hue = 0.0;

    if (glm_eq(color[0], max))
    {
        hue = (color[1] - color[2]) / delta;
    }
    else if (glm_eq(color[1], max))
    {
        hue = 2.0f + (color[2] - color[0]) / delta;
    }
    else if (glm_eq(color[2], max))
    {
        hue = 4.0f + (color[0] - color[1]) / delta;
    }

    hue *= 60.0f;
    if (hue < 0.0f)
        hue += 360.0f;

    return DBL2NUM(hue);
}

static VALUE RGSS_Color_GetSaturation(VALUE self)
{
    float *color = DATA_PTR(self);
    float max = glm_max(color[0], glm_max(color[1], color[2]));
    float min = glm_min(color[0], glm_min(color[1], color[2]));
    float lightness, s = 0.0f;

    if (fabsf(max - min) > __FLT_EPSILON__)
    {
        lightness = (max + min) * 0.5f;
        s = (max - min) / (lightness <= 0.5f ? (max + min) : (2 - max - min));
    }
    return DBL2NUM(s);
}

static VALUE RGSS_Color_GetBrightness(VALUE self)
{
    float *color = DATA_PTR(self);
    return DBL2NUM(glm_max(color[0], glm_max(color[1], color[2])));
}

static VALUE RGSS_Color_GetLightness(VALUE self)
{
    float *color = DATA_PTR(self);
    float max = glm_max(color[0], glm_max(color[1], color[2]));
    float min = glm_min(color[0], glm_min(color[1], color[2]));
    return DBL2NUM((max + min) * 0.5f);
}

static VALUE RGSS_Color_GetHSL(VALUE self)
{
    float *color = DATA_PTR(self);
    float max = glm_max(color[0], glm_max(color[1], color[2]));
    float min = glm_min(color[0], glm_min(color[1], color[2]));
    float delta = max - min, lightness = (max + min) * 0.5f, hue = 0.0f, saturation = 0.0f;

    if (delta > __FLT_EPSILON__)
    {
        saturation = (max - min) / (lightness <= 0.5f ? (max + min) : (2 - max - min));

        if (glm_eq(color[0], max))
        {
            hue = (color[1] - color[2]) / delta;
        }
        else if (glm_eq(color[1], max))
        {
            hue = 2.0f + (color[2] - color[0]) / delta;
        }
        else if (glm_eq(color[2], max))
        {
            hue = 4.0f + (color[0] - color[1]) / delta;
        }

        hue *= 60.0f;
        if (hue < 0.0f)
            hue += 360.0f;
    }
    return rb_ary_new_from_args(3, DBL2NUM(hue), DBL2NUM(saturation), DBL2NUM(lightness));
}

static VALUE RGSS_Color_GetHSB(VALUE self)
{
    float *color = DATA_PTR(self);
    float max = glm_max(color[0], glm_max(color[1], color[2]));
    float min = glm_min(color[0], glm_min(color[1], color[2]));
    float delta = max - min, s = 0.0f, h = 0.0f, v = max;

    if (delta > __FLT_EPSILON__)
    {
        s = (delta / max);
        if (color[0] >= max)
            h = (color[1] - color[2]) / delta;
        else if (color[1] >= max)
            h = 2.0f + (color[2] - color[0]) / delta;
        else
            h = 4.0f + (color[0] - color[1]) / delta;
        h *= 60.0f;
        if (h < 0.0f)
            h += 360.0f;
    }
    return rb_ary_new_from_args(3, DBL2NUM(h), DBL2NUM(s), DBL2NUM(v));
}

static void RGSS_Color_HSB2RGB(float *color, float hue, float saturation, float brightness)
{
    float r, g, b;

    if (saturation < __FLT_EPSILON__)
    {
        r = brightness;
        g = brightness;
        b = brightness;
    }
    else
    {
        float p, q, t, ff;
        if (hue >= 360.0f)
            hue = 0.0f;
        hue /= 60.0f;
        int i = (int)hue;
        ff = hue - i;
        p = brightness * (1.0f - saturation);
        q = brightness * (1.0f - (saturation * ff));
        t = brightness * (1.0f - (saturation * (1.0f - ff)));

        switch (i)
        {
        case 0:
            r = brightness;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = brightness;
            b = p;
            break;
        case 2:
            r = p;
            g = brightness;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = brightness;
            break;
        case 4:
            r = t;
            g = p;
            b = brightness;
            break;
        case 5:
        default:
            r = brightness;
            g = p;
            b = q;
            break;
        }
    }

    color[0] = r;
    color[1] = g;
    color[2] = b;
}

static VALUE RGSS_Color_FromHSB(int argc, VALUE *argv, VALUE klass)
{
    VALUE hue, saturation, brightness, alpha;
    rb_scan_args(argc, argv, "31", &hue, &saturation, &brightness, &alpha);

    float h = glm_clamp(NUM2FLT(hue), 0.0f, 360.0f);
    float s = glm_clamp(NUM2FLT(saturation), 0.0f, 1.0f);
    float b = glm_clamp(NUM2FLT(brightness), 0.0f, 1.0f);

    float *color = RGSS_VEC4_NEW;
    RGSS_Color_HSB2RGB(color, h, s, b);
    color[3] = NIL_P(alpha) ? 1.0f : glm_clamp(NUM2FLT(alpha), 0.0f, 1.0f);

    return Data_Wrap_Struct(klass, NULL, free, color);
}

static inline void RGSS_Color_HSL2HSB(float in_s, float in_l, float *out_s, float *out_b)
{
    float t = in_s * (in_l < 0.5f ? in_l : 1.0f - in_l);
    *out_b = in_l + t;
    *out_s = in_l > 0.0f ? 2.0f * t / *out_b : *out_s;
}

static VALUE RGSS_Color_FromHSL(int argc, VALUE *argv, VALUE klass)
{
    VALUE hue, saturation, lightness, alpha;
    rb_scan_args(argc, argv, "31", &hue, &saturation, &lightness, &alpha);

    float h = glm_clamp(NUM2FLT(hue), 0.0f, 360.0f);
    float s = glm_clamp(NUM2FLT(saturation), 0.0f, 1.0f);
    float l = glm_clamp(NUM2FLT(lightness), 0.0f, 1.0f);
    float sat, b;

    // Convert the saturation and lightness to be consistent with HSB/HSV, then compute
    RGSS_Color_HSL2HSB(s, l, &sat, &b);

    float *color = RGSS_VEC4_NEW;
    RGSS_Color_HSB2RGB(color, h, sat, b);
    color[3] = NIL_P(alpha) ? 1.0f : glm_clamp(NUM2FLT(alpha), 0.0f, 1.0f);
    return Data_Wrap_Struct(klass, NULL, free, color);
}

static VALUE RGSS_Color_Multiply(VALUE self, VALUE scale)
{
    float *color = DATA_PTR(self), *result = RGSS_VEC4_NEW;
    float w = NUM2FLT(scale);
    result[0] = glm_clamp(color[0] * w, 0.0f, 1.0f);
    result[1] = glm_clamp(color[1] * w, 0.0f, 1.0f);
    result[2] = glm_clamp(color[2] * w, 0.0f, 1.0f);
    result[3] = glm_clamp(color[3] * w, 0.0f, 1.0f);
    return Data_Wrap_Struct(CLASS_OF(self), NULL, free, result);
}

static VALUE RGSS_Color_Inverse(int argc, VALUE *argv, VALUE self)
{
    VALUE include_alpha;
    rb_scan_args(argc, argv, "01", &include_alpha);
    float *color = DATA_PTR(self), *result = RGSS_VEC4_NEW;
    result[0] = 1.0f - color[0];
    result[1] = 1.0f - color[1];
    result[2] = 1.0f - color[2];
    result[3] = RTEST(include_alpha) ? 1.0f - color[3] : color[3];
    return Data_Wrap_Struct(CLASS_OF(self), NULL, free, result);
}

void RGSS_Init_ColorAndTone(VALUE parent)
{
    rb_cColor = rb_define_class_under(parent, "Color", rb_cObject);
    rb_cTone = rb_define_class_under(parent, "Tone", rb_cObject);
    rb_define_alloc_func(rb_cColor, RGSS_Color_Alloc);
    rb_define_alloc_func(rb_cTone, RGSS_Color_Alloc);

    rb_define_methodm1(rb_cColor, "initialize", RGSS_Color_Initialize, -1);
    rb_define_method0(rb_cColor, "r", RGSS_Color_GetRed, 0);
    rb_define_method0(rb_cColor, "g", RGSS_Color_GetGreen, 0);
    rb_define_method0(rb_cColor, "b", RGSS_Color_GetBlue, 0);
    rb_define_method0(rb_cColor, "a", RGSS_Color_GetAlpha, 0);
    rb_define_method1(rb_cColor, "eql?", RGSS_Color_Equal, 1);
    rb_define_method1(rb_cColor, "==", RGSS_Color_Equal, 1);
    rb_define_method1(rb_cColor, "*", RGSS_Color_Multiply, 1);
    rb_define_method0(rb_cColor, "inspect", RGSS_Color_Inspect, 0);
    rb_define_method0(rb_cColor, "clone", RGSS_Color_Clone, 0);
    rb_define_method0(rb_cColor, "dup", RGSS_Color_Clone, 0);
    rb_define_method0(rb_cColor, "to_a", RGSS_Color_ToArray, 0);
    rb_define_method0(rb_cColor, "to_h", RGSS_Color_ToHash, 0);
    rb_define_methodm1(rb_cColor, "inverse", RGSS_Color_Inverse, -1);
    rb_define_methodm1(rb_cColor, "_dump", RGSS_Color_Dump, -1);

    rb_define_method0(rb_cColor, "css", RGSS_Color_GetCSS, 0);
    rb_define_method0(rb_cColor, "hue", RGSS_Color_GetHue, 0);
    rb_define_method0(rb_cColor, "saturation", RGSS_Color_GetSaturation, 0);
    rb_define_method0(rb_cColor, "lightness", RGSS_Color_GetLightness, 0);
    rb_define_method0(rb_cColor, "brightness", RGSS_Color_GetBrightness, 0);
    rb_define_method0(rb_cColor, "hsl", RGSS_Color_GetHSL, 0);
    rb_define_method0(rb_cColor, "hsb", RGSS_Color_GetHSB, 0);

    rb_define_alias(rb_cColor, "multiply", "*");
    rb_define_alias(rb_cColor, "red", "r");
    rb_define_alias(rb_cColor, "green", "g");
    rb_define_alias(rb_cColor, "blue", "b");
    rb_define_alias(rb_cColor, "alpha", "a");
    rb_define_alias(rb_cColor, "value", "brightness");
    rb_define_alias(rb_cColor, "hsv", "hsb");
    rb_define_singleton_method3(rb_cColor, "mix", RGSS_Color_Mix, 3);

    rb_define_singleton_methodm1(rb_cColor, "from_hsl", RGSS_Color_FromHSL, -1);
    rb_define_singleton_methodm1(rb_cColor, "from_hsb", RGSS_Color_FromHSB, -1);
    rb_define_singleton_methodm1(rb_cColor, "from_hsv", RGSS_Color_FromHSB, -1);

    rb_define_methodm1(rb_cTone, "initialize", RGSS_Tone_Initialize, -1);
    rb_define_method0(rb_cTone, "r", RGSS_Color_GetRed, 0);
    rb_define_method0(rb_cTone, "g", RGSS_Color_GetGreen, 0);
    rb_define_method0(rb_cTone, "b", RGSS_Color_GetBlue, 0);
    rb_define_method0(rb_cTone, "gray", RGSS_Color_GetAlpha, 0);
    rb_define_method1(rb_cTone, "eql?", RGSS_Color_Equal, 1);
    rb_define_method1(rb_cTone, "==", RGSS_Color_Equal, 1);
    rb_define_method0(rb_cTone, "inspect", RGSS_Color_Inspect, 0);
    rb_define_method0(rb_cTone, "clone", RGSS_Color_Clone, 0);
    rb_define_method0(rb_cTone, "dup", RGSS_Color_Clone, 0);
    rb_define_method0(rb_cTone, "to_a", RGSS_Color_ToArray, 0);
    rb_define_method0(rb_cTone, "to_h", RGSS_Tone_ToHash, 0);
    rb_define_methodm1(rb_cTone, "_dump", RGSS_Color_Dump, -1);

    rb_define_alias(rb_cTone, "red", "r");
    rb_define_alias(rb_cTone, "green", "g");
    rb_define_alias(rb_cTone, "blue", "b");
    rb_define_alias(rb_cTone, "grey", "gray");

    rb_define_singleton_method1(rb_cColor, "_load", RGSS_Color_Load, 1);
    rb_define_singleton_method1(rb_cTone, "_load", RGSS_Color_Load, 1);

    float *gray = RGSS_VEC4_NEW;
    glm_vec4_zero(gray);
    gray[3] = 1.0f;
    rb_define_const(rb_cTone, "GRAY", Data_Wrap_Struct(rb_cTone, NULL, free, gray));

#define COLOR_CONST(r, g, b, a, name) rb_define_const(rb_cColor, #name, RGSS_Color_New(rb_cColor, r, g, b, a))
    COLOR_CONST(0.00000f, 0.00000f, 0.00000f, 0.00000f, NONE);
    COLOR_CONST(1.00000f, 1.00000f, 1.00000f, 0.00000f, TRANSPARENT);
    COLOR_CONST(0.94118f, 0.97255f, 1.00000f, 1.00000f, ALICE_BLUE);
    COLOR_CONST(0.98039f, 0.92157f, 0.84314f, 1.00000f, ANTIQUE_WHITE);
    COLOR_CONST(0.00000f, 1.00000f, 1.00000f, 1.00000f, AQUA);
    COLOR_CONST(0.49804f, 1.00000f, 0.83137f, 1.00000f, AQUAMARINE);
    COLOR_CONST(0.94118f, 1.00000f, 1.00000f, 1.00000f, AZURE);
    COLOR_CONST(0.96078f, 0.96078f, 0.86275f, 1.00000f, BEIGE);
    COLOR_CONST(1.00000f, 0.89412f, 0.76863f, 1.00000f, BISQUE);
    COLOR_CONST(0.00000f, 0.00000f, 0.00000f, 1.00000f, BLACK);
    COLOR_CONST(1.00000f, 0.92157f, 0.80392f, 1.00000f, BLANCHED_ALMOND);
    COLOR_CONST(0.00000f, 0.00000f, 1.00000f, 1.00000f, BLUE);
    COLOR_CONST(0.54118f, 0.16863f, 0.88627f, 1.00000f, BLUE_VIOLET);
    COLOR_CONST(0.64706f, 0.16471f, 0.16471f, 1.00000f, BROWN);
    COLOR_CONST(0.87059f, 0.72157f, 0.52941f, 1.00000f, BURLY_WOOD);
    COLOR_CONST(0.37255f, 0.61961f, 0.62745f, 1.00000f, CADET_BLUE);
    COLOR_CONST(0.49804f, 1.00000f, 0.00000f, 1.00000f, CHARTREUSE);
    COLOR_CONST(0.82353f, 0.41176f, 0.11765f, 1.00000f, CHOCOLATE);
    COLOR_CONST(1.00000f, 0.49804f, 0.31373f, 1.00000f, CORAL);
    COLOR_CONST(0.39216f, 0.58431f, 0.92941f, 1.00000f, CORNFLOWER_BLUE);
    COLOR_CONST(1.00000f, 0.97255f, 0.86275f, 1.00000f, CORNSILK);
    COLOR_CONST(0.86275f, 0.07843f, 0.23529f, 1.00000f, CRIMSON);
    COLOR_CONST(0.00000f, 1.00000f, 1.00000f, 1.00000f, CYAN);
    COLOR_CONST(0.00000f, 0.00000f, 0.54510f, 1.00000f, DARK_BLUE);
    COLOR_CONST(0.00000f, 0.54510f, 0.54510f, 1.00000f, DARK_CYAN);
    COLOR_CONST(0.72157f, 0.52549f, 0.04314f, 1.00000f, DARK_GOLDENROD);
    COLOR_CONST(0.66275f, 0.66275f, 0.66275f, 1.00000f, DARK_GRAY);
    COLOR_CONST(0.00000f, 0.39216f, 0.00000f, 1.00000f, DARK_GREEN);
    COLOR_CONST(0.74118f, 0.71765f, 0.41961f, 1.00000f, DARK_KHAKI);
    COLOR_CONST(0.54510f, 0.00000f, 0.54510f, 1.00000f, DARK_MAGENTA);
    COLOR_CONST(0.33333f, 0.41961f, 0.18431f, 1.00000f, DARK_OLIVE_GREEN);
    COLOR_CONST(1.00000f, 0.54902f, 0.00000f, 1.00000f, DARK_ORANGE);
    COLOR_CONST(0.60000f, 0.19608f, 0.80000f, 1.00000f, DARK_ORCHID);
    COLOR_CONST(0.54510f, 0.00000f, 0.00000f, 1.00000f, DARK_RED);
    COLOR_CONST(0.91373f, 0.58824f, 0.47843f, 1.00000f, DARK_SALMON);
    COLOR_CONST(0.56078f, 0.73725f, 0.54510f, 1.00000f, DARK_SEA_GREEN);
    COLOR_CONST(0.28235f, 0.23922f, 0.54510f, 1.00000f, DARK_SLATE_BLUE);
    COLOR_CONST(0.18431f, 0.30980f, 0.30980f, 1.00000f, DARK_SLATE_GRAY);
    COLOR_CONST(0.00000f, 0.80784f, 0.81961f, 1.00000f, DARK_TURQUOISE);
    COLOR_CONST(0.58039f, 0.00000f, 0.82745f, 1.00000f, DARK_VIOLET);
    COLOR_CONST(1.00000f, 0.07843f, 0.57647f, 1.00000f, DEEP_PINK);
    COLOR_CONST(0.00000f, 0.74902f, 1.00000f, 1.00000f, DEEP_SKY_BLUE);
    COLOR_CONST(0.41176f, 0.41176f, 0.41176f, 1.00000f, DIM_GRAY);
    COLOR_CONST(0.11765f, 0.56471f, 1.00000f, 1.00000f, DODGER_BLUE);
    COLOR_CONST(0.69804f, 0.13333f, 0.13333f, 1.00000f, FIREBRICK);
    COLOR_CONST(1.00000f, 0.98039f, 0.94118f, 1.00000f, FLORAL_WHITE);
    COLOR_CONST(0.13333f, 0.54510f, 0.13333f, 1.00000f, FOREST_GREEN);
    COLOR_CONST(1.00000f, 0.00000f, 1.00000f, 1.00000f, FUCHSIA);
    COLOR_CONST(0.86275f, 0.86275f, 0.86275f, 1.00000f, GAINSBORO);
    COLOR_CONST(0.97255f, 0.97255f, 1.00000f, 1.00000f, GHOST_WHITE);
    COLOR_CONST(1.00000f, 0.84314f, 0.00000f, 1.00000f, GOLD);
    COLOR_CONST(0.85490f, 0.64706f, 0.12549f, 1.00000f, GOLDENROD);
    COLOR_CONST(0.50196f, 0.50196f, 0.50196f, 1.00000f, GRAY);
    COLOR_CONST(0.00000f, 0.50196f, 0.00000f, 1.00000f, GREEN);
    COLOR_CONST(0.67843f, 1.00000f, 0.18431f, 1.00000f, GREEN_YELLOW);
    COLOR_CONST(0.94118f, 1.00000f, 0.94118f, 1.00000f, HONEYDEW);
    COLOR_CONST(1.00000f, 0.41176f, 0.70588f, 1.00000f, HOT_PINK);
    COLOR_CONST(0.80392f, 0.36078f, 0.36078f, 1.00000f, INDIAN_RED);
    COLOR_CONST(0.29412f, 0.00000f, 0.50980f, 1.00000f, INDIGO);
    COLOR_CONST(1.00000f, 1.00000f, 0.94118f, 1.00000f, IVORY);
    COLOR_CONST(0.94118f, 0.90196f, 0.54902f, 1.00000f, KHAKI);
    COLOR_CONST(0.90196f, 0.90196f, 0.98039f, 1.00000f, LAVENDER);
    COLOR_CONST(1.00000f, 0.94118f, 0.96078f, 1.00000f, LAVENDER_BLUSH);
    COLOR_CONST(0.48627f, 0.98824f, 0.00000f, 1.00000f, LAWN_GREEN);
    COLOR_CONST(1.00000f, 0.98039f, 0.80392f, 1.00000f, LEMON_CHIFFON);
    COLOR_CONST(0.67843f, 0.84706f, 0.90196f, 1.00000f, LIGHT_BLUE);
    COLOR_CONST(0.94118f, 0.50196f, 0.50196f, 1.00000f, LIGHT_CORAL);
    COLOR_CONST(0.87843f, 1.00000f, 1.00000f, 1.00000f, LIGHT_CYAN);
    COLOR_CONST(0.98039f, 0.98039f, 0.82353f, 1.00000f, LIGHT_GOLDENROD_YELLOW);
    COLOR_CONST(0.82745f, 0.82745f, 0.82745f, 1.00000f, LIGHT_GRAY);
    COLOR_CONST(0.56471f, 0.93333f, 0.56471f, 1.00000f, LIGHT_GREEN);
    COLOR_CONST(1.00000f, 0.71373f, 0.75686f, 1.00000f, LIGHT_PINK);
    COLOR_CONST(1.00000f, 0.62745f, 0.47843f, 1.00000f, LIGHT_SALMON);
    COLOR_CONST(0.12549f, 0.69804f, 0.66667f, 1.00000f, LIGHT_SEA_GREEN);
    COLOR_CONST(0.52941f, 0.80784f, 0.98039f, 1.00000f, LIGHT_SKY_BLUE);
    COLOR_CONST(0.46667f, 0.53333f, 0.60000f, 1.00000f, LIGHT_SLATE_GRAY);
    COLOR_CONST(0.69020f, 0.76863f, 0.87059f, 1.00000f, LIGHT_STEEL_BLUE);
    COLOR_CONST(1.00000f, 1.00000f, 0.87843f, 1.00000f, LIGHT_YELLOW);
    COLOR_CONST(0.00000f, 1.00000f, 0.00000f, 1.00000f, LIME);
    COLOR_CONST(0.19608f, 0.80392f, 0.19608f, 1.00000f, LIME_GREEN);
    COLOR_CONST(0.98039f, 0.94118f, 0.90196f, 1.00000f, LINEN);
    COLOR_CONST(1.00000f, 0.00000f, 1.00000f, 1.00000f, MAGENTA);
    COLOR_CONST(0.50196f, 0.00000f, 0.00000f, 1.00000f, MAROON);
    COLOR_CONST(0.40000f, 0.80392f, 0.66667f, 1.00000f, MEDIUM_AQUAMARINE);
    COLOR_CONST(0.00000f, 0.00000f, 0.80392f, 1.00000f, MEDIUM_BLUE);
    COLOR_CONST(0.72941f, 0.33333f, 0.82745f, 1.00000f, MEDIUM_ORCHID);
    COLOR_CONST(0.57647f, 0.43922f, 0.85882f, 1.00000f, MEDIUM_PURPLE);
    COLOR_CONST(0.23529f, 0.70196f, 0.44314f, 1.00000f, MEDIUM_SEA_GREEN);
    COLOR_CONST(0.48235f, 0.40784f, 0.93333f, 1.00000f, MEDIUM_SLATE_BLUE);
    COLOR_CONST(0.00000f, 0.98039f, 0.60392f, 1.00000f, MEDIUM_SPRING_GREEN);
    COLOR_CONST(0.28235f, 0.81961f, 0.80000f, 1.00000f, MEDIUM_TURQUOISE);
    COLOR_CONST(0.78039f, 0.08235f, 0.52157f, 1.00000f, MEDIUM_VIOLET_RED);
    COLOR_CONST(0.09804f, 0.09804f, 0.43922f, 1.00000f, MIDNIGHT_BLUE);
    COLOR_CONST(0.96078f, 1.00000f, 0.98039f, 1.00000f, MINT_CREAM);
    COLOR_CONST(1.00000f, 0.89412f, 0.88235f, 1.00000f, MISTY_ROSE);
    COLOR_CONST(1.00000f, 0.89412f, 0.70980f, 1.00000f, MOCCASIN);
    COLOR_CONST(1.00000f, 0.87059f, 0.67843f, 1.00000f, NAVAJO_WHITE);
    COLOR_CONST(0.00000f, 0.00000f, 0.50196f, 1.00000f, NAVY);
    COLOR_CONST(0.99216f, 0.96078f, 0.90196f, 1.00000f, OLD_LACE);
    COLOR_CONST(0.50196f, 0.50196f, 0.00000f, 1.00000f, OLIVE);
    COLOR_CONST(0.41961f, 0.55686f, 0.13725f, 1.00000f, OLIVE_DRAB);
    COLOR_CONST(1.00000f, 0.64706f, 0.00000f, 1.00000f, ORANGE);
    COLOR_CONST(1.00000f, 0.27059f, 0.00000f, 1.00000f, ORANGE_RED);
    COLOR_CONST(0.85490f, 0.43922f, 0.83922f, 1.00000f, ORCHID);
    COLOR_CONST(0.93333f, 0.90980f, 0.66667f, 1.00000f, PALE_GOLDENROD);
    COLOR_CONST(0.59608f, 0.98431f, 0.59608f, 1.00000f, PALE_GREEN);
    COLOR_CONST(0.68627f, 0.93333f, 0.93333f, 1.00000f, PALE_TURQUOISE);
    COLOR_CONST(0.85882f, 0.43922f, 0.57647f, 1.00000f, PALE_VIOLET_RED);
    COLOR_CONST(1.00000f, 0.93725f, 0.83529f, 1.00000f, PAPAYA_WHIP);
    COLOR_CONST(1.00000f, 0.85490f, 0.72549f, 1.00000f, PEACH_PUFF);
    COLOR_CONST(0.80392f, 0.52157f, 0.24706f, 1.00000f, PERU);
    COLOR_CONST(1.00000f, 0.75294f, 0.79608f, 1.00000f, PINK);
    COLOR_CONST(0.86667f, 0.62745f, 0.86667f, 1.00000f, PLUM);
    COLOR_CONST(0.69020f, 0.87843f, 0.90196f, 1.00000f, POWDER_BLUE);
    COLOR_CONST(0.50196f, 0.00000f, 0.50196f, 1.00000f, PURPLE);
    COLOR_CONST(1.00000f, 0.00000f, 0.00000f, 1.00000f, RED);
    COLOR_CONST(0.73725f, 0.56078f, 0.56078f, 1.00000f, ROSY_BROWN);
    COLOR_CONST(0.25490f, 0.41176f, 0.88235f, 1.00000f, ROYAL_BLUE);
    COLOR_CONST(0.54510f, 0.27059f, 0.07451f, 1.00000f, SADDLE_BROWN);
    COLOR_CONST(0.98039f, 0.50196f, 0.44706f, 1.00000f, SALMON);
    COLOR_CONST(0.95686f, 0.64314f, 0.37647f, 1.00000f, SANDY_BROWN);
    COLOR_CONST(0.18039f, 0.54510f, 0.34118f, 1.00000f, SEA_GREEN);
    COLOR_CONST(1.00000f, 0.96078f, 0.93333f, 1.00000f, SEA_SHELL);
    COLOR_CONST(0.62745f, 0.32157f, 0.17647f, 1.00000f, SIENNA);
    COLOR_CONST(0.75294f, 0.75294f, 0.75294f, 1.00000f, SILVER);
    COLOR_CONST(0.52941f, 0.80784f, 0.92157f, 1.00000f, SKY_BLUE);
    COLOR_CONST(0.41569f, 0.35294f, 0.80392f, 1.00000f, SLATE_BLUE);
    COLOR_CONST(0.43922f, 0.50196f, 0.56471f, 1.00000f, SLATE_GRAY);
    COLOR_CONST(1.00000f, 0.98039f, 0.98039f, 1.00000f, SNOW);
    COLOR_CONST(0.00000f, 1.00000f, 0.49804f, 1.00000f, SPRING_GREEN);
    COLOR_CONST(0.27451f, 0.50980f, 0.70588f, 1.00000f, STEEL_BLUE);
    COLOR_CONST(0.82353f, 0.70588f, 0.54902f, 1.00000f, TAN);
    COLOR_CONST(0.00000f, 0.50196f, 0.50196f, 1.00000f, TEAL);
    COLOR_CONST(0.84706f, 0.74902f, 0.84706f, 1.00000f, THISTLE);
    COLOR_CONST(1.00000f, 0.38824f, 0.27843f, 1.00000f, TOMATO);
    COLOR_CONST(0.25098f, 0.87843f, 0.81569f, 1.00000f, TURQUOISE);
    COLOR_CONST(0.93333f, 0.50980f, 0.93333f, 1.00000f, VIOLET);
    COLOR_CONST(0.96078f, 0.87059f, 0.70196f, 1.00000f, WHEAT);
    COLOR_CONST(1.00000f, 1.00000f, 1.00000f, 1.00000f, WHITE);
    COLOR_CONST(0.96078f, 0.96078f, 0.96078f, 1.00000f, WHITE_SMOKE);
    COLOR_CONST(1.00000f, 1.00000f, 0.00000f, 1.00000f, YELLOW);
    COLOR_CONST(0.60392f, 0.80392f, 0.19608f, 1.00000f, YELLOW_GREEN);
#undef COLOR_CONST
}