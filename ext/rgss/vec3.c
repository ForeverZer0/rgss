#include "game.h"

VALUE rb_cVec3;

CREATE_ALLOC_FUNC(vec3, RGSS_VEC3_ALIGN, RGSS_VEC3_SIZE)
VEC_ATTR_ACCESSOR(vec3, x, 0)
VEC_ATTR_ACCESSOR(vec3, y, 1)
VEC_ATTR_ACCESSOR(vec3, z, 2)

static VALUE vec3_get(VALUE self, VALUE index)
{
    int i = NUM2INT(index);
    if (index < 0 || index > 2)
        rb_raise(rb_eArgError, "index of of range (must be 0..2)");
    return DBL2NUM(((float*) DATA_PTR(self))[i]);
}

static VALUE vec3_set(VALUE self, VALUE index, VALUE value)
{
    int i = NUM2INT(index);
    if (index < 0 || index > 2)
        rb_raise(rb_eArgError, "index of of range (must be 0..2)");
    ((float*) DATA_PTR(self))[i] = NUM2FLT(value);
    return value;
}

static VALUE vec3_copy(VALUE self)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_copy(DATA_PTR(self), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_zero(VALUE klass)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_zero(d);
    return VEC3_WRAP(d);
}

static VALUE vec3_one(VALUE klass)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_one(d);
    return VEC3_WRAP(d);
}

static VALUE vec3_dot(VALUE self, VALUE other)
{
    return DBL2NUM(glm_vec3_dot(DATA_PTR(self), DATA_PTR(other)));
}

static VALUE vec3_length(VALUE self)
{
    return DBL2NUM(glm_vec3_norm(DATA_PTR(self)));
}

static VALUE vec3_length_squared(VALUE self)
{
    return DBL2NUM(glm_vec3_norm2(DATA_PTR(self)));
}

static VALUE vec3_manhattan_distance(VALUE self)
{
    return DBL2NUM(glm_vec3_norm_one(DATA_PTR(self)));
}

static VALUE vec3_norm_inf(VALUE self)
{
    return DBL2NUM(glm_vec3_norm_inf(DATA_PTR(self)));
}

static VALUE vec3_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    float *v = DATA_PTR(self);

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec3_add(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec3_adds(v, NUM2FLT(other), d);
    }
    return VEC3_WRAP(d);
}

static VALUE vec3_sub(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    float *v = DATA_PTR(self);

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec3_sub(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec3_subs(v, NUM2FLT(other), d);
    }
    return VEC3_WRAP(d);
}

static VALUE vec3_mul(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    float *v = DATA_PTR(self);

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec3_mul(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec3_scale(v, NUM2FLT(other), d);
    }
    return VEC3_WRAP(d);
}

static VALUE vec3_div(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    float *v = DATA_PTR(self);

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec3_div(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec3_divs(v, NUM2FLT(other), d);
    }
    return VEC3_WRAP(d);
}

