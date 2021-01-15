#include "game.h"

VALUE rb_cVec2;

CREATE_ALLOC_FUNC(vec2, RGSS_VEC2_ALIGN, RGSS_VEC2_SIZE)
VEC_ATTR_ACCESSOR(vec2, x, 0)
VEC_ATTR_ACCESSOR(vec2, y, 1)

static VALUE vec2_get(VALUE self, VALUE index)
{
    int i = NUM2INT(index);
    if (index < 0 || index > 1)
        rb_raise(rb_eArgError, "index of of range (must be 0..1)");
    return DBL2NUM(((float*) DATA_PTR(self))[i]);
}

static VALUE vec2_set(VALUE self, VALUE index, VALUE value)
{
    int i = NUM2INT(index);
    if (index < 0 || index > 1)
        rb_raise(rb_eArgError, "index of of range (must be 0..1)");
    ((float*) DATA_PTR(self))[i] = NUM2FLT(value);
    return value;
}

static VALUE vec2_copy(VALUE self)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_copy(DATA_PTR(self), d);
    return VEC2_WRAP(d);
}

static VALUE vec2_zero(VALUE klass)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_zero(d);
    return VEC2_WRAP(d);
}

static VALUE vec2_one(VALUE klass)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_one(d);
    return VEC2_WRAP(d);
}

static VALUE vec2_dot(VALUE self, VALUE other)
{
    return DBL2NUM(glm_vec2_dot(DATA_PTR(self), DATA_PTR(other)));
}

static VALUE vec2_cross(VALUE self, VALUE other)
{
    return DBL2NUM(glm_vec2_cross(DATA_PTR(self), DATA_PTR(other)));
}

static VALUE vec2_length(VALUE self)
{
    return DBL2NUM(glm_vec2_norm(DATA_PTR(self)));
}

static VALUE vec2_length_squared(VALUE self)
{
    return DBL2NUM(glm_vec2_norm2(DATA_PTR(self)));
}

static VALUE vec2_add(VALUE self, VALUE other)
{
    float *v = DATA_PTR(self);
    float *d = RGSS_VEC2_NEW;

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec2_add(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec2_adds(v, NUM2FLT(other), d);
    }
    
    return VEC2_WRAP(d);
}

static VALUE vec2_sub(VALUE self, VALUE other)
{
    float *v = DATA_PTR(self);
    float *d = RGSS_VEC2_NEW;

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec2_sub(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec2_subs(v, NUM2FLT(other), d);
    }
    
    return VEC2_WRAP(d);
}

static VALUE vec2_mul(VALUE self, VALUE other)
{
    float *v = DATA_PTR(self);
    float *d = RGSS_VEC2_NEW;

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec2_mul(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec2_scale(v, NUM2FLT(other), d);
    }
    
    return VEC2_WRAP(d);
}

static VALUE vec2_div(VALUE self, VALUE other)
{
    float *v = DATA_PTR(self);
    float *d = RGSS_VEC2_NEW;

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec2_div(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec2_divs(v, NUM2FLT(other), d);
    }
    
    return VEC2_WRAP(d);
}

static VALUE vec2_norm_scale(VALUE self, VALUE scalar)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_scale_as(DATA_PTR(self), NUM2FLT(scalar), d);
    return VEC2_WRAP(d);
}

static VALUE vec2_add_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_addadd(DATA_PTR(self), DATA_PTR(other), d);
    return VEC2_WRAP(d);
}

static VALUE vec2_sub_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_subadd(DATA_PTR(self), DATA_PTR(other), d);
    return VEC2_WRAP(d);
}

static VALUE vec2_mul_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC2_NEW;
    float *v = DATA_PTR(self);

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec2_muladd(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec2_muladds(v, NUM2FLT(other), d);
    }
    return VEC2_WRAP(d);
}

static VALUE vec2_max_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_maxadd(DATA_PTR(self), DATA_PTR(other), d);
    return VEC2_WRAP(d);
}

static VALUE vec2_min_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_minadd(DATA_PTR(self), DATA_PTR(other), d);
    return VEC2_WRAP(d);
}

