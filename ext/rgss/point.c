#include "rgss.h"

VALUE rb_cIVec2;
VALUE rb_cPoint;
VALUE rb_cSize;

#define IVEC_SIZE (sizeof(int) * 2)

RGSS_DEFINE_MARSHAL(RGSS_Point)

static VALUE RGSS_IVec_Alloc(VALUE klass)
{
    int *vec = xmalloc(IVEC_SIZE);
    memset(vec, 0, IVEC_SIZE);
    return Data_Wrap_Struct(klass, NULL, RUBY_DEFAULT_FREE, vec);
}

static VALUE RGSS_IVec_GetX(VALUE self)
{
    return INT2NUM(((int *)DATA_PTR(self))[0]);
}

static VALUE RGSS_IVec_GetY(VALUE self)
{
    return INT2NUM(((int *)DATA_PTR(self))[1]);
}

static VALUE RGSS_IVec_SetX(VALUE self, VALUE value)
{
    RGSS_ASSERT_UNFROZEN(self);
    int *vec = DATA_PTR(self);
    vec[0] = NUM2INT(value);
    return value;
}

static VALUE RGSS_IVec_SetY(VALUE self, VALUE value)
{
    RGSS_ASSERT_UNFROZEN(self);
    int *vec = DATA_PTR(self);
    vec[1] = NUM2INT(value);
    return value;
}

static VALUE RGSS_IVec_Equal(VALUE self, VALUE other)
{
    if (CLASS_OF(self) != CLASS_OF(other))
        return Qfalse;

    int *v1 = DATA_PTR(self), *v2 = DATA_PTR(other);
    return RB_BOOL(v1[0] == v2[0] && v1[1] == v2[1]);
}

static VALUE RGSS_IVec_GetIndex(VALUE self, VALUE index)
{
    int i = NUM2INT(index);
    if (i < 0 || i > 1)
        return Qnil;
    return INT2NUM(((int *)DATA_PTR(self))[i]);
}

static VALUE RGSS_IVec_SetIndex(VALUE self, VALUE index, VALUE value)
{
    RGSS_ASSERT_UNFROZEN(self);
    int i = NUM2INT(index);
    if (i < 0 || i > 1)
        rb_raise(rb_eArgError, "index out of range (given %d, expect 0..1)", i);

    int *vec = DATA_PTR(self);
    vec[i] = NUM2INT(value);
    return value;
}

static VALUE RGSS_IVec_IsEmpty(VALUE self)
{
    int *i = DATA_PTR(self);
    return RB_BOOL(i[0] == 0 && i[1] == 0);
}

static VALUE RGSS_IVec_Add(VALUE self, VALUE other)
{
    int *v1 = DATA_PTR(self), *v2 = DATA_PTR(other), *dst = xmalloc(IVEC_SIZE);

    dst[0] = v1[0] + v2[0];
    dst[1] = v1[1] + v2[1];
    return Data_Wrap_Struct(CLASS_OF(self), NULL, RUBY_DEFAULT_FREE, dst);
}

static VALUE RGSS_IVec_Subtract(VALUE self, VALUE other)
{
    int *v1 = DATA_PTR(self), *v2 = DATA_PTR(other), *dst = xmalloc(IVEC_SIZE);

    dst[0] = v1[0] - v2[0];
    dst[1] = v1[1] - v2[1];
    return Data_Wrap_Struct(CLASS_OF(self), NULL, RUBY_DEFAULT_FREE, dst);
}

static VALUE RGSS_IVec_Clone(VALUE self)
{
    int *src = DATA_PTR(self), *dst = xmalloc(IVEC_SIZE);
    memcpy(dst, src, IVEC_SIZE);

    VALUE value = Data_Wrap_Struct(CLASS_OF(self), NULL, RUBY_DEFAULT_FREE, dst);
    if (rb_obj_frozen_p(self))
        rb_obj_freeze(value);

    return value;
}

static VALUE RGSS_IVec_Dupe(VALUE self)
{
    int *src = DATA_PTR(self), *dst = xmalloc(IVEC_SIZE);
    memcpy(dst, src, IVEC_SIZE);
    return Data_Wrap_Struct(CLASS_OF(self), NULL, RUBY_DEFAULT_FREE, dst);
}

static VALUE RGSS_IVec_OffsetBang(int argc, VALUE *argv, VALUE self)
{
    RGSS_ASSERT_UNFROZEN(self);
    VALUE a1, a2;
    rb_scan_args(argc, argv, "11", &a1, &a2);

    int *vec = DATA_PTR(self);
    int x, y;

    if (argc == 1)
    {
        int *v = DATA_PTR(a1);
        x = v[0];
        y = v[1];
    }
    else
    {
        x = NUM2INT(a1);
        y = NUM2INT(a2);
    }

    vec[0] += x;
    vec[1] += y;
    return self;
}

static VALUE RGSS_IVec_Offset(int argc, VALUE *argv, VALUE self)
{
    return RGSS_IVec_OffsetBang(argc, argv, RGSS_IVec_Dupe(self));
}

static VALUE RGSS_IVec_Inspect(VALUE self)
{
    int *vec = DATA_PTR(self);
    return rb_sprintf("<%d, %d>", vec[0], vec[1]);
}

