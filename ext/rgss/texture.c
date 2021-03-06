#include "game.h"
#include "graphics.h"

VALUE rb_cTexture;

vec4 RGSS_VEC4_ZERO;
mat4 RGSS_BLIT_MODEL;


#define RGSS_TEXTURE_OPTS "@default_options"
#define RGSS_ASSERT_TEXTURE(texture)                                                                                   \
    if ((texture)->id == 0)                                                                                            \
    rb_raise(rb_eRuntimeError, "disposed texture")

static VALUE RGSS_Texture_Alloc(VALUE klass)
{
    RGSS_Texture *tex = ALLOC(RGSS_Texture);
    memset(tex, 0, sizeof(RGSS_Texture));
    return Data_Wrap_Struct(klass, NULL, RUBY_DEFAULT_FREE, tex);
}

static VALUE RGSS_Texture_Dispose(VALUE self)
{
    RGSS_Texture *tex = DATA_PTR(self);
    if (tex->fbo)
    {
        glDeleteFramebuffers(1, &tex->fbo);
        tex->fbo = GL_NONE;
    }
    if (tex->id)
    {
        glDeleteTextures(1, &tex->id);
        tex->id = GL_NONE;
    }
    return Qnil;
}

static VALUE RGSS_Texture_IsDisposed(VALUE self)
{
    RGSS_Texture *tex = DATA_PTR(self);
    return RB_BOOL(tex->id != GL_NONE);
}

static inline void RGSS_Texture_BindFramebuffer(RGSS_Texture *texture)
{
    if (texture->fbo == 0)
    {
        glGenFramebuffers(1, &texture->fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, texture->fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->id, 0);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, texture->fbo);
    }
}

static VALUE RGSS_Texture_GetDefaultOptions(VALUE klass)
{
    VALUE opts = rb_iv_get(klass, RGSS_TEXTURE_OPTS);
    if (!RB_TYPE_P(opts, T_HASH))
    {
        opts = rb_hash_new();
        rb_hash_aset(opts, STR2SYM("format"), INT2NUM(GL_RGBA));
        rb_hash_aset(opts, STR2SYM("internal"), INT2NUM(GL_RGBA));
        rb_hash_aset(opts, STR2SYM("type"), INT2NUM(GL_UNSIGNED_BYTE));
        rb_hash_aset(opts, STR2SYM("wrap_s"), INT2NUM(GL_CLAMP_TO_EDGE));
        rb_hash_aset(opts, STR2SYM("wrap_t"), INT2NUM(GL_CLAMP_TO_EDGE));
        rb_hash_aset(opts, STR2SYM("min_filter"), INT2NUM(GL_NEAREST));
        rb_hash_aset(opts, STR2SYM("max_filter"), INT2NUM(GL_LINEAR));
        rb_iv_set(klass, RGSS_TEXTURE_OPTS, opts);
    }
    return opts;
}

static VALUE RGSS_Texture_GetID(VALUE self)
{
    RGSS_Texture *tex = DATA_PTR(self);
    RGSS_ASSERT_TEXTURE(tex);
    return INT2NUM(tex->id);
}

static VALUE RGSS_Texture_GetFBO(VALUE self)
{
    RGSS_Texture *tex = DATA_PTR(self);
    RGSS_ASSERT_TEXTURE(tex);
    return INT2NUM(tex->fbo);
}

static VALUE RGSS_Texture_GetWidth(VALUE self)
{
    RGSS_Texture *tex = DATA_PTR(self);
    RGSS_ASSERT_TEXTURE(tex);
    return INT2NUM(tex->width);
}

static VALUE RGSS_Texture_GetHeight(VALUE self)
{
    RGSS_Texture *tex = DATA_PTR(self);
    RGSS_ASSERT_TEXTURE(tex);
    return INT2NUM(tex->height);
}

