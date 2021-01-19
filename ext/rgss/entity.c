#include "game.h"

#define VERTICES_COUNT  16
#define VERTICES_SIZE   (SIZEOF_FLOAT * VERTICES_COUNT)
#define VERTICES_STRIDE (SIZEOF_FLOAT * 4)
#define INDICES_COUNT   6

#define RGSS_SHADER RGSS_GAME.graphics.shader

VALUE rb_cEntity;
VALUE rb_cRenderable;
VALUE rb_cBlend;

vec3 AXIS_Z = {0.0f, 0.0f, 1.0f};

typedef struct
{
    RGSS_Renderable base;

} RGSS_Sprite;

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
        result[0] = (float)ivec[0];
        result[1] = (float)ivec[1];
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

static void RGSS_Enity_Init(RGSS_Entity *entity)
{
    entity->model = RGSS_MAT4_NEW;
    glm_mat4_identity(entity->model);
    glm_vec3_zero(entity->position);
    glm_vec3_zero(entity->velocity);
    glm_vec3_zero(entity->size);
    glm_vec3_one(entity->scale);
    glm_vec3_zero(entity->pivot);
    entity->angle = 0.0f;
    entity->depth = 0;
}

static void RGSS_Renderable_Init(RGSS_Renderable *obj)
{
    obj->opacity = 1.0f;
    obj->visible = true;
    obj->flash_duration = -1;

    obj->blend.op = GL_FUNC_ADD;
    obj->blend.src = GL_SRC_ALPHA;
    obj->blend.dst = GL_ONE_MINUS_SRC_ALPHA;

    glGenVertexArrays(1, &obj->vao);
    glGenBuffers(1, &obj->vbo);
    glGenBuffers(1, &obj->ebo);
}

static VALUE RGSS_Entity_Alloc(VALUE klass)
{
    RGSS_Entity *entity = ALLOC(RGSS_Entity);
    RGSS_Enity_Init(entity);
    return Data_Wrap_Struct(klass, NULL, RGSS_Entity_Free, entity);
}

static VALUE RGSS_Entity_GetX(VALUE self)
{
    RGSS_Entity *entity = DATA_PTR(self);
    return INT2NUM((int)roundf(entity->position[0]));
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
    return INT2NUM((int)roundf(entity->position[1]));
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
    return RGSS_Point_New((int)roundf(entity->position[0]), (int)roundf(entity->position[1]));
}

static VALUE RGSS_Entity_SetLocation(VALUE self, VALUE point)
{
    RGSS_Entity *entity = DATA_PTR(self);
    int *ivec = DATA_PTR(point);
    entity->position[0] = (float)ivec[0];
    entity->position[1] = (float)ivec[1];
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
    return RGSS_Size_New((int)roundf(entity->size[0]), (int)roundf(entity->size[1]));
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

    rect[0] = (int)roundf(entity->position[0]);
    rect[1] = (int)roundf(entity->position[1]);
    rect[2] = (int)roundf(entity->size[0]);
    rect[3] = (int)roundf(entity->size[1]);

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
    return INT2NUM((int)roundf(entity->size[0]));
}

static VALUE RGSS_Entity_GetHeight(VALUE self)
{
    RGSS_Entity *entity = DATA_PTR(self);
    return INT2NUM((int)roundf(entity->size[1]));
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

static void RGSS_Renderable_Mark(void *data)
{
    RGSS_Renderable *obj = data;
    rb_gc_mark(obj->parent);
}

static VALUE RGSS_Renderable_Alloc(VALUE klass)
{
    RGSS_Renderable *obj = ALLOC(RGSS_Renderable);
    memset(obj, 0, sizeof(RGSS_Renderable));
    RGSS_Enity_Init(&obj->entity);
    return Data_Wrap_Struct(klass, RGSS_Renderable_Mark, RUBY_DEFAULT_FREE, obj);
}

static VALUE RGSS_Renderable_Initialize(VALUE self, VALUE parent)
{
    if (NIL_P(parent))
        rb_raise(rb_eArgError, "parent cannot be nil");
    if (rb_obj_is_kind_of(parent, rb_cBatch) != Qtrue)
        rb_raise(rb_eTypeError, "%s is not a Batch", CLASS_NAME(parent));

    RGSS_Renderable *obj = DATA_PTR(self);
    RGSS_Renderable_Init(obj);

    obj->parent = parent;
    RGSS_Batch_Add(parent, self);
    return self;
}

static VALUE RGSS_Renderable_Dispose(VALUE self)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    RGSS_Batch_Remove(obj->parent, self);
    obj->parent = Qnil;

    if (obj->vao)
    {
        glDeleteVertexArrays(1, &obj->vao);
        obj->vao = GL_NONE;
    }
    if (obj->vbo)
    {
        glDeleteBuffers(1, &obj->vbo);
        obj->vbo = GL_NONE;
    }
    if (obj->ebo)
    {
        glDeleteBuffers(1, &obj->ebo);
        obj->ebo = GL_NONE;
    }
    return Qnil;
}

