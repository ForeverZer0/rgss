#include "rgss.h"

VALUE rb_mRGSS;

VALUE RGSS_Handle_Alloc(VALUE klass)
{
    return Data_Wrap_Struct(klass, NULL, RUBY_NEVER_FREE, NULL);
}

static VALUE RGSS_Math_Radians(VALUE math, VALUE degrees)
{
    return DBL2NUM(NUM2DBL(degrees) * (M_PI / 180.0));
}

static VALUE RGSS_Math_Degrees(VALUE math, VALUE radians)
{
    return DBL2NUM(NUM2DBL(radians) * (180.0 / M_PI));
}

void Init_rgss(void) {
    rb_mRGSS = rb_define_module("RGSS");

    rb_define_module_function1(rb_mMath, "degrees", RGSS_Math_Degrees, 1);
    rb_define_module_function1(rb_mMath, "radians", RGSS_Math_Radians, 1);

    RGSS_Init_GL(rb_mRGSS);
    RGSS_Init_GLFW(rb_mRGSS);
    RGSS_Init_Game(rb_mRGSS);
    RGSS_Init_Graphics(rb_mRGSS);
    RGSS_Init_Input(rb_mRGSS);

    RGSS_Init_Image(rb_mRGSS);
    RGSS_Init_ColorAndTone(rb_mRGSS);
    RGSS_Init_PointAndSize(rb_mRGSS);
    RGSS_Init_Rect(rb_mRGSS);
    RGSS_Init_Vec2(rb_mRGSS);
    RGSS_Init_Vec3(rb_mRGSS);
    RGSS_Init_Vec4(rb_mRGSS);
    RGSS_Init_Mat4(rb_mRGSS);
    RGSS_Init_Entity(rb_mRGSS);
    RGSS_Init_TrueType(rb_mRGSS);

    rb_define_const(rb_mRGSS, "SIZEOF_VOIDP",  INT2NUM(SIZEOF_VOIDP)); 
    rb_define_const(rb_mRGSS, "SIZEOF_CHAR",  INT2NUM(1)); 
    rb_define_const(rb_mRGSS, "SIZEOF_SHORT",  INT2NUM(SIZEOF_SHORT)); 
    rb_define_const(rb_mRGSS, "SIZEOF_INT",  INT2NUM(SIZEOF_INT)); 
    rb_define_const(rb_mRGSS, "SIZEOF_LONG",  INT2NUM(SIZEOF_LONG)); 
    rb_define_const(rb_mRGSS, "SIZEOF_LONG_LONG",  INT2NUM(SIZEOF_LONG_LONG));
    rb_define_const(rb_mRGSS, "SIZEOF_FLOAT",  INT2NUM(SIZEOF_FLOAT)); 
    rb_define_const(rb_mRGSS, "SIZEOF_DOUBLE",  INT2NUM(SIZEOF_DOUBLE)); 
    rb_define_const(rb_mRGSS, "SIZEOF_SIZE_T",  INT2NUM(SIZEOF_SIZE_T)); 
    rb_define_const(rb_mRGSS, "SIZEOF_SSIZE_T",  INT2NUM(SIZEOF_SSIZE_T)); 
    rb_define_const(rb_mRGSS, "SIZEOF_PTRDIFF_T",  INT2NUM(SIZEOF_PTRDIFF_T));
    rb_define_const(rb_mRGSS, "SIZEOF_INTPTR_T",  INT2NUM(SIZEOF_INTPTR_T)); 
    rb_define_const(rb_mRGSS, "SIZEOF_UINTPTR_T",  INT2NUM(SIZEOF_UINTPTR_T));
}