static VALUE RGSS_Texture_GetSize(VALUE self)
{
    RGSS_Texture *tex = DATA_PTR(self);
    RGSS_ASSERT_TEXTURE(tex);
    return RGSS_Size_New(tex->width, tex->height);
}

static VALUE RGSS_Texture_GetRect(VALUE self)
{
    RGSS_Texture *tex = DATA_PTR(self);
    RGSS_ASSERT_TEXTURE(tex);
    return RGSS_Rect_New(0, 0, tex->width, tex->height);
}

static void RGSS_Texture_Generate(int width, int height, void *data, VALUE opts, RGSS_Texture *texture)
{
    texture->width = width;
    texture->height = height;
    texture->fbo = 0;

    GLenum format, internal, type, wrap_s, wrap_t, min_filter, max_filter;
    RGSS_ParseOpt(opts, "format", GL_RGBA, &format);
    RGSS_ParseOpt(opts, "internal", GL_RGBA, &internal);
    RGSS_ParseOpt(opts, "type", GL_UNSIGNED_BYTE, &type);
    RGSS_ParseOpt(opts, "wrap_s", GL_CLAMP_TO_EDGE, &wrap_s);
    RGSS_ParseOpt(opts, "wrap_t", GL_CLAMP_TO_EDGE, &wrap_t);
    RGSS_ParseOpt(opts, "min_filter", GL_NEAREST, &min_filter);
    RGSS_ParseOpt(opts, "max_filter", GL_LINEAR, &max_filter);

    glGenTextures(1, &texture->id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, internal, type, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_filter);
}

static VALUE RGSS_Texture_Bind(int argc, VALUE *argv, VALUE self)
{
    VALUE index;
    rb_scan_args(argc, argv, "01", &index);

    GLenum unit = GL_TEXTURE0 + (FIXNUM_P(index) ? NUM2INT(index) : 0);

    RGSS_Texture *tex = DATA_PTR(self);
    RGSS_ASSERT_TEXTURE(tex);

    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    if (rb_block_given_p())
    {
        rb_yield(Qundef);
        glActiveTexture(unit);
        glBindTexture(GL_TEXTURE_2D, GL_NONE);
    }
    return self;
}

static VALUE RGSS_Texture_Unbind(VALUE klass, VALUE index)
{
    GLenum unit = GL_TEXTURE0 + NUM2INT(index);
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, GL_NONE);
    return Qnil;
}

static VALUE RGSS_Texture_FromID(VALUE klass, VALUE id, VALUE width, VALUE height)
{
    RGSS_Texture *tex = ALLOC(RGSS_Texture);
    tex->id = NUM2UINT(id);
    tex->width = NUM2INT(width);
    tex->height = NUM2INT(height);

    if (!glIsTexture(tex->id))
        rb_raise(rb_eArgError, "invalid texture ID");
    RGSS_SizeNotEmpty(tex->width, tex->height);

    return Data_Wrap_Struct(klass, NULL, RUBY_DEFAULT_FREE, tex);
}

static VALUE RGSS_Texture_Load(int argc, VALUE *argv, VALUE klass)
{
    VALUE path, opts;
    rb_scan_args(argc, argv, "1:", &path, &opts);

    if (NIL_P(path))
        rb_raise(rb_eArgError, "path cannot be nil");

    int width, height;
    unsigned char *pixels;

    RGSS_Texture *tex = ALLOC(RGSS_Texture);
    RGSS_Image_Load(StringValueCStr(path), &width, &height, &pixels);
    RGSS_Texture_Generate(width, height, pixels, opts, tex);
    xfree(pixels);

    return Data_Wrap_Struct(klass, NULL, RUBY_DEFAULT_FREE, tex);
}

