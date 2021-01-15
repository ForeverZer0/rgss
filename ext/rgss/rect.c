#include "rgss.h"

VALUE rb_cRect;

RGSS_DEFINE_MARSHAL(RGSS_Rect)

static VALUE RGSS_Rect_Alloc(VALUE klass)
{
    RGSS_Rect *rect = ALLOC(RGSS_Rect);
    memset(rect, 0, sizeof(RGSS_Rect));
    return Data_Wrap_Struct(klass, NULL, RUBY_DEFAULT_FREE, rect);
}

static VALUE RGSS_Rect_GetX(VALUE self)
{
    RGSS_Rect *rect = DATA_PTR(self);
    return INT2NUM(rect->x);
}

static VALUE RGSS_Rect_GetY(VALUE self)
{
    RGSS_Rect *rect = DATA_PTR(self);
    return INT2NUM(rect->y);
}

static VALUE RGSS_Rect_GetWidth(VALUE self)
{
    RGSS_Rect *rect = DATA_PTR(self);
    return INT2NUM(rect->width);
}

static VALUE RGSS_Rect_GetHeight(VALUE self)
{
    RGSS_Rect *rect = DATA_PTR(self);
    return INT2NUM(rect->height);
}

static VALUE RGSS_Rect_GetLocation(VALUE self)
{
    RGSS_Rect *rect = DATA_PTR(self);
    return RGSS_IVec2_New(rb_cPoint, rect->x, rect->y);
}

static VALUE RGSS_Rect_GetSize(VALUE self)
{
    RGSS_Rect *rect = DATA_PTR(self);
    return RGSS_IVec2_New(rb_cSize, rect->width, rect->height);
}

static VALUE RGSS_Rect_GetIndex(VALUE self, VALUE index)
{
    int i = NUM2INT(index);
    if (i < 0 || i > 3)
        return Qnil;
    return INT2NUM(((int*) DATA_PTR(self))[i]);
}

static VALUE RGSS_Rect_SetIndex(VALUE self, VALUE index, VALUE value)
{
    int i = NUM2INT(index);
    if (i < 0 || i > 3)
        rb_raise(rb_eArgError, "index out of range (given %d, expect 0..3)", i);
    
    int *vec = DATA_PTR(self);
    vec[i] = NUM2INT(value);
    return value;
}

static VALUE RGSS_Rect_SetX(VALUE self, VALUE value)
{
    RGSS_ASSERT_UNFROZEN(self);
    RGSS_Rect *rect = DATA_PTR(self);
    rect->x = NUM2INT(value);
    return value;
}

static VALUE RGSS_Rect_SetY(VALUE self, VALUE value)
{
    RGSS_ASSERT_UNFROZEN(self);
    RGSS_Rect *rect = DATA_PTR(self);
    rect->y = NUM2INT(value);
    return value;
}

static VALUE RGSS_Rect_SetWidth(VALUE self, VALUE value)
{
    RGSS_ASSERT_UNFROZEN(self);
    RGSS_Rect *rect = DATA_PTR(self);
    rect->width = NUM2INT(value);
    return value;
}

static VALUE RGSS_Rect_SetHeight(VALUE self, VALUE value)
{
    RGSS_ASSERT_UNFROZEN(self);
    RGSS_Rect *rect = DATA_PTR(self);
    rect->y = NUM2INT(value);
    return value;
}

static VALUE RGSS_Rect_SetLocation(VALUE self, VALUE value)
{
    RGSS_ASSERT_UNFROZEN(self);
    RGSS_Rect *rect = DATA_PTR(self);
    RGSS_Point *ivec = DATA_PTR(value);
    memcpy(&rect->location, ivec, sizeof(RGSS_Point));
    return value;
}

static VALUE RGSS_Rect_SetSize(VALUE self, VALUE value)
{
    RGSS_ASSERT_UNFROZEN(self);
    RGSS_Rect *rect = DATA_PTR(self);
    RGSS_Size *ivec = DATA_PTR(value);
    memcpy(&rect->size, ivec, sizeof(RGSS_Size));
    return value;
}

static VALUE RGSS_Rect_Set(VALUE self, VALUE x, VALUE y, VALUE width, VALUE height)
{
    RGSS_ASSERT_UNFROZEN(self);
    RGSS_Rect *rect = DATA_PTR(self);

    rect->x = NUM2INT(x);
    rect->y = NUM2INT(y);
    rect->width = NUM2INT(width);
    rect->height = NUM2INT(height);

    return self;
}