static VALUE RGSS_Renderable_IsDisposed(VALUE self)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    return RB_BOOL(obj->vao == GL_NONE);
}

static VALUE RGSS_Renderable_SetDepth(VALUE self, VALUE value)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    RGSS_Batch_Invalidate(obj->parent);
    return rb_call_super(1, &value);    
}

static VALUE RGSS_Renderable_GetParent(VALUE self)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    return obj->parent;
}

static VALUE RGSS_Renderable_Update(VALUE self, VALUE delta)
{
    rb_call_super(1, &delta);
    RGSS_Renderable *obj = DATA_PTR(self);
    if (obj->flash_duration > -1)
    {
        obj->flash_duration--;
        if (obj->flash_duration <= -1)
            glm_vec4_zero(obj->flash_color);
    }
}

static VALUE RGSS_Renderable_GetColor(VALUE self)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    float *color = RGSS_VEC4_NEW;
    glm_vec4_copy(obj->color, color);
    return Data_Wrap_Struct(rb_cColor, NULL, free, color);
}

static VALUE RGSS_Renderable_SetColor(VALUE self, VALUE color)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    if (rb_obj_is_kind_of(color, rb_cColor))
    {
        float *vec = DATA_PTR(color);
        glm_vec4_copy(vec, obj->color);
    }
    else
    {
        glm_vec4_zero(obj->color);
    }
    return color;
}

static VALUE RGSS_Renderable_GetFlashColor(VALUE self)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    float *color = RGSS_VEC4_NEW;
    glm_vec4_copy(obj->flash_color, color);
    return Data_Wrap_Struct(rb_cColor, NULL, free, color);
}

static VALUE RGSS_Renderable_SetFlashColor(VALUE self, VALUE color)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    if (rb_obj_is_kind_of(color, rb_cColor))
    {
        float *vec = DATA_PTR(color);
        glm_vec4_copy(vec, obj->flash_color);
    }
    else
    {
        glm_vec4_zero(obj->flash_color);
    }
    return color;
}

static VALUE RGSS_Renderable_GetTone(VALUE self)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    float *tone = RGSS_VEC4_NEW;
    glm_vec4_copy(obj->tone, tone);
    return Data_Wrap_Struct(rb_cTone, NULL, free, tone);
}

static VALUE RGSS_Renderable_SetTone(VALUE self, VALUE tone)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    if (rb_obj_is_kind_of(tone, rb_cTone))
    {
        float *vec = DATA_PTR(tone);
        glm_vec4_copy(vec, obj->tone);
    }
    else
    {
        glm_vec4_zero(obj->tone);
    }
    return tone;
}

static VALUE RGSS_Renderable_GetOpacity(VALUE self)
{
    return DBL2NUM(((RGSS_Renderable *)DATA_PTR(self))->opacity);
}

static VALUE RGSS_Renderable_SetOpacity(VALUE self, VALUE value)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    obj->opacity = glm_clamp(NUM2FLT(value), 0.0f, 1.0f);
    return value;
}

static VALUE RGSS_Renderable_GetHue(VALUE self)
{
    return DBL2NUM(((RGSS_Renderable *)DATA_PTR(self))->hue);
}