static VALUE RGSS_Texture_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE width, height, data, opts;
    rb_scan_args(argc, argv, "21:", &width, &height, &data, &opts);

    int w = NUM2INT(width);
    int h = NUM2INT(height);
    RGSS_SizeNotEmpty(w, h);

    RGSS_Texture *tex = DATA_PTR(self);
    void *pixels = NULL;
    int free_pixels = false;

    if (RTEST(data))
        pixels = RGSS_ValuePointer(data);
    else if (RTEST(opts))
    {
        VALUE color = rb_hash_aref(opts, STR2SYM("color"));
        if (RTEST(color))
        {
            unsigned int packed;
            RGSS_PackColor(DATA_PTR(color), &packed);

            long len = w * h;
            pixels = xmalloc(len * sizeof(unsigned int));
            unsigned int *ptr = pixels;
            free_pixels = true;

            for (long i = 0; i < len; i++)
                ptr[i] = packed;
        }
    }

    RGSS_Texture_Generate(w, h, pixels, opts, tex);
    if (free_pixels)
        xfree(pixels);

    return self;
}

static VALUE RGSS_Texture_ToImage(VALUE self)
{
    RGSS_Texture *tex = DATA_PTR(self);
    RGSS_ASSERT_TEXTURE(tex);

    unsigned char *pixels = xmalloc(tex->width * tex->height * sizeof(unsigned int));
    RGSS_Texture_BindFramebuffer(tex);
    glReadPixels(0, 0, tex->width, tex->height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);

    return RGSS_Image_New(tex->width, tex->height, pixels);
}

#define RGSS_SHADER RGSS_GAME.graphics.shader

static void RGSS_Texture_BlitImpl(RGSS_Texture *dst, RGSS_Texture *src, RGSS_Rect *src_rect, RGSS_Rect *dst_rect, float opacity)
{
    if (dst_rect->width < 1 || dst_rect->height < 1)
        return;
    RGSS_SizeNotEmpty(src_rect->width, src_rect->height);

    // Setup geometry of rendering area
    int x = dst_rect->x;
    int y = dst_rect->y;
    int w = dst_rect->width;
    int h = dst_rect->height;

    mat4 mat;
    glm_ortho(x, x + w, y, y + h, -1.0f, 1.0f, mat);
    glBindBuffer(GL_UNIFORM_BUFFER, RGSS_GAME.graphics.ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, RGSS_MAT4_SIZE, mat);
    glViewport(x, y, w, h);
    glScissor(x, y, w, h);

    GLfloat l, t, r, b;
    l = (GLfloat) src_rect->x / src->width;
    t = (GLfloat) src_rect->y / src->height;
    r = (GLfloat) src_rect->width /  src->width;
    b = (GLfloat) src_rect->height / src->height;
    
    GLfloat vertices[] =
    {
        0.0f, 1.0f, l, b, // Bottom-Left
        1.0f, 0.0f, r, t, // Top-Right
        0.0f, 0.0f, l, t, // Top-Left
        1.0f, 1.0f, r, b, // Bottom-Right
    };
    glBindBuffer(GL_ARRAY_BUFFER, RGSS_BLIT_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    // Bind the framebuffer of the texture to render to, then bind source texture to render
    RGSS_Texture_BindFramebuffer(dst);
    vec3 scale = { src_rect->width, src_rect->height, 0 };
    glm_scale_make(RGSS_BLIT_MODEL, scale);

    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(RGSS_SHADER.id);
    glUniformMatrix4fv(RGSS_SHADER.model, 1, GL_FALSE, RGSS_BLIT_MODEL);
    glUniform4fv(RGSS_SHADER.color, 1, RGSS_VEC4_ZERO);
    glUniform4fv(RGSS_SHADER.tone,  1, RGSS_VEC4_ZERO);
    glUniform4fv(RGSS_SHADER.flash, 1, RGSS_VEC4_ZERO);
    glUniform1f(RGSS_SHADER.hue, 0.0f);
    glUniform1f(RGSS_SHADER.opacity, opacity); // TODO

    // Render 
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, src->id);
    glBindVertexArray(RGSS_BLIT_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, NULL);

    // Restore rendering to screen 
    RGSS_Graphics_Restore(rb_mGraphics);
}

