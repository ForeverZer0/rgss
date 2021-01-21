#include "game.h"
#include "graphics.h"

#define VERTICES_COUNT  16
#define VERTICES_SIZE   (SIZEOF_FLOAT * VERTICES_COUNT)
#define VERTICES_STRIDE (SIZEOF_FLOAT * 4)
#define INDICES_COUNT   6

#define RGSS_SHADER RGSS_GAME.graphics.shader

VALUE rb_cEntity;
VALUE rb_cRenderable;
VALUE rb_cBlend;
VALUE rb_cViewport;
VALUE rb_cSprite;
VALUE rb_cPlane;

vec3 AXIS_Z = {0.0f, 0.0f, 1.0f};

typedef struct
{
    RGSS_Renderable base;
    VALUE texture;
    RGSS_Rect src_rect;
    VALUE viewport;
} RGSS_Sprite;

typedef struct {
    RGSS_Renderable base;
    GLuint texture;
    GLuint fbo;
    RGSS_Batch batch;
    vec4 *ortho;
    RGSS_Rect rect;
    RGSS_Color back_color;
} RGSS_Viewport;

typedef struct {
    RGSS_Renderable base;
    VALUE texture;
    VALUE viewport;
    GLuint sampler;
    vec2 zoom;
    vec2 scroll;
    vec2 origin;
} RGSS_Plane;


// TODO: Use direct pointer with RUBY_NEVER_FREE instead of new vector?


static void RGSS_Entity_Free(void *data)
{
    if (data)
    {
        RGSS_Entity *entity = data;
        if (entity->model)
            free(entity->model);
        xfree(data);
    }
}

void RGSS_Entity_Init(RGSS_Entity *entity)
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

