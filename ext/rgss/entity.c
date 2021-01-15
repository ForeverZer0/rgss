#include "game.h"

VALUE rb_cEntity;

vec3 AXIS_Z = { 0.0f, 0.0f, 1.0f };

typedef struct {
    vec4 *model;
    vec3 position;
    vec3 velocity;
    vec3 scale;
    vec3 pivot;
    float angle;
    int depth;
    vec3 size;
} RGSS_Entity;

// TODO: Use direct pointer with RUBY_NEVER_FREE instead of new vector?

static inline void RGSS_Entity_ParseArg(VALUE value, vec3 result)
{
    if (!RTEST(value))
    {
        glm_vec3_zero(result);
        return;
    }

    result[2] = 0.0f;
    if (rb_obj_is_kind_of(value, rb_cPoint) == Qtrue || rb_obj_is_kind_of(value, rb_cSize) == Qtrue)
    {
        int *ivec = DATA_PTR(value);
        result[0] = (float) ivec[0];
        result[1] = (float) ivec[1];
    }
    else if (rb_obj_is_kind_of(value, rb_cVec2))
    {
        float *vec = DATA_PTR(value);
        result[0] = vec[0];
        result[1] = vec[1];        
    }
    else if (rb_obj_is_kind_of(value, rb_cVec3))
    {
        glm_vec3_copy(DATA_PTR(value), result);
    }
    else
    {
        rb_raise(rb_eTypeError, "%s is not a Point, Size, Vec2, or Vec3", CLASS_NAME(value));
    }
}

static void RGSS_Entity_Free(void *data)
{
    if (data)
    {
        RGSS_Entity *entity = data;
        if (entity->model)
        {
            free(entity->model);
            entity->model = NULL;
        }
        xfree(data);
    }
}

static VALUE RGSS_Entity_Alloc(VALUE klass)
{
    RGSS_Entity *entity = ALLOC(RGSS_Entity);
    entity->model = RGSS_MAT4_NEW;
    glm_mat4_identity(entity->model);

    glm_vec3_zero(entity->position);
    glm_vec3_zero(entity->velocity);
    glm_vec3_zero(entity->size);
    glm_vec3_one(entity->scale);
    glm_vec3_zero(entity->pivot);
    entity->angle = 0.0f;
    entity->depth = 0;

    return Data_Wrap_Struct(klass, NULL, RGSS_Entity_Free, entity);
}

static VALUE RGSS_Entity_GetX(VALUE self)
{   
    RGSS_Entity *entity = DATA_PTR(self);
    return INT2NUM((int) roundf(entity->position[0]));
}

static VALUE RGSS_Entity_SetX(VALUE self, VALUE value)
{
    RGSS_Entity *entity = DATA_PTR(self);
    entity->position[0] = NUM2FLT(value);
    return self;
}

static VALUE RGSS_Entity_GetY(VALUE self)
{   
    RGSS_Entity *entity = DATA_PTR(self);
    return INT2NUM((int) roundf(entity->position[1]));
}

static VALUE RGSS_Entity_SetY(VALUE self, VALUE value)
{
    RGSS_Entity *entity = DATA_PTR(self);
    entity->position[1] = NUM2FLT(value);
    return self;
}

static VALUE RGSS_Entity_GetZ(VALUE self)
{   
    RGSS_Entity *entity = DATA_PTR(self);
    return INT2NUM(entity->depth);
}

static VALUE RGSS_Entity_SetZ(VALUE self, VALUE value)
{
    RGSS_Entity *entity = DATA_PTR(self);
    entity->depth = NUM2INT(value);
    return self;
}

static VALUE RGSS_Entity_GetLocation(VALUE self)
{
    RGSS_Entity *entity = DATA_PTR(self);
    return RGSS_Point_New((int) roundf(entity->position[0]), (int) roundf(entity->position[1]));
}

static VALUE RGSS_Entity_SetLocation(VALUE self, VALUE point)
{
    RGSS_Entity *entity = DATA_PTR(self);
    int *ivec = DATA_PTR(point);
    entity->position[0] = (float) ivec[0];
    entity->position[1] = (float) ivec[1];    
    return point;
}

static VALUE RGSS_Entity_Update(VALUE self, VALUE delta)
{
    RGSS_Entity *entity = DATA_PTR(self);

    vec3 scale;
    glm_vec3_mul(entity->scale, entity->size, scale);

    vec3 velocity;
    glm_vec3_scale(entity->velocity, NUM2FLT(delta), velocity);

    glm_vec3_add(entity->position, velocity, entity->position);


    vec3 pivot;
    glm_vec3_add(entity->pivot, entity->position, pivot);

    glm_rotate_atm(entity->model, pivot, entity->angle, AXIS_Z);
    glm_translate(entity->model, entity->position);
    glm_scale(entity->model, scale);

    return self;
}