static VALUE RGSS_Dexture_DrawText(int argc, VALUE *argv, VALUE self)
{
    VALUE a0, a1, a2, a3, a4, opts;
    rb_scan_args(argc, argv, "23:", &a0, &a1, &a2, &a3, &a4, &opts);

    RGSS_Rect dst_rect, src_rect;
    VALUE text;

    switch (argc)
    {
        case 2:
        {
            if (rb_obj_is_kind_of(a0, rb_cRect) != Qtrue)
                rb_raise(rb_eArgError, "%s is not a Rect", CLASS_NAME(a0));
            memcpy(&dst_rect, DATA_PTR(a0), sizeof(RGSS_Rect));
            text = a1;
            break;
        }
        case 3:
        {
            if (rb_obj_is_kind_of(a0, rb_cIVec2) != Qtrue)
                rb_raise(rb_eArgError, "%s is not a Point", CLASS_NAME(a0));
            if (rb_obj_is_kind_of(a0, rb_cIVec2) != Qtrue)
                rb_raise(rb_eArgError, "%s is not a Size", CLASS_NAME(a1));
            memcpy(&dst_rect.location, DATA_PTR(a0), sizeof(RGSS_Point));
            memcpy(&dst_rect.size, DATA_PTR(a1), sizeof(RGSS_Size));
            text = a3;
            break;
        }
        case 5:
        {
            dst_rect.x = NUM2INT(a0);
            dst_rect.y = NUM2INT(a1);
            dst_rect.width = NUM2INT(a2);
            dst_rect.height = NUM2INT(a3);
            text = a4;
            break; 
        }
        default: rb_raise(rb_eArgError, "wrong number of arguments (given %d, expected 2, 3, or 5)", argc);
    }

    RGSS_SizeNotEmpty(dst_rect.width, dst_rect.height);
    // src_rect = (RGSS_Rect) 

    // TODO: Parse opts, use vertical aligm, measure first, place appropriatly on image

    return self;
}