static VALUE RGSS_Renderable_SetHue(VALUE self, VALUE value)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    float r = remainderf(NUM2FLT(value), 360.0f);
    obj->hue = r < 0.0f ? r + 360.0f : r;
    return value;
}

static VALUE RGSS_Renderable_GetVisible(VALUE self)
{
    return RB_BOOL(((RGSS_Renderable *)DATA_PTR(self))->visible);
}

static VALUE RGSS_Renderable_SetVisible(VALUE self, VALUE value)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    obj->visible = RTEST(value);
    return value;
}

static VALUE RGSS_Renderable_GetFlashDuration(VALUE self)
{
    return INT2NUM(((RGSS_Renderable *)DATA_PTR(self))->flash_duration);
}

static VALUE RGSS_Renderable_SetFlashDuration(VALUE self, VALUE value)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    int i = NUM2INT(value);
    obj->flash_duration = RGSS_MAX(i, -1);
    return value;
}

static VALUE RGSS_Renderable_IsFlashing(VALUE self)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    return RB_BOOL(obj->flash_duration > -1);
}

static VALUE RGSS_Renderable_Flash(VALUE self, VALUE color, VALUE ticks)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    if (rb_obj_is_kind_of(color, rb_cColor))
    {
        float *v = DATA_PTR(color);
        glm_vec4_copy(v, obj->flash_color);
        obj->flash_duration = NUM2INT(ticks);
        if (obj->flash_duration < -1)
            obj->flash_duration = -1;
    }
    else
    {
        glm_vec4_zero(obj->flash_color);
        obj->flash_duration = -1;
    }
    return Qnil;
}

static VALUE RGSS_Renderable_GetBlend(VALUE self)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    RGSS_Blend *blend = ALLOC(RGSS_Blend);
    memcpy(blend, &obj->blend, sizeof(RGSS_Blend));
    return Data_Wrap_Struct(rb_cBlend, NULL, RUBY_DEFAULT_FREE, blend);
}

static VALUE RGSS_Renderable_SetBlend(VALUE self, VALUE blend)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    if (rb_obj_is_kind_of(blend, rb_cBlend))
    {
        memcpy(&obj->blend, DATA_PTR(blend), sizeof(RGSS_Blend));
    }
    else
    {
        obj->blend.op = GL_FUNC_ADD;
        obj->blend.src = GL_SRC_ALPHA;
        obj->blend.dst = GL_ONE_MINUS_SRC_ALPHA;
    }
    return blend;
}

static VALUE RGSS_Renderable_VertexSetup(int argc, VALUE *argv, VALUE self)
{
    VALUE vertices, indices, vbo_usage, ebo_usage;
    rb_scan_args(argc, argv, "04", &vertices, &indices, &vbo_usage, &ebo_usage);

    GLenum vu = RTEST(vbo_usage) ? NUM2INT(vbo_usage) : GL_DYNAMIC_DRAW;
    GLenum eu = RTEST(ebo_usage) ? NUM2INT(ebo_usage) : GL_STATIC_DRAW;
    RGSS_Renderable *obj = DATA_PTR(self);
    glBindVertexArray(obj->vao);

    glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);
    if (RTEST(vertices))
    {
        if (rb_array_len(vertices) != VERTICES_COUNT)
            rb_raise(rb_eArgError, "vertices size is not equal to VERTICES_COUNT");

        GLfloat v[VERTICES_COUNT];
        for (long i = 0; i < VERTICES_COUNT; i++)
            v[i] = NUM2FLT(rb_ary_entry(vertices, i));

        glBufferData(GL_ARRAY_BUFFER, VERTICES_SIZE, v, vu);
    }
    else
        glBufferData(GL_ARRAY_BUFFER, VERTICES_SIZE, NULL, vu);

    GLubyte ind[6] = {0, 1, 2, 0, 3, 1};
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->ebo);

    if (RTEST(indices))
    {
        if (rb_array_len(indices) != INDICES_COUNT)
            rb_raise(rb_eArgError, "vertices size is not equal to INDICES_COUNT");

        for (long i = 0; i < INDICES_COUNT; i++)
            ind[i] = (GLubyte)NUM2CHR(rb_ary_entry(indices, i));
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDICES_COUNT, ind, eu);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VERTICES_STRIDE, NULL);
    glBindVertexArray(GL_NONE);
    return Qnil;
}

