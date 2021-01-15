#include "game.h"

VALUE rb_cVec4;

CREATE_ALLOC_FUNC(vec4, RGSS_VEC4_ALIGN, RGSS_VEC4_SIZE)
VEC_ATTR_ACCESSOR(vec4, x, 0)
VEC_ATTR_ACCESSOR(vec4, y, 1)
VEC_ATTR_ACCESSOR(vec4, z, 2)
VEC_ATTR_ACCESSOR(vec4, w, 3)

static VALUE vec4_get(VALUE self, VALUE index)
{
    int i = NUM2INT(index);
    if (index < 0 || index > 3)
        rb_raise(rb_eArgError, "index of of range (must be 0..3)");
    return DBL2NUM(((float*) DATA_PTR(self))[i]);
}

static VALUE vec4_set(VALUE self, VALUE index, VALUE value)
{
    int i = NUM2INT(index);
    if (index < 0 || index > 3)
        rb_raise(rb_eArgError, "index of of range (must be 0..3)");
    ((float*) DATA_PTR(self))[i] = NUM2FLT(value);
    return value;
}

static VALUE vec4_copy(VALUE self)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_copy(DATA_PTR(self), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_zero(VALUE klass)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_zero(d);
    return VEC4_WRAP(d);
}

static VALUE vec4_one(VALUE klass)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_one(d);
    return VEC4_WRAP(d);
}

static VALUE vec4_unitx(VALUE klass)
{
    return RGSS_Vec4_New(1.0f, 0.0f, 0.0f, 0.0f);
}

static VALUE vec4_unity(VALUE klass)
{
    return RGSS_Vec4_New(0.0f, 1.0f, 0.0f, 0.0f);
}

static VALUE vec4_unitz(VALUE klass)
{
    return RGSS_Vec4_New(0.0f, 0.0f, 1.0f, 0.0f);
}

static VALUE vec4_unitw(VALUE klass)
{
    return RGSS_Vec4_New(0.0f, 0.0f, 0.0f, 1.0f);
}

static VALUE vec4_dot(VALUE self, VALUE other)
{
    return DBL2NUM(glm_vec4_dot(DATA_PTR(self), DATA_PTR(other)));
}

static VALUE vec4_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC4_NEW;
    float *v = DATA_PTR(self);

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec4_add(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec4_adds(v, NUM2FLT(other), d);
    }
    return VEC4_WRAP(d);
}

static VALUE vec4_sub(VALUE self, VALUE other)
{
    float *d = RGSS_VEC4_NEW;
    float *v = DATA_PTR(self);

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec4_sub(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec4_subs(v, NUM2FLT(other), d);
    }
    return VEC4_WRAP(d);
}

static VALUE vec4_mul(VALUE self, VALUE other)
{
    float *d = RGSS_VEC4_NEW;
    float *v = DATA_PTR(self);

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec4_mul(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec4_scale(v, NUM2FLT(other), d);
    }
    return VEC4_WRAP(d);
}

static VALUE vec4_div(VALUE self, VALUE other)
{
    float *d = RGSS_VEC4_NEW;
    float *v = DATA_PTR(self);

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec4_div(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec4_divs(v, NUM2FLT(other), d);
    }
    return VEC4_WRAP(d);
}