static VALUE RGSS_Texture_Blit(int argc, VALUE *argv, VALUE self)
{
    
    // Quite a few argument combinations can be supplied.
    //
    // The first "section" of arguments can be any combination that make either a point, or a rectangle,
    // for the destination to copy to. so (x, y), (point), (point, size), (rect), (x, y, width, height).
    //
    // The second section is the source texture to be copied. It serves as the marker where sections begin/end.
    // 
    // The third section is the source coordinates to copy from, which can be either none (entire texture), or rectangle.
    //
    // Finally there is an optional opacity to apply.


    int tex_index = -1;
    for (int i = 0; i < argc; i++)
    {
        if (rb_obj_is_kind_of(argv[i], rb_cTexture))
        {
            tex_index = i;
            break;
        }
    }

    if (tex_index == -1)
        rb_raise(rb_eArgError, "invalid arguments");

    float opacity;
    RGSS_Texture *src = DATA_PTR(argv[tex_index]), *dst = DATA_PTR(self);
    RGSS_Rect dst_rect, src_rect;

    // Parse destination rect out of arguments
    switch (tex_index)
    {
        case 1:
        {
            // (point)
            if (rb_obj_is_kind_of(argv[0], rb_cIVec2))
            {
                memcpy(&dst_rect.location, DATA_PTR(argv[0]), sizeof(RGSS_Point));
                dst_rect.width = src->width;
                dst_rect.height = src->height;
            }
            // (rect)
            else if (rb_obj_is_kind_of(argv[0], rb_cRect))
                memcpy(&dst_rect, DATA_PTR(argv[0]), sizeof(RGSS_Rect));
            else 
                rb_raise(rb_eTypeError, "given %s, expected Point or Rect", CLASS_NAME(argv[0]));
            break;
        }
        case 2:
        {
            // (point, size)
            if (rb_obj_is_kind_of(argv[0], rb_cIVec2))
            {
                memcpy(&dst_rect.location, DATA_PTR(argv[0]), sizeof(RGSS_Point));
                memcpy(&dst_rect.size, DATA_PTR(argv[1]), sizeof(RGSS_Size));
            }
            // (x, y)
            else
            {
                dst_rect.x = NUM2INT(argv[0]);
                dst_rect.y = NUM2INT(argv[1]);
                dst_rect.width = src->width;
                dst_rect.height = src->height;
            }
            break;
        }
        case 4:
        {
            // (x, y, w, h)
            dst_rect.x = NUM2INT(argv[0]);
            dst_rect.y = NUM2INT(argv[1]);
            dst_rect.width = NUM2INT(argv[2]);
            dst_rect.height = NUM2INT(argv[3]);
            break;
        }
        default: rb_raise(rb_eArgError, "invalid arguments");
    }

    int last = argc - tex_index - 1;
    switch (last)
    {
        case 0:
        {
            src_rect = (RGSS_Rect) { 0, 0, src->width, src->height };
            opacity = 1.0f;
            break;
        }
        case 1:
        {
            if (rb_obj_is_kind_of(argv[tex_index + 1], rb_cRect))
            {
                memcpy(&src_rect, DATA_PTR(argv[tex_index + 1]), sizeof(RGSS_Rect));
                opacity = 1.0f;
            }
            else
            {
                src_rect = (RGSS_Rect) { 0, 0, src->width, src->height };
                opacity = glm_clamp(NUM2FLT(argv[tex_index + 1]), 0.0f, 1.0f);
            }
            break;
        }
        case 2:
        {
            memcpy(&src_rect, DATA_PTR(argv[tex_index + 1]), sizeof(RGSS_Rect));
            opacity = glm_clamp(NUM2FLT(argv[tex_index + 1]), 0.0f, 1.0f);
            break;
        }
        default: rb_raise(rb_eArgError, "invalid arguments");
    }

    RGSS_Texture_BlitImpl(dst, src, &src_rect, &dst_rect, opacity);
    return self;
}

static VALUE RGSS_Texture_Target(int argc, VALUE *argv, VALUE self)
{
    VALUE area;
    rb_scan_args(argc, argv, "01", &area);

    rb_need_block();

    RGSS_Texture *tex = DATA_PTR(self);
    RGSS_ASSERT_TEXTURE(tex);

    int x, y, w, h;
    if (NIL_P(area))
    {
        x = 0;
        y = 0;
        w = tex->width;
        h = tex->height;
    }
    else
    {
        RGSS_Rect *rect = DATA_PTR(area);
        x = rect->x;
        y = rect->y;
        w = rect->width;
        h = rect->height;
    }

    RGSS_Texture_BindFramebuffer(tex);

    mat4 mat;
    glm_ortho(x, x + w, y, y + h, -1.0f, 1.0f, mat);
    glBindBuffer(GL_UNIFORM_BUFFER, RGSS_GAME.graphics.ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, RGSS_MAT4_SIZE, mat);
    glViewport(x, y, w, h);
    glScissor(x, y, w, h);

    rb_yield(Qundef);

    glBindBuffer(GL_UNIFORM_BUFFER, RGSS_GAME.graphics.ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, RGSS_MAT4_SIZE, RGSS_GAME.graphics.projection);
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);

    RGSS_Graphics_Restore(rb_mGraphics);

    return self;
}

static inline void RGSS_Texture_Fill(RGSS_Texture *tex, float *color, RGSS_Rect *rect)
{
    RGSS_Texture_BindFramebuffer(tex);
    glScissor(rect->x, rect->y, rect->width, rect->height);
    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT);
    RGSS_Graphics_Restore(rb_mGraphics);
}