static VALUE RGSS_Entity_GetModel(VALUE self)
{
    RGSS_Entity *entity = DATA_PTR(self);
    return Data_Wrap_Struct(rb_cMat4, NULL, RUBY_NEVER_FREE, entity->model);
}

static VALUE RGSS_Entity_SetModel(VALUE self, VALUE model)
{
    RGSS_Entity *entity = DATA_PTR(self);
    if (RTEST(model))
    {
        vec4 *mat = DATA_PTR(model);
        glm_mat4_copy(mat, entity->model);
    }
    else
    {
        glm_mat4_identity(entity->model);
    }
    return model;
}

static VALUE RGSS_Entity_GetPosition(VALUE self)
{
    RGSS_Entity *entity = DATA_PTR(self);
    return RGSS_Vec2_New(entity->position[0], entity->position[1]);
}

static VALUE RGSS_Entity_SetPosition(VALUE self, VALUE value)
{
    RGSS_Entity *entity = DATA_PTR(self);

    vec3 vec;
    RGSS_Entity_ParseArg(value, vec);
    
    glm_vec3_copy(vec, entity->position);
    return value;
}

static VALUE RGSS_Entity_GetVelocity(VALUE self)
{
    RGSS_Entity *entity = DATA_PTR(self);
    return RGSS_Vec2_New(entity->velocity[0], entity->velocity[1]);
}

static VALUE RGSS_Entity_SetVelocity(VALUE self, VALUE value)
{
    RGSS_Entity *entity = DATA_PTR(self);

    vec3 vec;
    RGSS_Entity_ParseArg(value, vec);
    
    glm_vec3_copy(vec, entity->velocity);
    return value;
}

static VALUE RGSS_Entity_GetScale(VALUE self)
{
    RGSS_Entity *entity = DATA_PTR(self);
    return RGSS_Vec2_New(entity->scale[0], entity->scale[1]);
}

static VALUE RGSS_Entity_SetScale(VALUE self, VALUE value)
{
    RGSS_Entity *entity = DATA_PTR(self);

    vec3 vec;
    RGSS_Entity_ParseArg(value, vec);
    
    glm_vec3_copy(vec, entity->scale);
    return value;
}

static VALUE RGSS_Entity_GetPivot(VALUE self)
{
    RGSS_Entity *entity = DATA_PTR(self);
    return RGSS_Vec2_New(entity->pivot[0], entity->pivot[1]);
}

static VALUE RGSS_Entity_SetPivot(VALUE self, VALUE value)
{
    RGSS_Entity *entity = DATA_PTR(self);

    vec3 vec;
    RGSS_Entity_ParseArg(value, vec);
    
    glm_vec3_copy(vec, entity->pivot);
    return value;
}

static VALUE RGSS_Entity_GetSize(VALUE self)
{
    RGSS_Entity *entity = DATA_PTR(self);
    return RGSS_Size_New((int) roundf(entity->size[0]), (int) roundf(entity->size[1]));
}

static VALUE RGSS_Entity_SetSize(VALUE self, VALUE value)
{
    RGSS_Entity *entity = DATA_PTR(self);

    vec3 vec;
    RGSS_Entity_ParseArg(value, vec);
    
    glm_vec3_copy(vec, entity->size);
    return value;
}

static VALUE RGSS_Entity_GetAngle(VALUE self)
{
    RGSS_Entity *entity = DATA_PTR(self);
    return DBL2NUM(entity->angle * (180.0 / M_PI));
}

static VALUE RGSS_Entity_SetAngle(VALUE self, VALUE degrees)
{
    RGSS_Entity *entity = DATA_PTR(self);
    entity->angle = NUM2DBL(degrees) * (M_PI / 180.0);
    return degrees;
}

static VALUE RGSS_Entity_GetBounds(VALUE self)
{
    RGSS_Entity *entity = DATA_PTR(self);
    int *rect = xmalloc(sizeof(int) * 4);

    rect[0] = (int) roundf(entity->position[0]);
    rect[1] = (int) roundf(entity->position[1]);
    rect[2] = (int) roundf(entity->size[0]);
    rect[3] = (int) roundf(entity->size[1]);

    return Data_Wrap_Struct(rb_cRect, NULL, RUBY_DEFAULT_FREE, rect);
}

