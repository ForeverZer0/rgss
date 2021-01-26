#include "game.h"

VALUE rb_cMat4;

CREATE_ALLOC_FUNC(mat4, RGSS_MAT4_ALIGN, RGSS_MAT4_SIZE)

static VALUE mat4_get(int argc, VALUE *argv, VALUE self)
{
    VALUE x, y;
    rb_scan_args(argc, argv, "11", &x, &y);

    // TODO
    return Qnil;
}

static VALUE mat4_set(int argc, VALUE *argv, VALUE self)
{

    // TODO
    return self;
}

static VALUE mat4_rows(VALUE self)
{
    float *mat = DATA_PTR(self);
    
    VALUE ary = rb_ary_new_capa(4);
    for (long i = 0; i < 4; i++)
    {
        float *v = RGSS_VEC4_NEW;
        glm_vec4_copy(&mat[i * 4], v);
        rb_ary_store(ary, i, RGSS_VEC4_WRAP(v));
    }
    return ary;
}

static VALUE mat4_columns(VALUE self)
{
    float *mat = DATA_PTR(self);
    
    VALUE ary = rb_ary_new_capa(4);
    for (long i = 0; i < 4; i++)
    {
        rb_ary_store(ary, i, RGSS_Vec4_New(
            mat[i + 0], 
            mat[i + 4], 
            mat[i + 8], 
            mat[i + 12]
        ));
    }
    return ary;
}

static VALUE mat4_zero(VALUE klass)
{
    vec4 *mat = RGSS_MAT4_NEW;
    glm_mat4_zero(mat);
    return RGSS_MAT4_WRAP(mat);
}

static VALUE mat4_identity(VALUE klass)
{
    vec4 *mat = RGSS_MAT4_NEW;
    glm_mat4_identity(mat);
    return RGSS_MAT4_WRAP(mat);
}

static VALUE mat4_initialize(VALUE self, VALUE r1, VALUE r2, VALUE r3, VALUE r4)
{
    float *mat = DATA_PTR(self);

    glm_vec4_copy(DATA_PTR(r1), &mat[0]);
    glm_vec4_copy(DATA_PTR(r2), &mat[4]);
    glm_vec4_copy(DATA_PTR(r3), &mat[8]);
    glm_vec4_copy(DATA_PTR(r4), &mat[12]);

    return Qnil;
}

static VALUE mat4_copy(VALUE self)
{
    vec4 *d = RGSS_MAT4_NEW;
    glm_mat4_copy(DATA_PTR(self), d);
    return RGSS_MAT4_WRAP(d);
}

static VALUE mat4_multiply(VALUE self, VALUE other)
{
    
    if (CLASS_OF(other) == rb_cMat4)
    {
        vec4 *dest = RGSS_MAT4_NEW;
        glm_mat4_mul(DATA_PTR(self), DATA_PTR(other), dest);
        return RGSS_MAT4_WRAP(dest);
    }
    else if (CLASS_OF(other) == rb_cVec4)
    {
        float *v = RGSS_VEC4_NEW;
        glm_mat4_mulv(DATA_PTR(self), DATA_PTR(other), v);
        return RGSS_VEC4_WRAP(v);
    }

    rb_raise(rb_eTypeError, "%s is not a Mat4 or Vec4", CLASS_NAME(other));
}

static VALUE mat4_rotate(VALUE self, VALUE pivot, VALUE angle, VALUE axis)
{
    vec4 *mat = DATA_PTR(self);
    vec4 *dst = RGSS_MAT4_NEW;
    glm_mat4_copy(mat, dst);

    float *p = DATA_PTR(pivot);
    float *a = DATA_PTR(axis);
    glm_rotate_at(dst, p, NUM2FLT(angle), a);
    return RGSS_MAT4_WRAP(dst);
}

static VALUE mat4_rotate_bang(VALUE self, VALUE pivot, VALUE angle, VALUE axis)
{
    vec4 *mat = DATA_PTR(self);
    float *p = DATA_PTR(pivot);
    float *a = DATA_PTR(axis);
    glm_rotate_at(mat, p, NUM2FLT(angle), a);
    return self;
}

static VALUE mat4_create_rotation(VALUE self, VALUE pivot, VALUE angle, VALUE axis)
{
    vec4 *mat = DATA_PTR(self);
    vec4 *dst = RGSS_MAT4_NEW;
    glm_mat4_copy(mat, dst);

    float *p = DATA_PTR(pivot);
    float *a = DATA_PTR(axis);
    glm_rotate_atm(dst, p, NUM2FLT(angle), a);
    return RGSS_MAT4_WRAP(dst);
}