static VALUE vec3_norm_scale(VALUE self, VALUE value)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_scale_as(DATA_PTR(self), NUM2FLT(value), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_add_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_addadd(DATA_PTR(self), DATA_PTR(other), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_sub_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_subadd(DATA_PTR(self), DATA_PTR(other), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_mul_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    float *v = DATA_PTR(self);

    if (RB_TYPE_P(other, T_DATA))
    {
        glm_vec3_muladd(v, DATA_PTR(other), d);
    }
    else
    {
        glm_vec3_muladds(v, NUM2FLT(other), d);
    }

    return VEC3_WRAP(d);
}

static VALUE vec3_max_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_maxadd(DATA_PTR(self), DATA_PTR(other), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_min_add(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_minadd(DATA_PTR(self), DATA_PTR(other), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_negate(VALUE self)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_negate_to(DATA_PTR(self), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_negate_bang(VALUE self)
{
    glm_vec3_negate(DATA_PTR(self));
    return self;
}

static VALUE vec3_normalize(VALUE self)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_normalize_to(DATA_PTR(self), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_normalize_bang(VALUE self)
{
    glm_vec3_normalize(DATA_PTR(self));
    return self;
}

static VALUE vec3_cross(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_cross(DATA_PTR(self), DATA_PTR(other), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_cross_norm(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_crossn(DATA_PTR(self), DATA_PTR(other), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_angle(VALUE self, VALUE other)
{
    return DBL2NUM(glm_vec3_angle(DATA_PTR(self), DATA_PTR(other)));
}

static VALUE vec3_rotate_bang(int argc, VALUE *argv, VALUE self)
{
    VALUE a1, a2;
    rb_scan_args(argc, argv, "11", &a1, &a2);

    vec3 d;
    float *v = DATA_PTR(self);

    switch (argc)
    {
        case 2:
        {
            glm_vec3_rotate(v, NUM2FLT(a1), DATA_PTR(a2));
            break;
        }
        case 1:
        {
            VALUE klass = CLASS_OF(a1);
            if (klass == rb_cMat4)
            {
                glm_vec3_rotate_m4(DATA_PTR(a1), v, d);
            }
            else if (klass == rb_cMat3)
            {
                glm_vec3_rotate_m3(DATA_PTR(a1), v, d);
            }
            else
            {
                rb_raise(rb_eTypeError, "expected Mat3 or Mat4, recieved %s", CLASS_NAME(a1));
            }
            glm_vec3_copy(d, v);
        }
    }

    return self;
}

static VALUE vec3_rotate(int argc, VALUE *argv, VALUE self)
{
    VALUE a1, a2;
    rb_scan_args(argc, argv, "11", &a1, &a2);

    float *d = RGSS_VEC3_NEW;
    float *v = DATA_PTR(self);

    switch (argc)
    {
        case 2:
        {
            glm_vec3_copy(v, d);
            glm_vec3_rotate(d, NUM2FLT(a1), DATA_PTR(a2));
            break;
        }
        case 1:
        {
            VALUE klass = CLASS_OF(a1);
            if (klass == rb_cMat4)
            {
                glm_vec3_rotate_m4(DATA_PTR(a1), v, d);
            }
            else if (klass == rb_cMat3)
            {
                glm_vec3_rotate_m3(DATA_PTR(a1), v, d);
            }
            else
            {
                rb_raise(rb_eTypeError, "expected Mat3 or Mat4, recieved %s", CLASS_NAME(a1));
            }
        }
    }
    return VEC3_WRAP(d);
}

static VALUE vec3_project(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_proj(DATA_PTR(self), DATA_PTR(other), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_center(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_center(DATA_PTR(self), DATA_PTR(other), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_distance(VALUE self, VALUE other)
{
    return DBL2NUM(glm_vec3_distance(DATA_PTR(self), DATA_PTR(other)));
}

static VALUE vec3_distance_squared(VALUE self, VALUE other)
{
    return DBL2NUM(glm_vec3_distance2(DATA_PTR(self), DATA_PTR(other)));
}

static VALUE vec3_maxv(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_maxv(DATA_PTR(self), DATA_PTR(other), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_minv(VALUE self, VALUE other)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_minv(DATA_PTR(self), DATA_PTR(other), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_ortho(VALUE self)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_ortho(DATA_PTR(self), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_clamp_bang(VALUE self, VALUE min, VALUE max)
{
    glm_vec3_clamp(DATA_PTR(self), NUM2FLT(min), NUM2FLT(max));
    return self;
}

static VALUE vec3_clamp(VALUE self, VALUE min, VALUE max)
{
    return vec3_clamp(vec3_copy(self), min, max);
}

static VALUE vec3_lerp_bang(int argc, VALUE *argv, VALUE self)
{
    VALUE other, weight, clamped;
    rb_scan_args(argc, argv, "21", &other, &weight, &clamped);

    float *v = DATA_PTR(self);

    if (RTEST(clamped))
    {
        glm_vec3_lerpc(v, DATA_PTR(other), NUM2FLT(weight), v);
    }
    else
    {
        glm_vec3_lerp(v, DATA_PTR(other), NUM2FLT(weight), v);
    }
    return self;
}

static VALUE vec3_lerp(int argc, VALUE *argv, VALUE self)
{
    VALUE other, weight, clamped;
    rb_scan_args(argc, argv, "21", &other, &weight, &clamped);

    float *d = RGSS_VEC3_NEW;
    float *v = DATA_PTR(self);

    if (RTEST(clamped))
    {
        glm_vec3_lerpc(v, DATA_PTR(other), NUM2FLT(weight), d);
    }
    else
    {
        glm_vec3_lerp(v, DATA_PTR(other), NUM2FLT(weight), d);
    }
    return VEC3_WRAP(d);
}

static VALUE vec3_step(VALUE self, VALUE edge)
{
    float *d = RGSS_VEC3_NEW;
    float *v = DATA_PTR(self);

    if (RB_TYPE_P(edge, T_DATA))
    {
        glm_vec3_step(DATA_PTR(edge), v, d);
    }
    else
    {
        glm_vec3_step_uni(NUM2FLT(edge), v, d);
    }

    return VEC3_WRAP(d);
}

static VALUE vec3_smoothstep(VALUE self, VALUE low, VALUE high)
{
    float *v = DATA_PTR(self);
    float *d = RGSS_VEC3_NEW;

    if (RB_TYPE_P(low, T_DATA) && RB_TYPE_P(high, T_DATA))
    {
        glm_vec3_smoothstep(v, DATA_PTR(low), DATA_PTR(high), d);
    }
    else
    {
        glm_vec3_smoothstep_uni(NUM2FLT(low), NUM2FLT(high), v, d);
    }

    return VEC3_WRAP(d);
}

static VALUE vec3_smoothinterp(int argc, VALUE *argv, VALUE self)
{
    VALUE other, weight, clamped;
    rb_scan_args(argc, argv, "21", &other, &weight, &clamped);

    float *from = DATA_PTR(self), *to = DATA_PTR(other);
    float w = NUM2FLT(weight);
    float *d = RGSS_VEC3_NEW;

    if (RTEST(clamped))
    {
        glm_vec3_smoothinterpc(from, to, w, d);
    }
    else
    {
        glm_vec3_smoothinterp(from, to, w, d);
    }
    return VEC3_WRAP(d);
}

static VALUE vec3_swizzle(VALUE self, VALUE mask)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_swizzle(DATA_PTR(self), NUM2INT(mask), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_swizzle_bang(VALUE self, VALUE mask)
{
    vec3 temp;
    glm_vec3_swizzle(DATA_PTR(self), NUM2INT(mask), temp);
    glm_vec3_copy(temp, DATA_PTR(self));
    return self;
}

static VALUE vec3_equal(int argc, VALUE *argv, VALUE self)
{
    VALUE other, epsilon;
    rb_scan_args(argc, argv, "11", &other, &epsilon);

    if (CLASS_OF(self) != CLASS_OF(other))
        return Qfalse;

    float *v1 = DATA_PTR(self), *v2 = DATA_PTR(other);
    bool result = RTEST(epsilon) ? glm_vec3_eqv_eps(v1, v2) : glm_vec3_eqv(v1, v2);

    return RB_BOOL(result);
}

static VALUE vec3_max_value(VALUE self)
{
    return DBL2NUM(glm_vec3_max(DATA_PTR(self)));
}

static VALUE vec3_min_value(VALUE self)
{
    return DBL2NUM(glm_vec3_min(DATA_PTR(self)));
}

static VALUE vec3_infinite_p(VALUE self)
{
    return RB_BOOL(glm_vec3_isinf(DATA_PTR(self)));
}

static VALUE vec3_nan_p(VALUE self)
{
    return RB_BOOL(glm_vec3_isnan(DATA_PTR(self)));
}

static VALUE vec3_sqrt(VALUE self)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_sqrt(DATA_PTR(self), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_abs(VALUE self)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_abs(DATA_PTR(self), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_sum(VALUE self)
{
    return DBL2NUM(glm_vec3_hadd(DATA_PTR(self)));
}

static VALUE vec3_sign(VALUE self)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_sign(DATA_PTR(self), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_fract(VALUE self)
{
    float *d = RGSS_VEC3_NEW;
    glm_vec3_fract(DATA_PTR(self), d);
    return VEC3_WRAP(d);
}

static VALUE vec3_all_p(int argc, VALUE *argv, VALUE self)
{
    VALUE value, epsilon;
    rb_scan_args(argc, argv, "11", &value, &epsilon);

    float *v1 = DATA_PTR(self), v2 = NUM2FLT(value);
    bool result = RTEST(epsilon) ? glm_vec3_eq_eps(v1, v2) : glm_vec3_eq(v1, v2);

    return RB_BOOL(result);
}

static VALUE vec3_unitx(VALUE klass)
{
    return RGSS_Vec3_New(1.0f, 0.0f, 0.0f);
}

static VALUE vec3_unity(VALUE klass)
{
    return RGSS_Vec3_New(0.0f, 1.0f, 0.0f);
}

static VALUE vec3_unitz(VALUE klass)
{
    return RGSS_Vec3_New(0.0f, 0.0f, 1.0f);
}

static VALUE vec3_inspect(VALUE self)
{
    float *v = DATA_PTR(self);
    return rb_sprintf("<%f, %f, %f>", v[0], v[1], v[2]);
}

static inline void vec3_create(int argc, VALUE *argv, vec3 v)
{
    VALUE x, y, z;
    rb_scan_args(argc, argv, "03", &x, &y, &z);

    switch (argc)
    {

    case 0:
    {
        // vec3()
        glm_vec3_zero(v);
        break;
    }

    case 1:
    {
        if (CLASS_OF(x) == rb_cVec4)
        {
            // vec3(v4)
            glm_vec3(DATA_PTR(x), v);
        }
        else if (CLASS_OF(x) == rb_cVec3)
        {
            // vec3(v3)
            glm_vec3_copy(DATA_PTR(x), v);
        }
        else
        {
            // vec3(float)
            glm_vec3_fill(v, NUM2FLT(x));
        }
        break;
    }
    case 2:
    {
        float *v2;
        if (CLASS_OF(x) == rb_cVec2)
        {
            // vec3(v2, float)
            v2 = DATA_PTR(x);
            v[0] = v2[0];
            v[1] = v2[1];
            v[3] = NUM2FLT(y);
        }
        else
        {
            v2 = DATA_PTR(y);
            v[0] = NUM2FLT(x);
            v[1] = v2[0];
            v[2] = v2[1];
        }
        break;
    }
    case 3:
    {
        // vec3(float, float, float)
        v[0] = NUM2FLT(x);
        v[1] = NUM2FLT(y);
        v[2] = NUM2FLT(z);
        break;
    }
    }
}

static VALUE vec3_initialize(int argc, VALUE *argv, VALUE self)
{
    if (CLASS_OF(self) != rb_cVec3)
        rb_warn("inheriting from %s may result in undefined behavior", rb_class2name(rb_cVec3));
        
    vec3_create(argc, argv, DATA_PTR(self));
    return Qnil;
}

static VALUE vec3_kernel_create(int argc, VALUE *argv, VALUE module)
{
    float *v = RGSS_VEC3_NEW;
    vec3_create(argc, argv, v);
    return VEC3_WRAP(v);
}

void RGSS_Init_Vec3(VALUE parent)
{
    rb_cVec3 = rb_define_class_under(parent, "Vec3", rb_cObject);
    rb_define_alloc_func(rb_cVec3, vec3_alloc);
    rb_define_methodm1(rb_cVec3, "initialize", vec3_initialize, -1);
    rb_define_methodm1(rb_mKernel, "vec3", vec3_kernel_create, -1);

    rb_define_const(rb_cVec3, "SIZE", SIZET2NUM(RGSS_VEC3_SIZE));
    rb_define_const(rb_cVec3, "ALIGN", SIZET2NUM(RGSS_VEC3_ALIGN));
    rb_define_const(rb_cVec3, "MASK_XXX", INT2NUM(GLM_XXX));
    rb_define_const(rb_cVec3, "MASK_YYY", INT2NUM(GLM_YYY));
    rb_define_const(rb_cVec3, "MASK_ZZZ", INT2NUM(GLM_ZZZ));
    rb_define_const(rb_cVec3, "MASK_ZYX", INT2NUM(GLM_ZYX));

    rb_define_method1(rb_cVec3, "[]", vec3_get, 1);
    rb_define_method2(rb_cVec3, "[]=", vec3_set, 2);
    rb_define_method0(rb_cVec3, "copy", vec3_copy, 0);
    rb_define_method1(rb_cVec3, "dot", vec3_dot, 1);
    rb_define_method0(rb_cVec3, "length", vec3_length, 0);
    rb_define_method0(rb_cVec3, "length_squared", vec3_length_squared, 0);
    rb_define_method0(rb_cVec3, "manhattan_distance", vec3_manhattan_distance, 0);
    rb_define_method0(rb_cVec3, "norm_inf", vec3_norm_inf, 0);
    rb_define_method1(rb_cVec3, "add", vec3_add, 1);
    rb_define_method1(rb_cVec3, "subtract", vec3_sub, 1);
    rb_define_method1(rb_cVec3, "multiply", vec3_mul, 1);
    rb_define_method1(rb_cVec3, "norm_scale", vec3_norm_scale, 1);
    rb_define_method1(rb_cVec3, "divide", vec3_div, 1);
    rb_define_method1(rb_cVec3, "add_add", vec3_add_add, 1);
    rb_define_method1(rb_cVec3, "sub_add", vec3_sub_add, 1);
    rb_define_method1(rb_cVec3, "mul_add", vec3_mul_add, 1);
    rb_define_method1(rb_cVec3, "max_add", vec3_max_add, 1);
    rb_define_method1(rb_cVec3, "min_add", vec3_min_add, 1);
    rb_define_method0(rb_cVec3, "negate", vec3_negate, 0);
    rb_define_method0(rb_cVec3, "negate!", vec3_negate_bang, 0);
    rb_define_method0(rb_cVec3, "normalize", vec3_normalize, 0);
    rb_define_method0(rb_cVec3, "normalize!", vec3_normalize_bang, 0);
    rb_define_method1(rb_cVec3, "cross", vec3_cross, 1);
    rb_define_method1(rb_cVec3, "cross_norm", vec3_cross_norm, 1);
    rb_define_method1(rb_cVec3, "angle", vec3_angle, 1);
    rb_define_method1(rb_cVec3, "project", vec3_project, 1);
    rb_define_method1(rb_cVec3, "center", vec3_center, 1);
    rb_define_method1(rb_cVec3, "distance", vec3_distance, 1);
    rb_define_method1(rb_cVec3, "distance_squared", vec3_distance_squared, 1);
    rb_define_method1(rb_cVec3, "max", vec3_maxv, 1);
    rb_define_method1(rb_cVec3, "min", vec3_minv, 1);
    rb_define_method2(rb_cVec3, "clamp", vec3_clamp, 2);
    rb_define_method2(rb_cVec3, "clamp!", vec3_clamp_bang, 2);
    rb_define_methodm1(rb_cVec3, "rotate", vec3_rotate, -1);
    rb_define_methodm1(rb_cVec3, "rotate!", vec3_rotate_bang, -1);
    rb_define_methodm1(rb_cVec3, "lerp", vec3_lerp, -1);
    rb_define_methodm1(rb_cVec3, "lerp!", vec3_lerp_bang, -1);
    rb_define_methodm1(rb_cVec3, "smoothinterp", vec3_smoothinterp, -1);
    rb_define_methodm1(rb_cVec3, "eql?", vec3_equal, -1);
    rb_define_methodm1(rb_cVec3, "all?", vec3_all_p, -1);
    rb_define_method1(rb_cVec3, "swizzle", vec3_swizzle, 1);
    rb_define_method1(rb_cVec3, "swizzle!", vec3_swizzle_bang, 1);
    rb_define_method0(rb_cVec3, "max_value", vec3_max_value, 0);
    rb_define_method0(rb_cVec3, "min_value", vec3_min_value, 0);
    rb_define_method0(rb_cVec3, "infinite?", vec3_infinite_p, 0);
    rb_define_method0(rb_cVec3, "nan?", vec3_nan_p, 0);
    rb_define_method1(rb_cVec3, "step", vec3_step, 1);
    rb_define_method2(rb_cVec3, "smoothstep", vec3_smoothstep, 2);
    rb_define_method0(rb_cVec3, "ortho", vec3_ortho, 0);
    rb_define_method0(rb_cVec3, "sqrt", vec3_sqrt, 0);
    rb_define_method0(rb_cVec3, "abs", vec3_abs, 0);
    rb_define_method0(rb_cVec3, "sum", vec3_sum, 0);
    rb_define_method0(rb_cVec3, "sign", vec3_sign, 0);
    rb_define_method0(rb_cVec3, "fract", vec3_fract, 0);
    rb_define_method0(rb_cVec3, "inspect", vec3_inspect, 0);
    rb_define_method0(rb_cVec3, "to_s", vec3_inspect, 0);
    rb_define_method0(rb_cVec3, "to_str", vec3_inspect, 0);

    rb_define_method0(rb_cVec3, "x", vec3_get_x, 0);
    rb_define_method0(rb_cVec3, "y", vec3_get_y, 0);
    rb_define_method0(rb_cVec3, "z", vec3_get_z, 0);
    rb_define_method1(rb_cVec3, "x=", vec3_set_x, 1);
    rb_define_method1(rb_cVec3, "y=", vec3_set_y, 1);
    rb_define_method1(rb_cVec3, "z=", vec3_set_z, 1);

    rb_define_singleton_method0(rb_cVec3, "zero", vec3_zero, 0);
    rb_define_singleton_method0(rb_cVec3, "one", vec3_one, 0);
    rb_define_singleton_method0(rb_cVec3, "unit_x", vec3_unitx, 0);
    rb_define_singleton_method0(rb_cVec3, "unit_y", vec3_unity, 0);
    rb_define_singleton_method0(rb_cVec3, "unit_z", vec3_unitz, 0);

    rb_define_alias(rb_cVec3, "dup", "copy");
    rb_define_alias(rb_cVec3, "magnitude", "length");
    rb_define_alias(rb_cVec3, "magnitude_squared", "length_squared");
    rb_define_alias(rb_cVec3, "norm", "length");
    rb_define_alias(rb_cVec3, "norm2", "length_squared");
    rb_define_alias(rb_cVec3, "norm_one", "manhattan_distance");
    rb_define_alias(rb_cVec3, "taxicab_norm", "manhattan_distance");
    rb_define_alias(rb_cVec3, "max_norm", "norm_inf");
    rb_define_alias(rb_cVec3, "+", "add");
    rb_define_alias(rb_cVec3, "-", "subtract");
    rb_define_alias(rb_cVec3, "*", "multiply");
    rb_define_alias(rb_cVec3, "/", "divide");
    rb_define_alias(rb_cVec3, "scale", "multiply");
    rb_define_alias(rb_cVec3, "-@", "negate");
    rb_define_alias(rb_cVec3, "==", "eql?");
    rb_define_alias(rb_cVec3, "mix", "lerp");
    rb_define_alias(rb_cVec3, "mix!", "lerp!");
}