static VALUE RGSS_Entity_Rotate(VALUE self, VALUE degrees, VALUE pivot)
{
    RGSS_Entity *entity = DATA_PTR(self);

    vec3 value;
    RGSS_Entity_ParseArg(pivot, value);

    entity->angle = NUM2DBL(degrees) * (M_PI / 180.0);
    glm_vec3_copy(value, entity->pivot);

    return self;
}

static VALUE RGSS_Entity_GetWidth(VALUE self)
{
    RGSS_Entity *entity = DATA_PTR(self);
    return INT2NUM((int) roundf(entity->size[0]));
}

static VALUE RGSS_Entity_GetHeight(VALUE self)
{
    RGSS_Entity *entity = DATA_PTR(self);
    return INT2NUM((int) roundf(entity->size[1]));
}

static VALUE RGSS_Entity_SetWidth(VALUE self, VALUE value)
{
    RGSS_Entity *entity = DATA_PTR(self);
    entity->size[0] = NUM2FLT(value);
    return value;
}

static VALUE RGSS_Entity_SetHeight(VALUE self, VALUE value)
{
    RGSS_Entity *entity = DATA_PTR(self);
    entity->size[1] = NUM2FLT(value);
    return value;
}

void RGSS_Init_Entity(VALUE parent)
{
    rb_cEntity = rb_define_class_under(parent, "Entity", rb_cObject);
    rb_define_alloc_func(rb_cEntity, RGSS_Entity_Alloc);

    rb_define_method1(rb_cEntity, "update", RGSS_Entity_Update, 1);

    rb_define_method0(rb_cEntity, "model", RGSS_Entity_GetModel, 0);
    rb_define_method1(rb_cEntity, "model=", RGSS_Entity_SetModel, 1);
    rb_define_method0(rb_cEntity, "position", RGSS_Entity_GetPosition, 0);
    rb_define_method1(rb_cEntity, "position=", RGSS_Entity_SetPosition, 1);
    rb_define_method0(rb_cEntity, "velocity", RGSS_Entity_GetVelocity, 0);
    rb_define_method1(rb_cEntity, "velocity=", RGSS_Entity_SetVelocity, 1);
    rb_define_method0(rb_cEntity, "scale", RGSS_Entity_GetScale, 0);
    rb_define_method1(rb_cEntity, "scale=", RGSS_Entity_SetScale, 1);
    rb_define_method0(rb_cEntity, "pivot", RGSS_Entity_GetPivot, 0);
    rb_define_method1(rb_cEntity, "pivot=", RGSS_Entity_SetPivot, 1);
    rb_define_method0(rb_cEntity, "angle", RGSS_Entity_GetAngle, 0);
    rb_define_method1(rb_cEntity, "angle=", RGSS_Entity_SetAngle, 1);

    rb_define_method0(rb_cEntity, "bounds", RGSS_Entity_GetBounds, 0);
    rb_define_method2(rb_cEntity, "rotate", RGSS_Entity_Rotate, 2);

    rb_define_method0(rb_cEntity, "x", RGSS_Entity_GetX, 0);
    rb_define_method1(rb_cEntity, "x=", RGSS_Entity_SetX, 1);
    rb_define_method0(rb_cEntity, "y", RGSS_Entity_GetY, 0);
    rb_define_method1(rb_cEntity, "y=", RGSS_Entity_SetY, 1);
    rb_define_method0(rb_cEntity, "z", RGSS_Entity_GetZ, 0);
    rb_define_method1(rb_cEntity, "z=", RGSS_Entity_SetZ, 1);
    rb_define_method0(rb_cEntity, "location", RGSS_Entity_GetLocation, 0);
    rb_define_method1(rb_cEntity, "location=", RGSS_Entity_SetLocation, 1);
    rb_define_method0(rb_cEntity, "size", RGSS_Entity_GetSize, 0);
    rb_define_method0(rb_cEntity, "width", RGSS_Entity_GetWidth, 0);
    rb_define_method0(rb_cEntity, "height", RGSS_Entity_GetHeight, 0);

    rb_define_protected_method1(rb_cEntity, "size=", RGSS_Entity_SetSize, 1);
    rb_define_protected_method1(rb_cEntity, "width=", RGSS_Entity_SetWidth, 1);
    rb_define_protected_method1(rb_cEntity, "height=", RGSS_Entity_SetHeight, 1);

    rb_define_alias(rb_cEntity, "depth", "z");

    // rb_define_method0(rb_cEntity, "model", RGSS_Entity_GetModel, 0);
    // rb_define_method1(rb_cEntity, "model", RGSS_Entity_SetModel, 1);
}