static VALUE RGSS_Renderable_GetVAO(VALUE self)
{
    return UINT2NUM(((RGSS_Renderable *)DATA_PTR(self))->vao);
}

static VALUE RGSS_Renderable_GetVBO(VALUE self)
{
    return UINT2NUM(((RGSS_Renderable *)DATA_PTR(self))->vbo);
}

static VALUE RGSS_Renderable_GetEBO(VALUE self)
{
    return UINT2NUM(((RGSS_Renderable *)DATA_PTR(self))->ebo);
}

static VALUE RGSS_Renderable_Render(VALUE self, VALUE alpha)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    if (!obj->visible || obj->opacity < FLT_EPSILON)
        return Qnil;

    glBlendEquation(obj->blend.op);
    glBlendFunc(obj->blend.src, obj->blend.dst);


    glUseProgram(RGSS_SHADER.id);
    glUniformMatrix4fv(RGSS_SHADER.model, 1, false, obj->entity.model[0]);
    glUniform4fv(RGSS_SHADER.color, 1, obj->color);
    glUniform4fv(RGSS_SHADER.tone, 1, obj->tone);
    glUniform4fv(RGSS_SHADER.flash, 1, obj->flash_color);
    glUniform1f(RGSS_SHADER.hue, obj->hue);
    glUniform1f(RGSS_SHADER.opacity, obj->opacity);
}

static VALUE RGSS_Blend_Alloc(VALUE klass)
{
    return Data_Wrap_Struct(klass, NULL, RUBY_DEFAULT_FREE, ALLOC(RGSS_Blend));
}

static VALUE RGSS_Blend_Initialize(VALUE self, VALUE op, VALUE src, VALUE dst)
{
    RGSS_Blend *blend = DATA_PTR(self);
    blend->op = NUM2INT(op);
    blend->src = NUM2INT(src);
    blend->dst = NUM2INT(dst);
    return self;
}

static VALUE RGSS_Blend_GetOp(VALUE self)
{
    return INT2NUM(((RGSS_Blend *)DATA_PTR(self))->op);
}

static VALUE RGSS_Blend_GetSrc(VALUE self)
{
    return INT2NUM(((RGSS_Blend *)DATA_PTR(self))->src);
}

static VALUE RGSS_Blend_GetDst(VALUE self)
{
    return INT2NUM(((RGSS_Blend *)DATA_PTR(self))->dst);
}