static VALUE RGSS_Rect_Equal(VALUE self, VALUE other)
{
    if (CLASS_OF(self) != CLASS_OF(other))
        return Qfalse;

    RGSS_Rect *r1 = DATA_PTR(self), *r2 = DATA_PTR(other);
    return RB_BOOL(r1->x == r2->x && r1->y == r2->y && r1->width == r2->width && r1->height == r2->height);
}

static VALUE RGSS_Rect_Dupe(VALUE self)
{
    RGSS_Rect *src = DATA_PTR(self), *dst = ALLOC(RGSS_Rect);
    memcpy(dst, src, sizeof(RGSS_Rect));
    return Data_Wrap_Struct(CLASS_OF(self), NULL, RUBY_DEFAULT_FREE, dst);
}

static VALUE RGSS_Rect_Clone(VALUE self)
{
    RGSS_Rect *src = DATA_PTR(self), *dst = ALLOC(RGSS_Rect);
    memcpy(dst, src, sizeof(RGSS_Rect));

    VALUE value = Data_Wrap_Struct(CLASS_OF(self), NULL, RUBY_DEFAULT_FREE, dst);
    if (rb_obj_frozen_p(self))
        rb_obj_freeze(value);

    return value;
}

static VALUE RGSS_Rect_IsEmpty(VALUE self)
{
    RGSS_Rect *rect = DATA_PTR(self);
    return RB_BOOL(rect->x == 0 && rect->y == 0 && rect->width == 0 && rect->height == 0);
}

static VALUE RGSS_Rect_FromLTRB(VALUE klass, VALUE left, VALUE top, VALUE right, VALUE bottom)
{
    RGSS_Rect *rect = ALLOC(RGSS_Rect);

    int x, y, width, height;
    rect->x = NUM2INT(left);
    rect->y = NUM2INT(top);
    rect->width = NUM2INT(right) - rect->x;
    rect->height = NUM2INT(bottom) - rect->y;

    return Data_Wrap_Struct(klass, NULL, RUBY_DEFAULT_FREE, rect);
}

static VALUE RGSS_Rect_GetRight(VALUE self)
{
    RGSS_Rect *rect = ALLOC(RGSS_Rect);
    return INT2NUM(rect->x + rect->width);
}

static VALUE RGSS_Rect_GetBottom(VALUE self)
{
    RGSS_Rect *rect = ALLOC(RGSS_Rect);
    return INT2NUM(rect->y + rect->height);
}

static VALUE RGSS_Rect_Contains(int argc, VALUE *argv, VALUE self)
{
    VALUE arg1, arg2;
    rb_scan_args(argc, argv, "11", &arg1, &arg2);

    RGSS_Rect *rect = ALLOC(RGSS_Rect);

    if (argc == 2)
    {
        int x = NUM2INT(arg1);
        int y = NUM2INT(arg2);
        return RB_BOOL(rect->x <= x && x < rect->x + rect->width && rect->y <= y && y < rect->y + rect->height);
    }

    if (rb_obj_is_kind_of(arg1, rb_cPoint))
    {
        RGSS_Point *pnt = DATA_PTR(arg1);
        return RB_BOOL(rect->x <= pnt->x && pnt->x < rect->x + rect->width && rect->y <= pnt->y &&
                       pnt->y < rect->y + rect->height);
    }

    RGSS_Rect *other = DATA_PTR(arg1);
    return RB_BOOL((rect->x <= other->x) && ((other->x + other->width) <= (rect->x + rect->width)) &&
                   (rect->y <= other->y) && ((other->y + other->height) <= (rect->y + rect->height)));
}

static VALUE RGSS_Rect_InflateBang(int argc, VALUE *argv, VALUE self)
{
    VALUE a1, a2;
    rb_scan_args(argc, argv, "11", &a1, &a2);

    RGSS_ASSERT_UNFROZEN(self);
    RGSS_Rect *rect = DATA_PTR(self);

    int x, y;
    if (argc == 1)
    {
        int *point = DATA_PTR(a1);
        x = point[0];
        y = point[1];
    }
    else
    {
        x = NUM2INT(a1);
        y = NUM2INT(a2);
    }

    rect->x -= x;
    rect->y -= y;
    rect->width += (2 * x);
    rect->height += (2 * y);

    return self;
}

static VALUE RGSS_Rect_Inflate(int argc, VALUE *argv, VALUE self)
{
    return RGSS_Rect_InflateBang(argc, argv, RGSS_Rect_Dupe(self));
}