static VALUE vec4_add_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_addadd(DATA_PTR(self), DATA_PTR(other), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_sub_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_subadd(DATA_PTR(self), DATA_PTR(other), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_mul_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC4_NEW;
    float *v = DATA_PTR(self);

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec4_muladd(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec4_muladds(v, NUM2FLT(other), d);
    }

    return VEC4_WRAP(d);
}

static VALUE vec4_max_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_maxadd(DATA_PTR(self), DATA_PTR(other), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_min_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_minadd(DATA_PTR(self), DATA_PTR(other), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_length(VALUE self)
{
    return DBL2NUM(glm_vec4_norm(DATA_PTR(self)));
}

static VALUE vec4_length_squared(VALUE self)
{
    return DBL2NUM(glm_vec4_norm(DATA_PTR(self)));
}

static VALUE vec4_norm_scale(VALUE self, VALUE value)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_scale_as(DATA_PTR(self), NUM2FLT(value), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_negate(VALUE self)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_negate_to(DATA_PTR(self), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_negate_bang(VALUE self)
{
    glm_vec4_negate(DATA_PTR(self));
    return self;
}

static VALUE vec4_manhattan_distance(VALUE self)
{
    return DBL2NUM(glm_vec4_norm_one(DATA_PTR(self)));
}

static VALUE vec4_norm_inf(VALUE self)
{
    return DBL2NUM(glm_vec4_norm_inf(DATA_PTR(self)));
}

static VALUE vec4_normalize(VALUE self)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_normalize_to(DATA_PTR(self), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_normalize_bang(VALUE self)
{
    glm_vec4_normalize(DATA_PTR(self));
    return self;
}

static VALUE vec4_distance(VALUE self, VALUE other)
{
    return DBL2NUM(glm_vec4_distance(DATA_PTR(self), DATA_PTR(other)));
}

static VALUE vec4_distance_squared(VALUE self, VALUE other)
{
    return DBL2NUM(glm_vec4_distance2(DATA_PTR(self), DATA_PTR(other)));
}

static VALUE vec4_maxv(VALUE self, VALUE other)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_maxv(DATA_PTR(self), DATA_PTR(other), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_minv(VALUE self, VALUE other)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_minv(DATA_PTR(self), DATA_PTR(other), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_clamp_bang(VALUE self, VALUE min, VALUE max)
{
    glm_vec4_clamp(DATA_PTR(self), NUM2FLT(min), NUM2FLT(max));
    return self;
}

static VALUE vec4_clamp(VALUE self, VALUE min, VALUE max)
{
    return vec4_clamp(vec4_copy(self), min, max);
}

static VALUE vec4_lerp_bang(int argc, VALUE *argv, VALUE self)
{
    VALUE other, weight, clamped;
    rb_scan_args(argc, argv, "21", &other, &weight, &clamped);

    float *v = DATA_PTR(self);

    if (RTEST(clamped))
    {
        glm_vec4_lerpc(v, DATA_PTR(other), NUM2FLT(weight), v);
    }
    else
    {
        glm_vec4_lerp(v, DATA_PTR(other), NUM2FLT(weight), v);
    }
    return self;
}

static VALUE vec4_lerp(int argc, VALUE *argv, VALUE self)
{
    VALUE other, weight, clamped;
    rb_scan_args(argc, argv, "21", &other, &weight, &clamped);

    float *d = RGSS_VEC4_NEW;
    float *v = DATA_PTR(self);

    if (RTEST(clamped))
    {
        glm_vec4_lerpc(v, DATA_PTR(other), NUM2FLT(weight), d);
    }
    else
    {
        glm_vec4_lerp(v, DATA_PTR(other), NUM2FLT(weight), d);
    }
    return VEC4_WRAP(d);
}

static VALUE vec4_step(VALUE self, VALUE edge)
{
    float *d = RGSS_VEC4_NEW;
    float *v = DATA_PTR(self);

    if (RB_TYPE_P(edge, T_DATA))
    {
        glm_vec4_step(DATA_PTR(edge), v, d);
    }
    else
    {
        glm_vec4_step_uni(NUM2FLT(edge), v, d);
    }

    return VEC4_WRAP(d);
}

static VALUE vec4_smoothstep(VALUE self, VALUE low, VALUE high)
{
    float *v = DATA_PTR(self);
    float *d = RGSS_VEC4_NEW;

    if (RB_TYPE_P(low, T_DATA) && RB_TYPE_P(high, T_DATA))
    {
        glm_vec4_smoothstep(v, DATA_PTR(low), DATA_PTR(high), d);
    }
    else
    {
        glm_vec4_smoothstep_uni(NUM2FLT(low), NUM2FLT(high), v, d);
    }

    return VEC4_WRAP(d);
}

static VALUE vec4_smoothinterp(int argc, VALUE *argv, VALUE self)
{
    VALUE other, weight, clamped;
    rb_scan_args(argc, argv, "21", &other, &weight, &clamped);

    float *from = DATA_PTR(self), *to = DATA_PTR(other);
    float w = NUM2FLT(weight);
    float *d = RGSS_VEC4_NEW;

    if (RTEST(clamped))
    {
        glm_vec4_smoothinterpc(from, to, w, d);
    }
    else
    {
        glm_vec4_smoothinterp(from, to, w, d);
    }
    return VEC4_WRAP(d);
}

static VALUE vec4_swizzle(VALUE self, VALUE mask)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_swizzle(DATA_PTR(self), NUM2INT(mask), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_swizzle_bang(VALUE self, VALUE mask)
{
    vec4 temp;
    glm_vec4_swizzle(DATA_PTR(self), NUM2INT(mask), temp);
    glm_vec4_copy(temp, DATA_PTR(self));
    return self;
}

static VALUE vec4_inspect(VALUE self)
{
    float *v = DATA_PTR(self);
    return rb_sprintf("<%f, %f, %f, %f>", v[0], v[1], v[2], v[3]);
}

static VALUE vec4_cubic(VALUE klass, VALUE parameter)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_cubic(NUM2FLT(parameter), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_equal(int argc, VALUE *argv, VALUE self)
{
    VALUE other, epsilon;
    rb_scan_args(argc, argv, "11", &other, &epsilon);

    if (CLASS_OF(self) != CLASS_OF(other))
        return Qfalse;

    float *v1 = DATA_PTR(self), *v2 = DATA_PTR(other);
    bool result = RTEST(epsilon) ? glm_vec4_eqv_eps(v1, v2) : glm_vec4_eqv(v1, v2);

    return RB_BOOL(result);
}

static VALUE vec4_all_p(int argc, VALUE *argv, VALUE self)
{
    VALUE value, epsilon;
    rb_scan_args(argc, argv, "11", &value, &epsilon);

    float *v1 = DATA_PTR(self), v2 = NUM2FLT(value);
    bool result = RTEST(epsilon) ? glm_vec4_eq_eps(v1, v2) : glm_vec4_eq(v1, v2);

    return RB_BOOL(result);
}

static VALUE vec4_max_value(VALUE self)
{
    return DBL2NUM(glm_vec4_max(DATA_PTR(self)));
}

static VALUE vec4_min_value(VALUE self)
{
    return DBL2NUM(glm_vec4_min(DATA_PTR(self)));
}

static VALUE vec4_infinite_p(VALUE self)
{
    return RB_BOOL(glm_vec4_isinf(DATA_PTR(self)));
}

static VALUE vec4_nan_p(VALUE self)
{
    return RB_BOOL(glm_vec4_isnan(DATA_PTR(self)));
}

static VALUE vec4_sqrt(VALUE self)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_sqrt(DATA_PTR(self), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_abs(VALUE self)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_abs(DATA_PTR(self), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_sum(VALUE self)
{
    return DBL2NUM(glm_vec4_hadd(DATA_PTR(self)));
}

static VALUE vec4_sign(VALUE self)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_sign(DATA_PTR(self), d);
    return VEC4_WRAP(d);
}

static VALUE vec4_fract(VALUE self)
{
    float *d = RGSS_VEC4_NEW;
    glm_vec4_fract(DATA_PTR(self), d);
    return VEC4_WRAP(d);
}

static inline void vec4_create(int argc, VALUE *argv, vec4 v)
{
    VALUE x, y, z, w;
    rb_scan_args(argc, argv, "04", &x, &y, &z, &w);

    switch (argc)
    {
        case 0:
        {
            // vec4()
            glm_vec4_zero(v);
            break;
        }
        case 1:
        {
            if (RB_TYPE_P(x, T_DATA))
            {
                // vec4(v4)
                glm_vec4_copy(DATA_PTR(x), v);
            }
            else
            {
                // vec4(float)
                glm_vec4_fill(v, NUM2FLT(x));
            }
            break;
        }
        case 2:
        {
            if (RB_TYPE_P(x, T_DATA))
            {
                if (RB_TYPE_P(y, T_DATA))
                {
                    // vec4(vec2, vec2)
                    float *v1 = DATA_PTR(x), *v2 = DATA_PTR(y);
                    v[0] = v1[0];
                    v[1] = v1[1];
                    v[2] = v2[0];
                    v[3] = v2[1];
                }
                else
                {
                    // vec4(vec3, float)
                    glm_vec4(DATA_PTR(x), NUM2FLT(y), v);
                }
            }
            else
            {
                // vec4(float, vec3)
                float *v3 = DATA_PTR(y);
                v[0] = NUM2FLT(x);
                v[1] = v3[0];
                v[2] = v3[1];
                v[3] = v3[2];                
            }
            break;
        }
        case 3:
        {
            float *v2;
            if (RB_TYPE_P(x, T_DATA))
            {
                // vec4(vec2, float, float)
                v2 = DATA_PTR(x);
                v[0] = v2[0];
                v[1] = v2[1];
                v[2] = NUM2FLT(y);
                v[3] = NUM2FLT(z);                
            }
            else if (RB_TYPE_P(y, T_DATA))
            {
                // vec4(float, vec2, float)
                v2 = DATA_PTR(y);
                v[0] = NUM2FLT(x);
                v[1] = v2[0];
                v[2] = v2[1];
                v[3] = NUM2FLT(z);  
            }
            else
            {
                // vec4(float, float, vec2)
                v2 = DATA_PTR(z);
                v[0] = NUM2FLT(x);
                v[1] = NUM2FLT(y);
                v[2] = v2[0];
                v[3] = v2[1];  
            }
            break;
        }
        case 4:
        {
            // vec3(float, float, float, float)
            v[0] = NUM2FLT(x);   
            v[1] = NUM2FLT(y);   
            v[2] = NUM2FLT(z);             
            v[3] = NUM2FLT(w);               
            break;
        }
    }
}

static VALUE vec4_initialize(int argc, VALUE *argv, VALUE self)
{
    if (CLASS_OF(self) != rb_cVec4)
        rb_warn("inheriting from %s may result in undefined behavior", rb_class2name(rb_cVec4));
        
    vec4_create(argc, argv, DATA_PTR(self));
    return Qnil;
}

static VALUE vec4_kernel_create(int argc, VALUE *argv, VALUE module)
{
    float *v = RGSS_VEC4_NEW;
    vec4_create(argc, argv, v);
    return VEC4_WRAP(v);
}

void RGSS_Init_Vec4(VALUE parent)
{
    rb_cVec4 = rb_define_class_under(parent, "Vec4", rb_cObject);
    rb_define_alloc_func(rb_cVec4, vec4_alloc);
    rb_define_methodm1(rb_cVec4, "initialize", vec4_initialize, -1);
    rb_define_methodm1(rb_mKernel, "vec4", vec4_kernel_create, -1);

    rb_define_const(rb_cVec4, "SIZE", SIZET2NUM(RGSS_VEC4_SIZE));
    rb_define_const(rb_cVec4, "ALIGN", SIZET2NUM(RGSS_VEC4_ALIGN));
    rb_define_const(rb_cVec4, "MASK_XXXX", INT2NUM(GLM_XXXX));
    rb_define_const(rb_cVec4, "MASK_YYYY", INT2NUM(GLM_YYYY));
    rb_define_const(rb_cVec4, "MASK_ZZZZ", INT2NUM(GLM_ZZZZ));
    rb_define_const(rb_cVec4, "MASK_WWWW", INT2NUM(GLM_WWWW));
    rb_define_const(rb_cVec4, "MASK_WZYX", INT2NUM(GLM_WZYX));

    rb_define_method1(rb_cVec4, "[]", vec4_get, 1);
    rb_define_method2(rb_cVec4, "[]=", vec4_set, 2);
    rb_define_method0(rb_cVec4, "x", vec4_get_x, 0);
    rb_define_method0(rb_cVec4, "y", vec4_get_y, 0);
    rb_define_method0(rb_cVec4, "z", vec4_get_z, 0);
    rb_define_method0(rb_cVec4, "w", vec4_get_w, 0);
    rb_define_method1(rb_cVec4, "x=", vec4_set_x, 1);
    rb_define_method1(rb_cVec4, "y=", vec4_set_y, 1);
    rb_define_method1(rb_cVec4, "z=", vec4_set_z, 1);
    rb_define_method1(rb_cVec4, "w=", vec4_set_w, 1);
    rb_define_method0(rb_cVec4, "copy", vec4_copy, 0);
    rb_define_method1(rb_cVec4, "dot", vec4_dot, 1);
    rb_define_method1(rb_cVec4, "add", vec4_add, 1);
    rb_define_method1(rb_cVec4, "subtract", vec4_sub, 1);
    rb_define_method1(rb_cVec4, "multiply", vec4_mul, 1);
    rb_define_method1(rb_cVec4, "divide", vec4_div, 1);
    rb_define_method1(rb_cVec4, "norm_scale", vec4_norm_scale, 1);
    rb_define_method1(rb_cVec4, "add_add", vec4_add_add, 1);
    rb_define_method1(rb_cVec4, "sub_add", vec4_sub_add, 1);
    rb_define_method1(rb_cVec4, "mul_add", vec4_mul_add, 1);
    rb_define_method1(rb_cVec4, "max_add", vec4_max_add, 1);
    rb_define_method1(rb_cVec4, "min_add", vec4_min_add, 1);
    rb_define_method0(rb_cVec4, "length", vec4_length, 0);
    rb_define_method0(rb_cVec4, "length_squared", vec4_length_squared, 0);
    rb_define_method0(rb_cVec4, "negate", vec4_negate, 0);
    rb_define_method0(rb_cVec4, "negate!", vec4_negate_bang, 0);
    rb_define_method0(rb_cVec4, "manhattan_distance", vec4_manhattan_distance, 0);
    rb_define_method0(rb_cVec4, "norm_inf", vec4_norm_inf, 0);
    rb_define_method0(rb_cVec4, "normalize", vec4_normalize, 0);
    rb_define_method0(rb_cVec4, "normalize!", vec4_normalize_bang, 0);
    rb_define_method1(rb_cVec4, "distance", vec4_distance, 1);
    rb_define_method1(rb_cVec4, "distance_squared", vec4_distance_squared, 1);
    rb_define_method1(rb_cVec4, "max", vec4_maxv, 1);
    rb_define_method1(rb_cVec4, "min", vec4_minv, 1);
    rb_define_method2(rb_cVec4, "clamp", vec4_clamp, 2);
    rb_define_method2(rb_cVec4, "clamp!", vec4_clamp_bang, 2);
    rb_define_methodm1(rb_cVec4, "lerp", vec4_lerp, -1);
    rb_define_methodm1(rb_cVec4, "lerp!", vec4_lerp_bang, -1);
    rb_define_methodm1(rb_cVec4, "smoothinterp", vec4_smoothinterp, -1);
    rb_define_method1(rb_cVec4, "step", vec4_step, 1);
    rb_define_method2(rb_cVec4, "smoothstep", vec4_smoothstep, 2);
    rb_define_method1(rb_cVec4, "swizzle", vec4_swizzle, 1);
    rb_define_method1(rb_cVec4, "swizzle!", vec4_swizzle_bang, 1);
    rb_define_method0(rb_cVec4, "inspect", vec4_inspect, 0);
    rb_define_method0(rb_cVec4, "to_s", vec4_inspect, 0);
    rb_define_method0(rb_cVec4, "to_str", vec4_inspect, 0);
    rb_define_methodm1(rb_cVec4, "eql?", vec4_equal, -1);
    rb_define_methodm1(rb_cVec4, "all?", vec4_all_p, -1);
    rb_define_method0(rb_cVec4, "max_value", vec4_max_value, 0);
    rb_define_method0(rb_cVec4, "min_value", vec4_min_value, 0);
    rb_define_method0(rb_cVec4, "infinite?", vec4_infinite_p, 0);
    rb_define_method0(rb_cVec4, "nan?", vec4_nan_p, 0);
    rb_define_method0(rb_cVec4, "sqrt", vec4_sqrt, 0);
    rb_define_method0(rb_cVec4, "abs", vec4_abs, 0);
    rb_define_method0(rb_cVec4, "sum", vec4_sum, 0);
    rb_define_method0(rb_cVec4, "sign", vec4_sign, 0);
    rb_define_method0(rb_cVec4, "fract", vec4_fract, 0);

    rb_define_alias(rb_cVec4, "dup", "copy");
    rb_define_alias(rb_cVec4, "+", "add");
    rb_define_alias(rb_cVec4, "-", "subtract");
    rb_define_alias(rb_cVec4, "*", "multiply");
    rb_define_alias(rb_cVec4, "/", "divide");
    rb_define_alias(rb_cVec4, "==", "eql?");
    rb_define_alias(rb_cVec4, "norm", "length");
    rb_define_alias(rb_cVec4, "norm2", "length_squared");
    rb_define_alias(rb_cVec4, "scale", "multiply");
    rb_define_alias(rb_cVec4, "-@", "negate");
    rb_define_alias(rb_cVec4, "norm_one", "manhattan_distance");
    rb_define_alias(rb_cVec4, "taxicab_norm", "manhattan_distance");
    rb_define_alias(rb_cVec4, "max_norm", "norm_inf");
    rb_define_alias(rb_cVec4, "mix", "lerp");
    rb_define_alias(rb_cVec4, "mix!", "lerp!");

    rb_define_singleton_method0(rb_cVec4, "zero", vec4_zero, 0);
    rb_define_singleton_method0(rb_cVec4, "one", vec4_one, 0);
    rb_define_singleton_method0(rb_cVec4, "unit_x", vec4_unitx, 0);
    rb_define_singleton_method0(rb_cVec4, "unit_y", vec4_unity, 0);
    rb_define_singleton_method0(rb_cVec4, "unit_z", vec4_unitz, 0);
    rb_define_singleton_method0(rb_cVec4, "unit_w", vec4_unitw, 0);
    rb_define_singleton_method1(rb_cVec4, "cubic", vec4_cubic, 1);
}