static VALUE mat4_create_rotation_bang(VALUE self, VALUE pivot, VALUE angle, VALUE axis)
{
    vec4 *mat = DATA_PTR(self);
    float *p = DATA_PTR(pivot);
    float *a = DATA_PTR(axis);
    glm_rotate_atm(mat, p, NUM2FLT(angle), a);
    return self;
}

static VALUE mat4_scale(VALUE self, VALUE scale)
{
    vec4 *mat = DATA_PTR(self);
    vec4 *dst = RGSS_MAT4_NEW;
    float *vec = DATA_PTR(scale);

    glm_scale_to(mat, vec, dst);
    return RGSS_MAT4_WRAP(dst);
}

static VALUE mat4_scale_bang(VALUE self, VALUE scale)
{
    vec4 *mat = DATA_PTR(self);
    float *vec = DATA_PTR(scale);
    glm_scale(mat, vec);
    return self;
}

static VALUE mat4_translate(VALUE self, VALUE translation)
{
    vec4 *mat = DATA_PTR(self);
    vec4 *dst = RGSS_MAT4_NEW;
    float *vec = DATA_PTR(translation);
    glm_translate_to(mat, vec, dst);
    return RGSS_MAT4_WRAP(dst);
}

static VALUE mat4_translate_bang(VALUE self, VALUE translation)
{
    vec4 *mat = DATA_PTR(self);
    float *vec = DATA_PTR(translation);
    glm_translate(mat, vec);
    return self;
}

static VALUE mat4_inspect(VALUE self)
{
    float *mat = DATA_PTR(self);
    return rb_sprintf("[<%f, %f, %f, %f>, <%f, %f, %f, %f>, <%f, %f, %f, %f>, <%f, %f, %f, %f>]",
        mat[0], mat[1], mat[2], mat[3],
        mat[4], mat[5], mat[6], mat[7],
        mat[8], mat[9], mat[10], mat[11], 
        mat[12], mat[13], mat[14], mat[15]
    );
}

static VALUE mat4_ortho(VALUE klass, VALUE left, VALUE right, VALUE top, VALUE bottom, VALUE near, VALUE far)
{
    vec4 *mat = RGSS_MAT4_NEW;
    glm_ortho(NUM2FLT(left), NUM2FLT(right), NUM2FLT(top), NUM2FLT(bottom), NUM2FLT(near), NUM2FLT(far), mat);
    return Data_Wrap_Struct(klass, NULL, free, mat);
}

static VALUE mat4_lookat(VALUE klass, VALUE eye, VALUE center, VALUE up)
{
    vec4 *mat = RGSS_MAT4_NEW;
    glm_lookat(DATA_PTR(eye), DATA_PTR(center), DATA_PTR(up), mat);
    return Data_Wrap_Struct(klass, NULL, free, mat);
}

void RGSS_Init_Mat4(VALUE parent)
{
    rb_cMat4 = rb_define_class_under(parent, "Mat4", rb_cObject);
    rb_define_alloc_func(rb_cMat4, mat4_alloc);

    rb_define_methodm1(rb_cMat4, "[]", mat4_get, -1);
    rb_define_methodm1(rb_cMat4, "[]=", mat4_set, -1);

    rb_define_method4(rb_cMat4, "initialize", mat4_initialize, 4);
    rb_define_method0(rb_cMat4, "inspect", mat4_inspect, 0);
    rb_define_method0(rb_cMat4, "rows", mat4_rows, 0);
    rb_define_method0(rb_cMat4, "columns", mat4_columns, 0);
    rb_define_method0(rb_cMat4, "copy", mat4_copy, 0);

    rb_define_method3(rb_cMat4, "rotate", mat4_rotate, 3);
    rb_define_method3(rb_cMat4, "rotate!", mat4_rotate_bang, 3);
    rb_define_method3(rb_cMat4, "create_rotation", mat4_create_rotation, 3);
    rb_define_method3(rb_cMat4, "create_rotation!", mat4_create_rotation_bang, 3);
    rb_define_method1(rb_cMat4, "scale", mat4_scale, 1);
    rb_define_method1(rb_cMat4, "scale!", mat4_scale_bang, 1);
    rb_define_method1(rb_cMat4, "translate", mat4_translate, 1);
    rb_define_method1(rb_cMat4, "translate!", mat4_translate_bang, 1);

    rb_define_alias(rb_cMat4, "dup", "copy");

    rb_define_singleton_method6(rb_cMat4, "ortho", mat4_ortho, 6);
    rb_define_singleton_method3(rb_cMat4, "lookat", mat4_lookat, 3);
    rb_define_singleton_method0(rb_cMat4, "identity", mat4_identity, 0);
    rb_define_singleton_method0(rb_cMat4, "zero", mat4_zero, 0);
}