static VALUE RGSS_Texture_Clear(VALUE self)
{
    RGSS_Texture *tex = DATA_PTR(self);
    RGSS_Rect rect = {0, 0, tex->width, tex->height};
    float color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    RGSS_Texture_Fill(tex, color, &rect);
    return self;
}

static VALUE RGSS_Texture_FillRect(int argc, VALUE *argv, VALUE self)
{
    VALUE a0, a1, a2, a3, a4;
    rb_scan_args(argc, argv, "23", &a0, &a1, &a2, &a3, &a4);

    RGSS_Texture *tex = DATA_PTR(self);
    float *color;
    RGSS_Rect rect;

    switch (argc)
    {
        case 2: {
            memcpy(&rect, DATA_PTR(a0), sizeof(RGSS_Rect));
            color = DATA_PTR(a1);
            break;
        }
        case 3: {
            memcpy(&rect.location, DATA_PTR(a0), sizeof(RGSS_Point));
            memcpy(&rect.size, DATA_PTR(a1), sizeof(RGSS_Size));
            color = DATA_PTR(a2);
            break;
        }
        case 5: {
            rect.x = NUM2INT(a0);
            rect.y = NUM2INT(a1);
            rect.width = NUM2INT(a2);
            rect.height = NUM2INT(a3);
            color = DATA_PTR(a4);
            break;
        }
        default:
            rb_raise(rb_eArgError, "wrong number of arguments (given %d, expexted 2, 3, or 5)");
    }

    RGSS_Texture_Fill(tex, color, &rect);
    return self;
}

void RGSS_Init_Texture(VALUE parent)
{
    rb_cTexture = rb_define_class_under(parent, "Texture", rb_cObject);
    rb_define_alloc_func(rb_cTexture, RGSS_Texture_Alloc);

    rb_define_methodm1(rb_cTexture, "initialize", RGSS_Texture_Initialize, -1);
    rb_define_method0(rb_cTexture, "dispose", RGSS_Texture_Dispose, 0);
    rb_define_method0(rb_cTexture, "disposed?", RGSS_Texture_IsDisposed, 0);
    rb_define_method0(rb_cTexture, "to_image", RGSS_Texture_ToImage, 0);
    rb_define_method0(rb_cTexture, "width", RGSS_Texture_GetWidth, 0);
    rb_define_method0(rb_cTexture, "height", RGSS_Texture_GetHeight, 0);
    rb_define_method0(rb_cTexture, "id", RGSS_Texture_GetID, 0);
    rb_define_method0(rb_cTexture, "fbo", RGSS_Texture_GetFBO, 0);
    rb_define_method0(rb_cTexture, "size", RGSS_Texture_GetSize, 0);
    rb_define_method0(rb_cTexture, "rect", RGSS_Texture_GetRect, 0);
    rb_define_methodm1(rb_cTexture, "bind", RGSS_Texture_Bind, -1);
    rb_define_methodm1(rb_cTexture, "target", RGSS_Texture_Target, -1);
    rb_define_methodm1(rb_cTexture, "blit", RGSS_Texture_Blit, -1);


    rb_define_method0(rb_cTexture, "clear", RGSS_Texture_Clear, 0);
    rb_define_methodm1(rb_cTexture, "fill_rect", RGSS_Texture_FillRect, -1);

    rb_define_alias(rb_cTexture, "bounds", "rect");
    rb_define_alias(rb_cTexture, "blt", "blit");

    rb_define_singleton_methodm1(rb_cTexture, "load", RGSS_Texture_Load, -1);
    rb_define_singleton_method0(rb_cTexture, "default_options", RGSS_Texture_GetDefaultOptions, 0);
    rb_define_singleton_method1(rb_cTexture, "unbind", RGSS_Texture_Unbind, 1);
    rb_define_singleton_method3(rb_cTexture, "wrap", RGSS_Texture_FromID, 3);

    glm_vec4_zero(RGSS_VEC4_ZERO);
    glm_mat4_identity(RGSS_BLIT_MODEL);
}