static VALUE RGSS_Rect_IntersectBang(VALUE self, VALUE other)
{
    RGSS_ASSERT_UNFROZEN(self);
    RGSS_Rect *a = DATA_PTR(self), *b = DATA_PTR(other);

    int x1 = RGSS_MAX(a->x, b->x);
    int x2 = RGSS_MIN(a->x + a->width, b->x + b->width);
    int y1 = RGSS_MAX(a->y, b->y);
    int y2 = RGSS_MIN(a->y + a->height, b->y + b->height);

    if (x2 >= x1 && y2 >= y1)
    {
        a->x = x1;
        a->y = y1;
        a->width = x2 - x1;
        a->height = y2 - y1;
    }
    else
    {
        memset(a, 0, sizeof(RGSS_Rect));
    }

    return self;
}

static VALUE RGSS_Rect_Intersect(VALUE self, VALUE other)
{
    return RGSS_Rect_Intersect(RGSS_Rect_Dupe(self), other);
}

static VALUE RGSS_Rect_IntersectWith(VALUE self, VALUE other)
{
    RGSS_Rect *a = DATA_PTR(self), *b = DATA_PTR(other);
    return RB_BOOL((b->x < a->x + a->width) && (a->x < (b->x + b->width)) && (b->y < a->y + a->height) &&
                   (a->y < b->y + b->height));
}

static VALUE RGSS_Rect_UnionBang(VALUE self, VALUE other)
{
    RGSS_ASSERT_UNFROZEN(self);
    RGSS_Rect *a = DATA_PTR(self), *b = DATA_PTR(other);

    int x1 = RGSS_MIN(a->x, b->x);
    int y1 = RGSS_MIN(a->y, b->y);
    int x2 = RGSS_MAX(a->x + a->width, b->x + b->width);
    int y2 = RGSS_MAX(a->y + a->height, b->y + b->height);
 
    a->x = x1;
    a->y = y1;
    a->width = x2 - x1;
    a->height = y2 - y1;

    return self;
}

static VALUE RGSS_Rect_Union(VALUE self, VALUE other)
{
    return RGSS_Rect_UnionBang(RGSS_Rect_Dupe(self), other);
}

static VALUE RGSS_Rect_OffsetBang(int argc, VALUE *argv, VALUE self)
{
    RGSS_ASSERT_UNFROZEN(self);
    VALUE a1, a2;
    rb_scan_args(argc, argv, "11", &a1, &a2);

    RGSS_Rect *rect = DATA_PTR(self);
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

    rect->x += x;
    rect->y += y;
    return self;
}

static VALUE RGSS_Rect_Offset(int argc, VALUE *argv, VALUE self)
{
    return RGSS_Rect_OffsetBang(argc, argv, RGSS_Rect_Dupe(self));
}

static VALUE RGSS_Rect_Inspect(VALUE self)
{
    RGSS_Rect *rect = DATA_PTR(self);
    return rb_sprintf("[%d, %d, %d, %d]", rect->x, rect->y, rect->width, rect->height);
}

static VALUE RGSS_Rect_ToArray(VALUE self)
{
    int *rect = DATA_PTR(self);
    VALUE ary = rb_ary_new_capa(4);
    
    for (int i = 0; i < 4; i++)
        rb_ary_store(ary, i, INT2NUM(rect[i]));
    return ary;
}

static VALUE RGSS_Rect_ToHash(VALUE self)
{
    RGSS_Rect *rect = DATA_PTR(self);
    VALUE hash = rb_hash_new();

    rb_hash_aset(hash, STR2SYM("x"), INT2NUM(rect->x));
    rb_hash_aset(hash, STR2SYM("y"), INT2NUM(rect->y));
    rb_hash_aset(hash, STR2SYM("width"), INT2NUM(rect->width));
    rb_hash_aset(hash, STR2SYM("height"), INT2NUM(rect->height));

    return hash;
}

static VALUE RGSS_Rect_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE a1, a2, a3, a4;
    rb_scan_args(argc, argv, "04", &a1, &a2, &a3, &a4);

    RGSS_Rect *rect = DATA_PTR(self);

    switch (argc)
    {
        case 0: break;
        case 2:
        {
            memcpy(&rect->location, DATA_PTR(a1), sizeof(RGSS_Point));
            memcpy(&rect->size, DATA_PTR(a2), sizeof(RGSS_Size));
            break;
        }
        case 4:
        {
            rect->x = NUM2INT(a1);
            rect->y = NUM2INT(a2);
            rect->width = NUM2INT(a3);
            rect->height = NUM2INT(a4);
            break;
        }
        default: rb_raise(rb_eArgError, "wrong number of arguments (given %d, expected 0, 2, 4)", argc);
    }

    return Qnil;
}