static VALUE RGSS_Blend_Equal(VALUE self, VALUE other)
{
    if (CLASS_OF(self) != CLASS_OF(other))
        return Qfalse;
    RGSS_Blend *b1 = DATA_PTR(self), *b2 = DATA_PTR(other);
    return RB_BOOL(b1->op == b2->op && b1->src == b2->src && b1->dst == b2->dst);
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

    rb_cRenderable = rb_define_class_under(parent, "Renderable", rb_cEntity);
    rb_define_alloc_func(rb_cRenderable, RGSS_Renderable_Alloc);
    rb_define_method1(rb_cRenderable, "initialize", RGSS_Renderable_Initialize, 1);
    rb_define_method1(rb_cRenderable, "update", RGSS_Renderable_Update, 1);
    rb_define_method0(rb_cRenderable, "dispose", RGSS_Renderable_Dispose, 0);
    rb_define_method0(rb_cRenderable, "disposed?", RGSS_Renderable_IsDisposed, 0);
    rb_define_method0(rb_cRenderable, "color", RGSS_Renderable_GetColor, 0);
    rb_define_method1(rb_cRenderable, "color=", RGSS_Renderable_SetColor, 1);
    rb_define_method0(rb_cRenderable, "flash_color", RGSS_Renderable_GetFlashColor, 0);
    rb_define_method1(rb_cRenderable, "flash_color=", RGSS_Renderable_SetFlashColor, 1);
    rb_define_method0(rb_cRenderable, "flash_duration", RGSS_Renderable_GetFlashDuration, 0);
    rb_define_method1(rb_cRenderable, "flash_duration=", RGSS_Renderable_SetFlashDuration, 1);
    rb_define_method0(rb_cRenderable, "tone", RGSS_Renderable_GetTone, 0);
    rb_define_method1(rb_cRenderable, "tone=", RGSS_Renderable_SetTone, 1);
    rb_define_method0(rb_cRenderable, "opacity", RGSS_Renderable_GetOpacity, 0);
    rb_define_method1(rb_cRenderable, "opacity=", RGSS_Renderable_SetOpacity, 1);
    rb_define_method0(rb_cRenderable, "hue", RGSS_Renderable_GetHue, 0);
    rb_define_method1(rb_cRenderable, "hue=", RGSS_Renderable_SetHue, 1);
    rb_define_method0(rb_cRenderable, "visible", RGSS_Renderable_GetVisible, 0);
    rb_define_method1(rb_cRenderable, "visible=", RGSS_Renderable_SetVisible, 1);
    rb_define_method2(rb_cRenderable, "flash", RGSS_Renderable_Flash, 2);
    rb_define_method0(rb_cRenderable, "flashing?", RGSS_Renderable_IsFlashing, 0);
    rb_define_method0(rb_cRenderable, "blend", RGSS_Renderable_GetBlend, 0);
    rb_define_method1(rb_cRenderable, "blend=", RGSS_Renderable_SetBlend, 1);
    rb_define_method1(rb_cRenderable, "z=", RGSS_Renderable_SetDepth, 1);
    rb_define_method1(rb_cRenderable, "render", RGSS_Renderable_Render, 1);
    rb_define_protected_method0(rb_cRenderable, "parent", RGSS_Renderable_GetParent, 0);
    rb_define_protected_methodm1(rb_cRenderable, "vertex_setup", RGSS_Renderable_VertexSetup, -1);
    rb_define_protected_method0(rb_cRenderable, "vao", RGSS_Renderable_GetVAO, 0);
    rb_define_protected_method0(rb_cRenderable, "vbo", RGSS_Renderable_GetVBO, 0);
    rb_define_protected_method0(rb_cRenderable, "ebo", RGSS_Renderable_GetEBO, 0);
    rb_define_const(rb_cRenderable, "VERTICES_COUNT", INT2NUM(VERTICES_COUNT));
    rb_define_const(rb_cRenderable, "VERTICES_SIZE", INT2NUM(VERTICES_SIZE));
    rb_define_const(rb_cRenderable, "VERTICES_STRIDE", INT2NUM(VERTICES_STRIDE));
    rb_define_const(rb_cRenderable, "INDICES_COUNT", INT2NUM(INDICES_COUNT));
    rb_include_module(rb_cRenderable, rb_mGL);
    rb_define_alias(rb_cRenderable, "depth=", "z=");

    rb_cBlend = rb_define_class_under(parent, "Blend", rb_cObject);
    rb_define_alloc_func(rb_cBlend, RGSS_Blend_Alloc);
    rb_define_method3(rb_cBlend, "initialize", RGSS_Blend_Initialize, 3);
    rb_define_method0(rb_cBlend, "op", RGSS_Blend_GetOp, 0);
    rb_define_method0(rb_cBlend, "src", RGSS_Blend_GetSrc, 0);
    rb_define_method0(rb_cBlend, "dst", RGSS_Blend_GetDst, 0);
    rb_define_method1(rb_cBlend, "==", RGSS_Blend_Equal, 1);
    rb_define_method1(rb_cBlend, "eql?", RGSS_Blend_Equal, 1);

    RGSS_Blend *b = ALLOC(RGSS_Blend);
    b->op = GL_FUNC_ADD;
    b->src = GL_SRC_ALPHA;
    b->dst = GL_ONE_MINUS_SRC_ALPHA;
    rb_define_const(rb_cBlend, "DEFAULT", Data_Wrap_Struct(rb_cBlend, NULL, RUBY_DEFAULT_FREE, b));
}