static VALUE vec2_negate_bang(VALUE self)
{
    glm_vec2_negate(DATA_PTR(self));
    return self;
}

static VALUE vec2_negate(VALUE self)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_negate_to(DATA_PTR(self), d);
    return VEC2_WRAP(d);
}

static VALUE vec2_normalize(VALUE self)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_normalize_to(DATA_PTR(self), d);
    return VEC2_WRAP(d);
}

static VALUE vec2_normalize_bang(VALUE self)
{
    glm_vec2_normalize(DATA_PTR(self));
    return self;
}

static VALUE vec2_rotate_bang(VALUE self, VALUE angle)
{
    float *v = DATA_PTR(self);
    vec2 temp;
    glm_vec2_rotate(v, NUM2FLT(angle), v);
    return self;
}

static VALUE vec2_rotate(VALUE self, VALUE angle)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_rotate(DATA_PTR(self), NUM2FLT(angle), d);
    return VEC2_WRAP(d);
}

static VALUE vec2_distance(VALUE self, VALUE other)
{
    return DBL2NUM(glm_vec2_distance(DATA_PTR(self), DATA_PTR(other)));
}

static VALUE vec2_distance_squared(VALUE self, VALUE other)
{
    return DBL2NUM(glm_vec2_distance2(DATA_PTR(self), DATA_PTR(other)));
}

static VALUE vec2_maxv(VALUE self, VALUE other)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_maxv(DATA_PTR(self), DATA_PTR(other), d);
    return VEC2_WRAP(d);
}

static VALUE vec2_minv(VALUE self, VALUE other)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_minv(DATA_PTR(self), DATA_PTR(other), d);
    return VEC2_WRAP(d);
}

static VALUE vec2_clamp_bang(VALUE self, VALUE min, VALUE max)
{
    glm_vec2_clamp(DATA_PTR(self), NUM2FLT(min), NUM2FLT(max));
    return self;
}

static VALUE vec2_clamp(VALUE self, VALUE min, VALUE max)
{
    return vec2_clamp(vec2_copy(self), min, max);
}

static VALUE vec2_lerp(VALUE self, VALUE other, VALUE weight)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_lerp(DATA_PTR(self), DATA_PTR(other), NUM2FLT(weight), d);
    return VEC2_WRAP(d);
}

static VALUE vec2_lerp_bang(VALUE self, VALUE other, VALUE weight)
{
    vec2 temp;
    float *v = DATA_PTR(self);
    glm_vec2_lerp(v, DATA_PTR(other), NUM2FLT(weight), temp);
    glm_vec2_copy(temp, v);
    return self;
}

static VALUE vec2_equal(int argc, VALUE *argv, VALUE self)
{
    VALUE other, epsilon;
    rb_scan_args(argc, argv, "11", &other, &epsilon);

    if (CLASS_OF(self) != CLASS_OF(other))
        return Qfalse;

    float *v1 = DATA_PTR(self), *v2 = DATA_PTR(other);
    bool result = RTEST(epsilon) ? glm_vec2_eqv_eps(v1, v2) : glm_vec2_eqv(v1, v2);

    return RB_BOOL(result);
}

static VALUE vec2_all_p(int argc, VALUE *argv, VALUE self)
{
    VALUE value, epsilon;
    rb_scan_args(argc, argv, "11", &value, &epsilon);

    float *v1 = DATA_PTR(self), v2 = NUM2FLT(value);
    bool result = RTEST(epsilon) ? glm_vec2_eq_eps(v1, v2) : glm_vec2_eq(v1, v2);

    return RB_BOOL(result);
}

static VALUE vec2_max_value(VALUE self)
{
    return DBL2NUM(glm_vec2_max(DATA_PTR(self)));
}

static VALUE vec2_min_value(VALUE self)
{
    return DBL2NUM(glm_vec2_min(DATA_PTR(self)));
}

static VALUE vec2_infinite_p(VALUE self)
{
    return RB_BOOL(glm_vec2_isinf(DATA_PTR(self)));
}

static VALUE vec2_nan_p(VALUE self)
{
    return RB_BOOL(glm_vec2_isnan(DATA_PTR(self)));
}