static VALUE RGSS_IVec_Set(VALUE self, VALUE x, VALUE y)
{
    RGSS_ASSERT_UNFROZEN(self);
    int *vec = DATA_PTR(self);
    vec[0] = NUM2INT(x);
    vec[1] = NUM2INT(y);
    return self;
}

static VALUE RGSS_IVec_ToArray(VALUE self)
{
    int *vec = DATA_PTR(self);
    return rb_ary_new_from_args(2, INT2NUM(vec[0]), INT2NUM(vec[1]));
}

static VALUE RGSS_Point_ToHash(VALUE self)
{
    int *vec = DATA_PTR(self);
    VALUE hash = rb_hash_new();
    rb_hash_aset(hash, STR2SYM("x"), INT2NUM(vec[0]));
    rb_hash_aset(hash, STR2SYM("y"), INT2NUM(vec[1]));
    return hash;
}

static VALUE RGSS_Size_ToHash(VALUE self)
{
    int *vec = DATA_PTR(self);
    VALUE hash = rb_hash_new();
    rb_hash_aset(hash, STR2SYM("width"), INT2NUM(vec[0]));
    rb_hash_aset(hash, STR2SYM("height"), INT2NUM(vec[1]));
    return hash;
}

static VALUE RGSS_IVec_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE x, y;
    rb_scan_args(argc, argv, "02", &x, &y);

    int *vec = DATA_PTR(self);

    switch (argc)
    {
        case 0:
            break;
        case 1: {
            memcpy(vec, DATA_PTR(x), IVEC_SIZE);
            break;
        }
        case 2: {
            vec[0] = NUM2INT(x);
            vec[1] = NUM2INT(y);
            break;
        }
    }
    return Qnil;
}

void RGSS_Init_PointAndSize(VALUE parent)
{
    rb_cIVec2 = rb_define_class_under(parent, "IVec2", rb_cObject);
    rb_cPoint = rb_define_class_under(parent, "Point", rb_cIVec2);
    rb_cSize = rb_define_class_under(parent, "Size", rb_cIVec2);
    rb_define_alloc_func(rb_cIVec2, RGSS_IVec_Alloc);
    rb_define_alloc_func(rb_cPoint, RGSS_IVec_Alloc);
    rb_define_alloc_func(rb_cSize, RGSS_IVec_Alloc);

    rb_define_methodm1(rb_cIVec2, "initialize", RGSS_IVec_Initialize, -1);
    rb_define_method1(rb_cIVec2, "[]", RGSS_IVec_GetIndex, 1);
    rb_define_method2(rb_cIVec2, "[]=", RGSS_IVec_SetIndex, 2);
    rb_define_method1(rb_cIVec2, "eql?", RGSS_IVec_Equal, 1);
    rb_define_method1(rb_cIVec2, "==", RGSS_IVec_Equal, 1);
    rb_define_method0(rb_cIVec2, "empty?", RGSS_IVec_IsEmpty, 0);
    rb_define_method1(rb_cIVec2, "+", RGSS_IVec_Add, 1);
    rb_define_method1(rb_cIVec2, "-", RGSS_IVec_Subtract, 1);
    rb_define_method1(rb_cIVec2, "add", RGSS_IVec_Add, 1);
    rb_define_method1(rb_cIVec2, "subtract", RGSS_IVec_Subtract, 1);
    rb_define_method0(rb_cIVec2, "clone", RGSS_IVec_Clone, 0);
    rb_define_method0(rb_cIVec2, "dup", RGSS_IVec_Dupe, 0);
    rb_define_method0(rb_cIVec2, "inspect", RGSS_IVec_Inspect, 0);
    rb_define_method2(rb_cIVec2, "set", RGSS_IVec_Set, 2);
    rb_define_method0(rb_cIVec2, "to_a", RGSS_IVec_ToArray, 0);
    rb_define_methodm1(rb_cIVec2, "_dump", RGSS_Point_Dump, -1);

    rb_define_method0(rb_cPoint, "x", RGSS_IVec_GetX, 0);
    rb_define_method0(rb_cPoint, "y", RGSS_IVec_GetY, 0);
    rb_define_method1(rb_cPoint, "x=", RGSS_IVec_SetX, 1);
    rb_define_method1(rb_cPoint, "y=", RGSS_IVec_SetY, 1);
    rb_define_methodm1(rb_cPoint, "offset!", RGSS_IVec_OffsetBang, -1);
    rb_define_methodm1(rb_cPoint, "offset", RGSS_IVec_Offset, -1);
    rb_define_method0(rb_cPoint, "to_h", RGSS_Point_ToHash, 0);

    rb_define_method0(rb_cSize, "width", RGSS_IVec_GetX, 0);
    rb_define_method0(rb_cSize, "height", RGSS_IVec_GetY, 0);
    rb_define_method1(rb_cSize, "width=", RGSS_IVec_SetX, 1);
    rb_define_method1(rb_cSize, "height=", RGSS_IVec_SetY, 1);
    rb_define_method0(rb_cSize, "to_h", RGSS_Size_ToHash, 0);

    rb_define_singleton_method0(rb_cIVec2, "empty", RGSS_IVec_Alloc, 0);
    rb_define_singleton_method1(rb_cIVec2, "_load", RGSS_Point_Load, 1);
}