void RGSS_Entity_Deinit(RGSS_Entity *entity)
{
    if (entity && entity->model)
    {
        free(entity->model);
        memset(entity, 0, sizeof(RGSS_Entity));
    }
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
    RGSS_Entity_Init(entity);
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

    // TODO: Set flag, only change when necessary?

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
    RGSS_ValueToVec3(value, vec);

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
    RGSS_ValueToVec3(value, vec);

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
    RGSS_ValueToVec3(value, vec);

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
    RGSS_ValueToVec3(value, vec);

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
    RGSS_ValueToVec3(value, vec);

    if (rb_respond_to(self, RGSS_ID_UPDATE_VERTICES))
        rb_funcall2(self, RGSS_ID_UPDATE_VERTICES, 0, NULL);

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

static VALUE RGSS_Entity_Rotate(int argc, VALUE *argv, VALUE self)
{
    VALUE degrees, pivot;
    rb_scan_args(argc, argv, "11", &degrees, &pivot);

    RGSS_Entity *entity = DATA_PTR(self);

    vec3 value;
    RGSS_ValueToVec3(pivot, value);

    entity->angle = NUM2DBL(degrees) * (M_PI / 180.0);

    if (NIL_P(pivot))
    {
        glm_vec3_copy(value, entity->pivot);
    }
    else
    {
        entity->pivot[0] = entity->size[0] * 0.5f;
        entity->pivot[1] = entity->size[1] * 0.5f;
    }
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
    
    if (rb_respond_to(self, RGSS_ID_UPDATE_VERTICES))
        rb_funcall2(self, RGSS_ID_UPDATE_VERTICES, 0, NULL);
    return value;
}

static VALUE RGSS_Entity_SetHeight(VALUE self, VALUE value)
{
    RGSS_Entity *entity = DATA_PTR(self);
    entity->size[1] = NUM2FLT(value);
    
    if (rb_respond_to(self, RGSS_ID_UPDATE_VERTICES))
        rb_funcall2(self, RGSS_ID_UPDATE_VERTICES, 0, NULL);
    return value;
}

static void RGSS_Renderable_Free(void *data)
{
    if (data)
    {
        RGSS_Renderable *obj = data;
        RGSS_Entity_Deinit(&obj->entity);
        xfree(data);
    }
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
    RGSS_Entity_Init(&obj->entity);
    return Data_Wrap_Struct(klass, RGSS_Renderable_Mark, RGSS_Renderable_Free, obj);
}

static VALUE RGSS_Renderable_Initialize(VALUE self, VALUE parent)
{
    if (NIL_P(parent))
    {
        parent = rb_funcall2(rb_mGraphics, RGSS_ID_BATCH, 0, NULL);
    }
    else if (rb_obj_is_kind_of(parent, rb_cViewport))
    {
        if (rb_obj_is_kind_of(self, rb_cViewport))
            rb_raise(rb_eRuntimeError, "a Viewport cannot be child of another Viewport");
        parent = rb_funcall2(parent, RGSS_ID_BATCH, 0, NULL);
    }
    else if (rb_obj_is_kind_of(parent, rb_cBatch) != Qtrue)
    {
        rb_raise(rb_eTypeError, "parent must be a Viewport, Batch, or NilClass");
    }

    // Indicates the Viewport or Graphics batch was nil, which should never be
    if (NIL_P(parent))
        rb_raise(rb_eRuntimeError, "an internal error occurred, please report this issue on GitHub");

    RGSS_Renderable *obj = DATA_PTR(self);
    RGSS_Renderable_Init(obj);

    obj->parent = parent;
    RGSS_Batch_Add(parent, self);


    // TODO Accept kwargs?

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
    return Data_Wrap_Struct(rb_cColor, NULL, RUBY_NEVER_FREE, &obj->color);
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
    return Data_Wrap_Struct(rb_cColor, NULL, RUBY_NEVER_FREE, &obj->flash_color);
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
    return Data_Wrap_Struct(rb_cTone, NULL, RUBY_NEVER_FREE, &obj->tone);
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

static VALUE RGSS_Renderable_UpdateBuffer(VALUE self, VALUE vertices)
{
    if (vertices != Qnil && rb_array_len(vertices) != VERTICES_COUNT)
        rb_raise(rb_eArgError, "number of vertices does not match VERTICES_SIZE");

    RGSS_Renderable *obj = DATA_PTR(self);
    glBindBuffer(GL_ARRAY_BUFFER, obj->vbo);

    if (vertices == Qnil)
    {
        glBufferSubData(GL_ARRAY_BUFFER, 0, VERTICES_SIZE, NULL);
    }
    else
    {
        GLfloat verts[VERTICES_COUNT];
        for (long i = 0; i < VERTICES_COUNT; i++)
            verts[i] = NUM2FLT(rb_ary_entry(vertices, i));
        
        glBufferSubData(GL_ARRAY_BUFFER, 0, VERTICES_SIZE, verts);
    }
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
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

static VALUE RGSS_Renderable_GetFlip(VALUE self)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    return INT2NUM(obj->flip);
}

static VALUE RGSS_Renderable_SetFlip(VALUE self, VALUE flip)
{
    RGSS_Renderable *obj = DATA_PTR(self);
    int value = NUM2INT(flip);
    if (value != obj->flip)
    {
        obj->flip = NUM2INT(flip);
        if (rb_respond_to(self, RGSS_ID_UPDATE_VERTICES))
            rb_funcall2(self, RGSS_ID_UPDATE_VERTICES, 0, NULL);
    }
    return flip;
}

static VALUE RGSS_Sprite_UpdateVertices(VALUE self)
{
    RGSS_Sprite *sprite = DATA_PTR(self);
    RGSS_Texture *tex = RTEST(sprite->texture) ? DATA_PTR(sprite->texture) : NULL;
    if (tex == NULL)
        return Qnil;

    GLfloat l, t, r, b, temp;
    l = (GLfloat) sprite->src_rect.x / (GLfloat) tex->width;
    t = (GLfloat) sprite->src_rect.y / (GLfloat) tex->height;
    r = (GLfloat) sprite->src_rect.width / (GLfloat) tex->width;
    b = (GLfloat) sprite->src_rect.height / (GLfloat) tex->height;

    if (RGSS_HAS_FLAG(sprite->base.flip, RGSS_FLIP_X))
    {
        temp = l;
        l = r;
        r = temp;
    }

    if (RGSS_HAS_FLAG(sprite->base.flip, RGSS_FLIP_Y))
    {
        temp = t;
        t = b;
        b = temp;
    }

    GLfloat vertices[VERTICES_COUNT] =
    {
        0.0f, 1.0f, l, b, // Bottom-Left
        1.0f, 0.0f, r, t, // Top-Right
        0.0f, 0.0f, l, t, // Top-Left
        1.0f, 1.0f, r, b, // Bottom-Right
    };

    glBindBuffer(GL_ARRAY_BUFFER, sprite->base.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, VERTICES_SIZE, vertices);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    return Qnil;
}

static VALUE RGSS_Sprite_GetSrcRect(VALUE self)
{
    RGSS_Sprite *sprite = DATA_PTR(self);
    RGSS_Rect *rect = ALLOC(RGSS_Rect);
    memcpy(rect, &sprite->src_rect, sizeof(RGSS_Rect));
    return Data_Wrap_Struct(rb_cRect, NULL, RUBY_DEFAULT_FREE, rect);
}

static VALUE RGSS_Sprite_SetSrcRect(VALUE self, VALUE rect)
{
    RGSS_Sprite *sprite = DATA_PTR(self);
    if (NIL_P(rect))
    {
        sprite->src_rect = (RGSS_Rect) { 0, 0, 0, 0 };
        glm_vec3_zero(sprite->base.entity.size);
    }
    else
    {
        RGSS_Rect *r = DATA_PTR(rect);
        memcpy(&sprite->src_rect, r, sizeof(RGSS_Rect));
        sprite->base.entity.size[0] = (float) r->width;
        sprite->base.entity.size[1] = (float) r->height;
    }
    RGSS_Sprite_UpdateVertices(self);
    return rect;
}

static VALUE RGSS_Sprite_GetTexture(VALUE self)
{
    RGSS_Sprite *sprite = DATA_PTR(self);
    return sprite->texture;
}

static VALUE RGSS_Sprite_SetTexture(VALUE self, VALUE texture)
{
    RGSS_Sprite *sprite = DATA_PTR(self);
    sprite->texture = texture;

    if (NIL_P(texture))
    {
        glm_vec3_zero(sprite->base.entity.size);
        sprite->src_rect = (RGSS_Rect) { 0, 0, 0, 0 };
    }
    else
    {
        RGSS_Texture *tex = DATA_PTR(texture);
        sprite->src_rect = (RGSS_Rect) { 0, 0, tex->width, tex->height };
        sprite->base.entity.size[0] = (float) tex->width;
        sprite->base.entity.size[1] = (float) tex->height;
        RGSS_Sprite_UpdateVertices(self);
    }
}

static VALUE RGSS_Sprite_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE viewport, opts;
    rb_scan_args(argc, argv, "01:", &viewport, &opts);

    rb_call_super(1, &viewport);
    RGSS_Renderable_VertexSetup(0, NULL, self);

    if (rb_obj_is_kind_of(viewport, rb_cViewport) == Qtrue)
    {
        RGSS_Sprite *sprite = DATA_PTR(self);
        sprite->viewport = viewport;
    }

    if (RTEST(opts))
    {
        VALUE opt;
        opt = rb_hash_aref(opts, STR2SYM("texture"));
        if (RTEST(opt))
            RGSS_Sprite_SetTexture(self, opt);
    }
    return self;
}

static void RGSS_Sprite_Mark(void *data)
{
    RGSS_Sprite *sprite = data;
    rb_gc_mark(sprite->texture);
    rb_gc_mark(sprite->viewport);
}

static VALUE RGSS_Sprite_Alloc(VALUE klass)
{
    RGSS_Sprite *sprite = ALLOC(RGSS_Sprite);
    memset(sprite, 0, sizeof(RGSS_Sprite));
    RGSS_Entity_Init(&sprite->base.entity);
    sprite->texture = Qnil;
    sprite->texture = Qnil;
    return Data_Wrap_Struct(klass, RGSS_Sprite_Mark, RGSS_Renderable_Free, sprite);
}

static VALUE RGSS_Sprite_Render(VALUE self, VALUE alpha)
{
    RGSS_Sprite *sprite = DATA_PTR(self);
    if (NIL_P(sprite->texture) || !sprite->base.visible || sprite->base.opacity < FLT_EPSILON)
        return Qnil;

    rb_call_super(1, &alpha);

    RGSS_Texture *tex = DATA_PTR(sprite->texture);
    if (tex->id == 0)
        rb_raise(rb_eRuntimeError, "disposed texture");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glBindVertexArray(sprite->base.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
    glBindVertexArray(GL_NONE);
    return Qnil;
}

static VALUE RGSS_Sprite_GetViewport(VALUE self)
{
    RGSS_Sprite *sprite = DATA_PTR(self);
    return sprite->viewport;
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

ATTR_ACCESSOR(RGSS_Blend, Op, op, INT2NUM, NUM2INT)
ATTR_ACCESSOR(RGSS_Blend, Src, src, INT2NUM, NUM2INT)
ATTR_ACCESSOR(RGSS_Blend, Dst, dst, INT2NUM, NUM2INT)

static VALUE RGSS_Blend_Equal(VALUE self, VALUE other)
{
    if (CLASS_OF(self) != CLASS_OF(other))
        return Qfalse;
    RGSS_Blend *b1 = DATA_PTR(self), *b2 = DATA_PTR(other);
    return RB_BOOL(b1->op == b2->op && b1->src == b2->src && b1->dst == b2->dst);
}

static void RGSS_Viewport_Mark(void *data)
{
    RGSS_Viewport *vp = data;

    VALUE obj;
    int i;

    vec_foreach(&vp->batch.items, obj, i)
    {
        rb_gc_mark(obj);
    }
}

static void RGSS_Viewport_Free(void *data)
{
    RGSS_Viewport *vp = data;
    vec_deinit(&vp->batch.items);
    RGSS_Entity_Deinit(&vp->base.entity);
    xfree(data);
}

static VALUE RGSS_Viewport_Alloc(VALUE klass)
{
    RGSS_Viewport *vp = ALLOC(RGSS_Viewport);
    memset(vp, 0, sizeof(RGSS_Viewport));
    RGSS_Entity_Init(&vp->base.entity);
    vec_init(&vp->batch.items);
    return Data_Wrap_Struct(klass, RGSS_Viewport_Mark, RGSS_Viewport_Free, vp);
}

static VALUE RGSS_Viewport_Initialize(int argc, VALUE *argv, VALUE self)
{   
    VALUE x, y, w, h;
    rb_scan_args(argc, argv, "04", &x, &y, &w, &h);

    VALUE batch = rb_funcall2(rb_mGraphics, RGSS_ID_BATCH, 0, NULL);
    rb_call_super(1, &batch);
    RGSS_Rect rect = {0};

    switch (argc)
    {
        case 0:
        {
            rect.width = (int) RGSS_GAME.graphics.resolution[0];
            rect.height = (int) RGSS_GAME.graphics.resolution[1];
            break;
        }
        case 1:
        {
            if (rb_obj_is_kind_of(x, rb_cRect) != Qtrue)
                rb_raise(rb_eTypeError, "%s is not a Rect", CLASS_NAME(x));
            memcpy(&rect, DATA_PTR(x), sizeof(RGSS_Rect));
            break;
        }
        case 2:
        {
            if (rb_obj_is_kind_of(x, rb_cPoint) != Qtrue)
                rb_raise(rb_eTypeError, "%s is not a Point", CLASS_NAME(x));
            if (rb_obj_is_kind_of(y, rb_cSize) != Qtrue)
                rb_raise(rb_eTypeError, "%s is not a Size", CLASS_NAME(y));

            memcpy(&rect.location, DATA_PTR(x), sizeof(RGSS_Point));
            memcpy(&rect.size, DATA_PTR(y), sizeof(RGSS_Size));
            break;
        }
        case 4:
        {
            rect.x = NUM2INT(x);
            rect.y = NUM2INT(y);
            rect.width = NUM2INT(w);
            rect.height = NUM2INT(h);            
            break;
        }
        default: rb_raise(rb_eArgError, "wrong number of arguments (given %d, expected 0, 1, 2, or 4)");
    }

    RGSS_SizeNotEmpty(rect.width, rect.height);
    
    RGSS_Viewport *vp = DATA_PTR(self);
    vp->base.entity.position[0] = rect.x;
    vp->base.entity.position[1] = rect.y;
    vp->base.entity.size[0] = rect.width;
    vp->base.entity.size[1] = rect.height;

    glGenTextures(1, &vp->texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, vp->texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rect.width, rect.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenFramebuffers(1, &vp->fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, vp->fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, vp->texture, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    GLfloat vertices[VERTICES_COUNT] = 
    {
        0.0f, 1.0f, 0.0f, 1.0f, // Bottom-Left
        1.0f, 0.0f, 1.0f, 0.0f, // Top-Right
        0.0f, 0.0f, 0.0f, 0.0f, // Top-Left
        1.0f, 1.0f, 1.0f, 1.0f, // Bottom-Right
    };

    VALUE ary = rb_ary_new_capa(VERTICES_COUNT);
    for (long i = 0; i < VERTICES_COUNT; i++)
        rb_ary_store(ary, i, DBL2NUM(vertices[i]));

    VALUE args[3] = { ary, Qnil, INT2NUM(GL_STATIC_DRAW) };
    RGSS_Renderable_VertexSetup(3, args, self);

    vp->ortho = RGSS_MAT4_NEW;
    glm_ortho(0.0f, (GLfloat) rect.width, 0.0f, (GLfloat) rect.height, -1.0f, 1.0f, vp->ortho);
    vp->rect = rect;
    return self;
}

static VALUE RGSS_Viewport_Render(VALUE self, VALUE alpha)
{
    RGSS_Viewport *vp = DATA_PTR(self);
    if (!vp->base.visible || vp->base.opacity < FLT_EPSILON)
        return Qnil;

    // Check if viewport has been disposed or not initialized
    if (vp->fbo == 0 || vp->texture == 0)
        rb_raise(rb_eRuntimeError, "disposed viewport");

    // Setup off-screen framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, vp->fbo);
    glClearColor(vp->back_color[0], vp->back_color[1], vp->back_color[2], vp->back_color[3]); 
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, vp->rect.width, vp->rect.height);
    glScissor(0, 0, vp->rect.width, vp->rect.height);

    // Configure the projection matrix to that of the viewport
    glBindBuffer(GL_UNIFORM_BUFFER, RGSS_GAME.graphics.ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, RGSS_MAT4_SIZE, vp->ortho[0]);

    // Render all the children of the sprite onto the bound framebuffer
    VALUE obj;
    int i;
    vec_foreach(&vp->batch.items, obj, i)
    {
        rb_funcall2(obj, RGSS_ID_RENDER, 1, &alpha);
    }

    // Restore rendering to the screen, and reapply color, viewport, projection, etc.
    RGSS_Graphics_Restore(rb_mGraphics);
    glBindBuffer(GL_UNIFORM_BUFFER, RGSS_GAME.graphics.ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, RGSS_MAT4_SIZE, RGSS_GAME.graphics.projection);
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);

    // Render the viewport's texture normally
    rb_call_super(1, &alpha);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, vp->texture);
    glBindVertexArray(vp->base.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
    glBindVertexArray(GL_NONE);
    return Qnil;
}

static VALUE RGSS_Viewport_Dispose(VALUE self)
{
    rb_call_super(0, NULL);
    RGSS_Viewport *vp = DATA_PTR(self);
    if (vp->fbo)
    {
        glDeleteFramebuffers(1, &vp->fbo);
        vp->fbo = GL_NONE;
    }
    if (vp->texture)
    {
        glDeleteTextures(1, &vp->texture);
        vp->texture = GL_NONE;
    }
    return Qnil;
}

static VALUE RGSS_Viewport_GetBatch(VALUE self)
{
    RGSS_Viewport *vp = DATA_PTR(self);
    return Data_Wrap_Struct(rb_cBatch, NULL, RUBY_NEVER_FREE, &vp->batch);
}

static VALUE RGSS_Viewport_GetBackColor(VALUE self)
{
    RGSS_Viewport *vp = DATA_PTR(self);
    float *color = RGSS_VEC4_NEW;
    memcpy(color, &vp->back_color, sizeof(RGSS_Color));
    return Data_Wrap_Struct(rb_cColor, NULL, free, color);
}

static VALUE RGSS_Viewport_SetBackColor(VALUE self, VALUE color)
{
    RGSS_Viewport *vp = DATA_PTR(self);
    if (rb_obj_is_kind_of(color, rb_cColor))
    {
        glm_vec4_copy(DATA_PTR(color), vp->back_color);
    }
    else
    {
        glm_vec4_zero(vp->back_color);
    }
    return color;
}

static void RGSS_Plane_Mark(void *data)
{
    RGSS_Plane *plane = data;
    rb_gc_mark(plane->texture);
    rb_gc_mark(plane->viewport);
}

static VALUE RGSS_Plane_Alloc(VALUE klass)
{
    RGSS_Plane *plane = ALLOC(RGSS_Plane);
    memset(plane, 0, sizeof(RGSS_Plane));
    RGSS_Entity_Init(&plane->base.entity);
    plane->texture = Qnil;
    plane->viewport = Qnil;
    return Data_Wrap_Struct(klass, RGSS_Plane_Mark, RGSS_Renderable_Free, plane);
}

static VALUE RGSS_Plane_Dispose(VALUE self)
{
    rb_call_super(0, NULL);
    RGSS_Plane *plane = DATA_PTR(self);
    if (plane->sampler)
    {
        glDeleteSamplers(1, &plane->sampler);
        plane->sampler = GL_NONE;
    }
    return Qnil;
}

static VALUE RGSS_Plane_UpdateVertices(VALUE self)
{
    RGSS_Plane *plane = DATA_PTR(self);
    RGSS_Texture *tex = RTEST(plane->texture) ? DATA_PTR(plane->texture) : NULL;
    if (tex == NULL)
        return Qnil;

    GLfloat l, t, r, b;

    if (glm_eq(plane->base.entity.size[0], 0.0f))
    {
        l = 0.0f;
        r = 0.0f;
    }
    else
    {
        l = (plane->origin[0] / plane->base.entity.size[0]) * plane->zoom[0];
        r = l + ((plane->base.entity.size[0] / (GLfloat) tex->width) * plane->zoom[0]);
        if (RGSS_HAS_FLAG(plane->base.flip, RGSS_FLIP_X))
        {
            GLfloat temp_l = l;
            l = r;
            r = temp_l;
        }
    }

    if (glm_eq(plane->base.entity.size[1], 0.0f))
    {
        t = 0.0f;
        b = 0.0f;
    }
    else
    {
        t = (plane->origin[1] / plane->base.entity.size[1]) * plane->zoom[1];
        b = t + ((plane->base.entity.size[1] / (GLfloat) tex->width) * plane->zoom[1]);
        if (RGSS_HAS_FLAG(plane->base.flip, RGSS_FLIP_Y))
        {
            GLfloat temp_t = t;
            t = b;
            b = temp_t;
        }
    }

    GLfloat vertices[VERTICES_COUNT] =
    {
        0.0f, 1.0f, l, b, // Bottom-Left
        1.0f, 0.0f, r, t, // Top-Right
        0.0f, 0.0f, l, t, // Top-Left
        1.0f, 1.0f, r, b, // Bottom-Right
    };

    glBindBuffer(GL_ARRAY_BUFFER, plane->base.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, VERTICES_SIZE, vertices);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    return Qnil;
}

static VALUE RGSS_Plane_GetTexture(VALUE self)
{
    RGSS_Plane *plane = DATA_PTR(self);
    return plane->texture;
}

static VALUE RGSS_Plane_SetTexture(VALUE self, VALUE texture)
{
    RGSS_Plane *plane = DATA_PTR(self);
    plane->texture = texture;

    if (NIL_P(texture))
    {
        glm_vec3_zero(plane->base.entity.size);
    }
    else
    {
        RGSS_Texture *tex = DATA_PTR(texture);
        plane->base.entity.size[0] = (float) tex->width;
        plane->base.entity.size[1] = (float) tex->height;
        RGSS_Plane_UpdateVertices(self);
    }
}

static VALUE RGSS_Plane_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE viewport, opts;
    rb_scan_args(argc, argv, "01:", &viewport, &opts);

    rb_call_super(1, &viewport);
    RGSS_Renderable_VertexSetup(0, NULL, self);

    RGSS_Plane *plane = DATA_PTR(self);
    if (rb_obj_is_kind_of(viewport, rb_cViewport) == Qtrue)
    {
        plane->viewport = viewport;
    }

    glGenSamplers(1, &plane->sampler);
    glSamplerParameteri(plane->sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(plane->sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameteri(plane->sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glSamplerParameteri(plane->sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glm_vec2_zero(plane->scroll);
    glm_vec2_zero(plane->origin);
    glm_vec2_one(plane->zoom);

    if (RTEST(opts))
    {
        VALUE opt;
        opt = rb_hash_aref(opts, STR2SYM("texture"));
        if (RTEST(opt))
            RGSS_Sprite_SetTexture(self, opt);
    }

    return self;
}

static VALUE RGSS_Plane_GetZoom(VALUE self)
{
    RGSS_Plane *plane = DATA_PTR(self);
    return RGSS_Vec2_New(plane->zoom[0], plane->zoom[1]);
}

static VALUE RGSS_Plane_SetZoom(VALUE self, VALUE value)
{
    RGSS_Plane *plane = DATA_PTR(self);
    if (rb_obj_is_kind_of(value, rb_cVec2))
    {
        glm_vec2_copy(DATA_PTR(value), plane->zoom);
    }
    else
    {
        glm_vec2_one(plane->zoom);
    }
    return value;
}

static VALUE RGSS_Plane_GetScroll(VALUE self)
{
    RGSS_Plane *plane = DATA_PTR(self);
    return RGSS_Vec2_New(plane->scroll[0], plane->scroll[1]);
}

static VALUE RGSS_Plane_SetScroll(VALUE self, VALUE value)
{
    RGSS_Plane *plane = DATA_PTR(self);
    if (rb_obj_is_kind_of(value, rb_cVec2))
    {
        glm_vec2_copy(DATA_PTR(value), plane->scroll);
    }
    else
    {
        glm_vec2_zero(plane->scroll);
    }
    return value;
}

static VALUE RGSS_Plane_GetOrigin(VALUE self)
{
    RGSS_Plane *plane = DATA_PTR(self);
    return RGSS_Vec2_New(plane->origin[0], plane->origin[1]);
}

static VALUE RGSS_Plane_SetOrigin(VALUE self, VALUE value)
{
    RGSS_Plane *plane = DATA_PTR(self);
    if (rb_obj_is_kind_of(value, rb_cVec2))
    {
        glm_vec2_copy(DATA_PTR(value), plane->origin);
    }
    else
    {
        glm_vec2_zero(plane->origin);
    }
    return value;
}

static VALUE RGSS_Plane_Update(VALUE self, VALUE delta)
{
    RGSS_Plane *plane = DATA_PTR(self);

    vec2 vec;
    glm_vec2_scale(plane->scroll, NUM2FLT(delta), vec);
    glm_vec2_add(vec, plane->origin, plane->origin);
    RGSS_Plane_UpdateVertices(self);

    return rb_call_super(1, &delta);
}

static VALUE RGSS_Plane_Render(VALUE self, VALUE alpha)
{

    RGSS_Plane *plane = DATA_PTR(self);
    if (NIL_P(plane->texture) || !plane->base.visible || plane->base.opacity < FLT_EPSILON)
        return Qnil;

    rb_call_super(1, &alpha);

    RGSS_Texture *tex = DATA_PTR(plane->texture);
    if (tex->id == 0)
        rb_raise(rb_eRuntimeError, "disposed texture");

    glBindSampler(0, plane->sampler);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glBindVertexArray(plane->base.vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);
    glBindVertexArray(GL_NONE);
    glBindSampler(0, GL_NONE);
    return Qnil;
}

void RGSS_Init_Entity(VALUE parent)
{
    rb_cEntity = rb_define_class_under(parent, "Entity", rb_cObject);
    rb_define_alloc_func(rb_cEntity, RGSS_Entity_Alloc);

    DEFINE_ACCESSOR(rb_cEntity, RGSS_Entity, Model, "model");
    DEFINE_ACCESSOR(rb_cEntity, RGSS_Entity, Position, "position");
    DEFINE_ACCESSOR(rb_cEntity, RGSS_Entity, Velocity, "velocity");
    DEFINE_ACCESSOR(rb_cEntity, RGSS_Entity, Scale, "scale");
    DEFINE_ACCESSOR(rb_cEntity, RGSS_Entity, Pivot, "pivot");
    DEFINE_ACCESSOR(rb_cEntity, RGSS_Entity, Angle, "angle");
    DEFINE_ACCESSOR(rb_cEntity, RGSS_Entity, X, "x");
    DEFINE_ACCESSOR(rb_cEntity, RGSS_Entity, Y, "y");
    DEFINE_ACCESSOR(rb_cEntity, RGSS_Entity, Z, "z");
    DEFINE_ACCESSOR(rb_cEntity, RGSS_Entity, Location, "location");
    rb_define_method1(rb_cEntity, "update", RGSS_Entity_Update, 1);
    rb_define_method0(rb_cEntity, "bounds", RGSS_Entity_GetBounds, 0);
    rb_define_methodm1(rb_cEntity, "rotate", RGSS_Entity_Rotate, -1);
    rb_define_method0(rb_cEntity, "size", RGSS_Entity_GetSize, 0);
    rb_define_method0(rb_cEntity, "width", RGSS_Entity_GetWidth, 0);
    rb_define_method0(rb_cEntity, "height", RGSS_Entity_GetHeight, 0);
    rb_define_protected_method1(rb_cEntity, "size=", RGSS_Entity_SetSize, 1);
    rb_define_protected_method1(rb_cEntity, "width=", RGSS_Entity_SetWidth, 1);
    rb_define_protected_method1(rb_cEntity, "height=", RGSS_Entity_SetHeight, 1);
    rb_define_alias(rb_cEntity, "depth", "z");

    rb_cRenderable = rb_define_class_under(parent, "Renderable", rb_cEntity);
    rb_define_alloc_func(rb_cRenderable, RGSS_Renderable_Alloc);
    DEFINE_ACCESSOR(rb_cRenderable, RGSS_Renderable, Color, "color");
    DEFINE_ACCESSOR(rb_cRenderable, RGSS_Renderable, FlashColor, "flash_color");
    DEFINE_ACCESSOR(rb_cRenderable, RGSS_Renderable, FlashDuration, "flash_duration");
    DEFINE_ACCESSOR(rb_cRenderable, RGSS_Renderable, Tone, "tone");
    DEFINE_ACCESSOR(rb_cRenderable, RGSS_Renderable, Opacity, "opacity");
    DEFINE_ACCESSOR(rb_cRenderable, RGSS_Renderable, Hue, "hue");
    DEFINE_ACCESSOR(rb_cRenderable, RGSS_Renderable, Visible, "visible");
    DEFINE_ACCESSOR(rb_cRenderable, RGSS_Renderable, Blend, "blend");
    DEFINE_ACCESSOR(rb_cRenderable, RGSS_Renderable, Flip, "flip");
    rb_define_method1(rb_cRenderable, "initialize", RGSS_Renderable_Initialize, 1);
    rb_define_method1(rb_cRenderable, "update", RGSS_Renderable_Update, 1);
    rb_define_method0(rb_cRenderable, "dispose", RGSS_Renderable_Dispose, 0);
    rb_define_method0(rb_cRenderable, "disposed?", RGSS_Renderable_IsDisposed, 0);
    rb_define_method2(rb_cRenderable, "flash", RGSS_Renderable_Flash, 2);
    rb_define_method0(rb_cRenderable, "flashing?", RGSS_Renderable_IsFlashing, 0);
    rb_define_method1(rb_cRenderable, "z=", RGSS_Renderable_SetDepth, 1);
    rb_define_method1(rb_cRenderable, "render", RGSS_Renderable_Render, 1);
    rb_define_protected_method0(rb_cRenderable, "parent", RGSS_Renderable_GetParent, 0);
    rb_define_protected_methodm1(rb_cRenderable, "vertex_setup", RGSS_Renderable_VertexSetup, -1);
    rb_define_protected_method1(rb_cRenderable, "update_buffer", RGSS_Renderable_UpdateBuffer, 1);
    rb_define_protected_method0(rb_cRenderable, "vao", RGSS_Renderable_GetVAO, 0);
    rb_define_protected_method0(rb_cRenderable, "vbo", RGSS_Renderable_GetVBO, 0);
    rb_define_protected_method0(rb_cRenderable, "ebo", RGSS_Renderable_GetEBO, 0);
    rb_define_const(rb_cRenderable, "VERTICES_COUNT", INT2NUM(VERTICES_COUNT));
    rb_define_const(rb_cRenderable, "VERTICES_SIZE", INT2NUM(VERTICES_SIZE));
    rb_define_const(rb_cRenderable, "VERTICES_STRIDE", INT2NUM(VERTICES_STRIDE));
    rb_define_const(rb_cRenderable, "INDICES_COUNT", INT2NUM(INDICES_COUNT));
    rb_define_const(rb_cRenderable, "FLIP_NONE", INT2NUM(RGSS_FLIP_NONE));
    rb_define_const(rb_cRenderable, "FLIP_X", INT2NUM(RGSS_FLIP_X));
    rb_define_const(rb_cRenderable, "FLIP_Y", INT2NUM(RGSS_FLIP_Y));
    rb_define_const(rb_cRenderable, "FLIP_BOTH", INT2NUM(RGSS_FLIP_BOTH));
    rb_include_module(rb_cRenderable, rb_mGL);
    rb_define_alias(rb_cRenderable, "depth=", "z=");

    rb_cBlend = rb_define_class_under(parent, "Blend", rb_cObject);
    rb_define_alloc_func(rb_cBlend, RGSS_Blend_Alloc);
    rb_define_method3(rb_cBlend, "initialize", RGSS_Blend_Initialize, 3);
    DEFINE_ACCESSOR(rb_cBlend, RGSS_Blend, Op, "op");
    DEFINE_ACCESSOR(rb_cBlend, RGSS_Blend, Src, "src");
    DEFINE_ACCESSOR(rb_cBlend, RGSS_Blend, Dst, "dst");
    rb_define_method1(rb_cBlend, "==", RGSS_Blend_Equal, 1);
    rb_define_method1(rb_cBlend, "eql?", RGSS_Blend_Equal, 1);

    rb_cSprite = rb_define_class_under(parent, "Sprite", rb_cRenderable);
    rb_define_alloc_func(rb_cSprite, RGSS_Sprite_Alloc);
    rb_define_methodm1(rb_cSprite, "initialize", RGSS_Sprite_Initialize, -1);
    rb_define_method0(rb_cSprite, "viewport", RGSS_Sprite_GetViewport, 0);
    DEFINE_ACCESSOR(rb_cSprite, RGSS_Sprite, SrcRect, "src_rect");
    DEFINE_ACCESSOR(rb_cSprite, RGSS_Sprite, Texture, "texture");
    rb_define_method1(rb_cSprite, "render", RGSS_Sprite_Render, 1);
    rb_define_protected_method0(rb_cSprite, "update_vertices", RGSS_Sprite_UpdateVertices, 0);

    rb_cViewport = rb_define_class_under(parent, "Viewport", rb_cRenderable);
    rb_define_alloc_func(rb_cViewport, RGSS_Viewport_Alloc);
    rb_define_methodm1(rb_cViewport, "initialize", RGSS_Viewport_Initialize, -1);
    rb_define_method1(rb_cViewport, "render", RGSS_Viewport_Render, 1);
    rb_define_protected_method0(rb_cViewport, "batch", RGSS_Viewport_GetBatch, 0);
    rb_define_method0(rb_cViewport, "dispose", RGSS_Viewport_Dispose, 0);
    DEFINE_ACCESSOR(rb_cViewport, RGSS_Viewport, BackColor, "back_color");

    rb_cPlane = rb_define_class_under(parent, "Plane", rb_cRenderable);
    rb_define_alloc_func(rb_cPlane, RGSS_Plane_Alloc);
    rb_define_method0(rb_cPlane, "dispose", RGSS_Plane_Dispose, 0);
    rb_define_methodm1(rb_cPlane, "initialize", RGSS_Plane_Initialize, -1);
    DEFINE_ACCESSOR(rb_cPlane, RGSS_Plane, Zoom, "zoom");
    DEFINE_ACCESSOR(rb_cPlane, RGSS_Plane, Scroll, "scroll");
    DEFINE_ACCESSOR(rb_cPlane, RGSS_Plane, Origin, "origin");
    DEFINE_ACCESSOR(rb_cPlane, RGSS_Plane, Texture, "texture");
    rb_define_method1(rb_cPlane, "update", RGSS_Plane_Update, 1);
    rb_define_method1(rb_cPlane, "render", RGSS_Plane_Render, 1);
    // Redefine as public
    rb_define_method1(rb_cPlane, "size=", RGSS_Entity_SetSize, 1);
    rb_define_method1(rb_cPlane, "width=", RGSS_Entity_SetWidth, 1);
    rb_define_method1(rb_cPlane, "height=", RGSS_Entity_SetHeight, 1);
    rb_define_protected_method0(rb_cPlane, "update_vertices", RGSS_Plane_UpdateVertices, 0);


    // TODO: Origin for viewport, offset children from it each draw/update?

    RGSS_Blend *b = ALLOC(RGSS_Blend);
    b->op = GL_FUNC_ADD;
    b->src = GL_SRC_ALPHA;
    b->dst = GL_ONE_MINUS_SRC_ALPHA;
    rb_define_const(rb_cBlend, "DEFAULT", Data_Wrap_Struct(rb_cBlend, NULL, RUBY_DEFAULT_FREE, b));

    rb_undef(rb_cViewport, rb_intern("width="));
    rb_undef(rb_cViewport, rb_intern("height="));
    rb_undef(rb_cViewport, rb_intern("size="));
}