static VALUE vec2_sqrt(VALUE self)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_sqrt(DATA_PTR(self), d);
    return VEC2_WRAP(d);
}

static VALUE vec2_abs(VALUE self)
{
    float *d = RGSS_VEC2_NEW;
    float *v = DATA_PTR(self);
    d[0] = fabsf(v[0]);
    d[1] = fabsf(v[1]);
    return VEC2_WRAP(d);
}

static VALUE vec2_sign(VALUE self)
{
    float *d = RGSS_VEC2_NEW;
    glm_vec2_sign(DATA_PTR(self), d);
    return VEC2_WRAP(d);
}

static void vec2_create(int argc, VALUE *argv, vec2 v)
{
    VALUE a1, a2;
    rb_scan_args(argc, argv, "02", &a1, &a2);

    switch (argc)
    {
        case 0:
        {
            glm_vec2_zero(v);
            break;
        }
        case 1:
        {
            if (RB_TYPE_P(a1, T_DATA))
            {
                glm_vec2_copy(DATA_PTR(a1), v);
            }
            else
            {
                glm_vec2_fill(v, NUM2FLT(a1));
            }
            break;
        }
        case 2:
        {
            v[0] = NUM2FLT(a1);    
            v[1] = NUM2FLT(a2);        
            break;
        }
    }
}

static VALUE vec2_initialize(int argc, VALUE *argv, VALUE self)
{
    if (CLASS_OF(self) != rb_cVec2)
        rb_warn("inheriting from %s may result in undefined behavior", rb_class2name(rb_cVec3));

    vec2_create(argc, argv, DATA_PTR(self));
    return Qnil;
}

static VALUE vec2_kernel_create(int argc, VALUE *argv, VALUE module)
{
    float *v = RGSS_VEC2_NEW;
    vec2_create(argc, argv, v);
    return VEC2_WRAP(v);
}

static VALUE vec2_unitx(VALUE klass)
{
    return RGSS_Vec2_New(1.0f, 0.0f);
}

static VALUE vec2_unity(VALUE klass)
{
    return RGSS_Vec2_New(0.0f, 1.0f);
}

static VALUE vec2_inspect(VALUE self)
{
    float *v = DATA_PTR(self);
    return rb_sprintf("<%f, %f>", v[0], v[1]);
}