void RGSS_Init_Rect(VALUE parent)
{
    rb_cRect = rb_define_class_under(parent, "Rect", rb_cObject);
    rb_define_alloc_func(rb_cRect, RGSS_Rect_Alloc);
    rb_define_methodm1(rb_cRect, "initialize", RGSS_Rect_Initialize, -1);

    rb_define_method0(rb_cRect, "x", RGSS_Rect_GetX, 0);
    rb_define_method0(rb_cRect, "y", RGSS_Rect_GetY, 0);
    rb_define_method0(rb_cRect, "width", RGSS_Rect_GetWidth, 0);
    rb_define_method0(rb_cRect, "height", RGSS_Rect_GetHeight, 0);
    rb_define_method0(rb_cRect, "location", RGSS_Rect_GetLocation, 0);
    rb_define_method0(rb_cRect, "size", RGSS_Rect_GetSize, 0);
    rb_define_method1(rb_cRect, "x=", RGSS_Rect_SetX, 0);
    rb_define_method1(rb_cRect, "y=", RGSS_Rect_SetY, 0);
    rb_define_method1(rb_cRect, "width=", RGSS_Rect_SetWidth, 0);
    rb_define_method1(rb_cRect, "height=", RGSS_Rect_SetHeight, 0);
    rb_define_method1(rb_cRect, "location=", RGSS_Rect_SetLocation, 0);
    rb_define_method1(rb_cRect, "size=", RGSS_Rect_SetSize, 0);
    rb_define_method1(rb_cRect, "[]", RGSS_Rect_GetIndex, 1);
    rb_define_method2(rb_cRect, "[]=", RGSS_Rect_SetIndex, 2);
    rb_define_method4(rb_cRect, "set", RGSS_Rect_Set, 4);
    rb_define_method1(rb_cRect, "==", RGSS_Rect_Equal, 1);
    rb_define_method1(rb_cRect, "eql?", RGSS_Rect_Equal, 1);
    rb_define_method0(rb_cRect, "dup", RGSS_Rect_Dupe, 0);
    rb_define_method0(rb_cRect, "clone", RGSS_Rect_Clone, 0);
    rb_define_method0(rb_cRect, "empty?", RGSS_Rect_IsEmpty, 0);
    rb_define_method0(rb_cRect, "left", RGSS_Rect_GetX, 0);
    rb_define_method0(rb_cRect, "top", RGSS_Rect_GetY, 0);
    rb_define_method0(rb_cRect, "right", RGSS_Rect_GetRight, 0);
    rb_define_method0(rb_cRect, "bottom", RGSS_Rect_GetBottom, 0);
    rb_define_methodm1(rb_cRect, "contains?", RGSS_Rect_Contains, -1);
    rb_define_methodm1(rb_cRect, "inflate!", RGSS_Rect_InflateBang, -1);
    rb_define_methodm1(rb_cRect, "inflate", RGSS_Rect_Inflate, -1);
    rb_define_method1(rb_cRect, "intersect!", RGSS_Rect_IntersectBang, 1);
    rb_define_method1(rb_cRect, "intersect", RGSS_Rect_Intersect, 1);
    rb_define_method1(rb_cRect, "intersect?", RGSS_Rect_IntersectWith, 1);
    rb_define_methodm1(rb_cRect, "offset!", RGSS_Rect_OffsetBang, -1);
    rb_define_methodm1(rb_cRect, "offset", RGSS_Rect_Offset, -1);
    rb_define_method0(rb_cRect, "inspect", RGSS_Rect_Inspect, 0);
    rb_define_method0(rb_cRect, "to_a", RGSS_Rect_ToArray, 0);
    rb_define_method0(rb_cRect, "to_h", RGSS_Rect_ToHash, 0);
    rb_define_methodm1(rb_cRect, "_dump", RGSS_Rect_Dump, -1);

    rb_define_singleton_method0(rb_cRect, "empty", RGSS_Rect_Alloc, 0);
    rb_define_singleton_method4(rb_cRect, "ltrb", RGSS_Rect_FromLTRB, 4);
    rb_define_singleton_method1(rb_cRect, "_load", RGSS_Rect_Load, 1);

    rb_define_alias(rb_cRect, "include?", "contains?");
}