void RGSS_Init_Vec2(VALUE parent)
{
    rb_cVec2 = rb_define_class_under(parent, "Vec2", rb_cObject);
    rb_define_alloc_func(rb_cVec2, vec2_alloc);
    rb_define_methodm1(rb_cVec2, "initialize", vec2_initialize, -1);
    rb_define_methodm1(rb_mKernel, "vec2", vec2_kernel_create, -1);

    rb_define_const(rb_cVec2, "SIZE", SIZET2NUM(RGSS_VEC2_SIZE));
    rb_define_const(rb_cVec2, "ALIGN", SIZET2NUM(RGSS_VEC2_ALIGN));

    rb_define_method1(rb_cVec2, "[]", vec2_get, 1);
    rb_define_method2(rb_cVec2, "[]=", vec2_set, 2);
    rb_define_method0(rb_cVec2, "x", vec2_get_x, 0);
    rb_define_method0(rb_cVec2, "y", vec2_get_y, 0);
    rb_define_method1(rb_cVec2, "x=", vec2_set_x, 1);
    rb_define_method1(rb_cVec2, "y=", vec2_set_y, 1);
    rb_define_method0(rb_cVec2, "copy", vec2_copy, 0);
    rb_define_method1(rb_cVec2, "dot", vec2_dot, 1);
    rb_define_method1(rb_cVec2, "cross", vec2_cross, 1);
    rb_define_method0(rb_cVec2, "length", vec2_length, 0);
    rb_define_method0(rb_cVec2, "length_squared", vec2_length_squared, 0);
    rb_define_method1(rb_cVec2, "add", vec2_add, 1);
    rb_define_method1(rb_cVec2, "subtract", vec2_sub, 1);
    rb_define_method1(rb_cVec2, "multiply", vec2_mul, 1);
    rb_define_method1(rb_cVec2, "divide", vec2_div, 1);
    rb_define_method1(rb_cVec2, "norm_scale", vec2_norm_scale, 1);
    rb_define_method1(rb_cVec2, "add_add", vec2_add_add, 1);
    rb_define_method1(rb_cVec2, "sub_add", vec2_sub_add, 1);
    rb_define_method1(rb_cVec2, "mul_add", vec2_mul_add, 1);
    rb_define_method1(rb_cVec2, "max_add", vec2_max_add, 1);
    rb_define_method1(rb_cVec2, "min_add", vec2_min_add, 1);
    rb_define_method0(rb_cVec2, "negate", vec2_negate, 0);
    rb_define_method0(rb_cVec2, "negate!", vec2_negate_bang, 0);
    rb_define_method0(rb_cVec2, "normalize", vec2_normalize, 0);
    rb_define_method0(rb_cVec2, "normalize!", vec2_normalize_bang, 0);
    rb_define_method1(rb_cVec2, "rotate",  vec2_rotate, 1);
    rb_define_method1(rb_cVec2, "rotate!", vec2_rotate_bang, 1);
    rb_define_method1(rb_cVec2, "distance",  vec2_distance, 1);
    rb_define_method1(rb_cVec2, "distance_squared", vec2_distance_squared, 1);
    rb_define_method1(rb_cVec2, "max", vec2_maxv, 1);
    rb_define_method1(rb_cVec2, "min", vec2_minv, 1);
    rb_define_method2(rb_cVec2, "clamp", vec2_clamp, 2);
    rb_define_method2(rb_cVec2, "clamp!", vec2_clamp_bang, 2);
    rb_define_method2(rb_cVec2, "lerp", vec2_lerp, 2);
    rb_define_method2(rb_cVec2, "lerp!", vec2_lerp_bang, 2);
    rb_define_method0(rb_cVec2, "infinite?", vec2_infinite_p, 0);
    rb_define_method0(rb_cVec2, "nan?", vec2_nan_p, 0);
    rb_define_method0(rb_cVec2, "sqrt", vec2_sqrt, 0);
    rb_define_method0(rb_cVec2, "abs", vec2_abs, 0);
    rb_define_method0(rb_cVec2, "sign", vec2_sign, 0);
    rb_define_methodm1(rb_cVec2, "eql?", vec2_equal, -1);
    rb_define_methodm1(rb_cVec2, "all?", vec2_all_p, -1);
    rb_define_method0(rb_cVec2, "max_value", vec2_max_value, 0);
    rb_define_method0(rb_cVec2, "min_value", vec2_min_value, 0);

    rb_define_method0(rb_cVec2, "inspect", vec2_inspect, 0);
    rb_define_method0(rb_cVec2, "to_s", vec2_inspect, 0);
    rb_define_method0(rb_cVec2, "to_str", vec2_inspect, 0);

    rb_define_alias(rb_cVec2, "dup", "copy");
    rb_define_alias(rb_cVec2, "magnitude", "length");
    rb_define_alias(rb_cVec2, "magnitude_squared", "length_squared");
    rb_define_alias(rb_cVec2, "norm", "length");
    rb_define_alias(rb_cVec2, "norm2", "length_squared");
    rb_define_alias(rb_cVec2, "+", "add");
    rb_define_alias(rb_cVec2, "-", "subtract");
    rb_define_alias(rb_cVec2, "*", "multiply");
    rb_define_alias(rb_cVec2, "/", "divide");
    rb_define_alias(rb_cVec2, "-@", "negate");
    rb_define_alias(rb_cVec2, "==", "eql?");
    rb_define_alias(rb_cVec2, "scale", "multiply");
    rb_define_alias(rb_cVec2, "mix", "lerp");
    rb_define_alias(rb_cVec2, "mix!", "lerp!");

    rb_define_singleton_method0(rb_cVec2, "zero", vec2_zero, 0);
    rb_define_singleton_method0(rb_cVec2, "one", vec2_one, 0);
    rb_define_singleton_method0(rb_cVec2, "unit_x", vec2_unitx, 0);
    rb_define_singleton_method0(rb_cVec2, "unit_y", vec2_unity, 0);
}