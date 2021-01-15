#include "game.h"
#include "glad.h"

VALUE rb_mGraphics;
ID render_id;

#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242

#define GL_DEBUG_SOURCE_API             0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM   0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY     0x8249
#define GL_DEBUG_SOURCE_APPLICATION     0x824A
#define GL_DEBUG_SOURCE_OTHER           0x824B

#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  0x824E
#define GL_DEBUG_TYPE_PORTABILITY         0x824F
#define GL_DEBUG_TYPE_PERFORMANCE         0x8250
#define GL_DEBUG_TYPE_MARKER              0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP          0x8269
#define GL_DEBUG_TYPE_POP_GROUP           0x826A
#define GL_DEBUG_TYPE_OTHER               0x8251

#define GL_DEBUG_SEVERITY_LOW          0x9148
#define GL_DEBUG_SEVERITY_MEDIUM       0x9147
#define GL_DEBUG_SEVERITY_HIGH         0x9146
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B

typedef void (*GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg,
                            const void *data);
typedef void (APIENTRYP PFGLDEBUGMESSAGECALLBACK)(GLDEBUGPROC callback, const void *userParam);


void RGSS_Graphics_GLCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg,
                             const void *data)
{
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) 
        return;

    printf("%s\n", msg);
}

#define RGSS_VIEWPORT(rect) \
glViewport(rect.x, rect.y, rect.width, rect.height);\
glScissor(rect.x, rect.y, rect.width, rect.height)

static void RGSS_Graphics_Reshape(int width, int height)
{
    // Calculate ratios between window and internal resolution
    RGSS_GAME.graphics.ratio[0] = (float)width / RGSS_GAME.graphics.resolution[0];
    RGSS_GAME.graphics.ratio[1] = (float)height / RGSS_GAME.graphics.resolution[1];
    float ratio = RGSS_MIN(RGSS_GAME.graphics.ratio[0], RGSS_GAME.graphics.ratio[1]);

    // Calculate letterbox/pillar rendering coordinates as required
    int x, y, w, h;
    w = (int)roundf(RGSS_GAME.graphics.resolution[0] * ratio);
    h = (int)roundf(RGSS_GAME.graphics.resolution[1] * ratio);
    x = (int)roundf(((float)width - RGSS_GAME.graphics.resolution[0] * ratio) * 0.5f);
    y = (int)roundf(((float)height - RGSS_GAME.graphics.resolution[1] * ratio) * 0.5f);

    RGSS_GAME.graphics.viewport = (RGSS_Rect){x, y, w, h};
    glViewport(x, y, w, h);

    // Ensure the clipping area is also cleared
    glDisable(GL_SCISSOR_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST);
    glScissor(x, y, w, h);
    glClearColor(RGSS_GAME.graphics.color[0], RGSS_GAME.graphics.color[1], RGSS_GAME.graphics.color[2],
                 RGSS_GAME.graphics.color[3]);
}

static VALUE RGSS_Graphics_GetFPS(VALUE graphics)
{
    return DBL2NUM(RGSS_GAME.window ? RGSS_GAME.time.fps : 0.0);
}

static VALUE RGSS_Graphics_GetBackColor(VALUE graphics)
{
    RGSS_ASSERT_GAME;
    float *color = RGSS_VEC4_NEW;
    memcpy(color, RGSS_GAME.graphics.color, RGSS_VEC4_SIZE);
    return Data_Wrap_Struct(rb_cColor, NULL, free, color);
}

static VALUE RGSS_Graphics_SetBackColor(VALUE graphics, VALUE color)
{
    RGSS_ASSERT_GAME;
    if (RTEST(color))
    {
        float *vec = DATA_PTR(color);
        memcpy(RGSS_GAME.graphics.color, vec, RGSS_VEC4_SIZE);
    }
    else
    {
        memset(RGSS_GAME.graphics.color, 0, RGSS_VEC4_SIZE);
    }
    glClearColor(RGSS_GAME.graphics.color[0], RGSS_GAME.graphics.color[1], RGSS_GAME.graphics.color[2],
                 RGSS_GAME.graphics.color[3]);
    return color;
}

static VALUE RGSS_Graphics_Clear(VALUE graphics)
{
    RGSS_ASSERT_GAME;
    glClear(GL_COLOR_BUFFER_BIT);
    return Qnil;
}

static VALUE RGSS_Graphics_Capture(VALUE graphics)
{
    RGSS_ASSERT_GAME;

    // glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    // TODO: Set projection/viewport to graphics size, render, read framebuffer, return image

    GLFWimage *image = xmalloc(sizeof(GLFWimage));
    image->width = (int)RGSS_GAME.graphics.resolution[0];
    image->height = (int)RGSS_GAME.graphics.resolution[1];
    image->pixels = xmalloc(sizeof(int) * image->width * image->height);

    glViewport(0, 0, image->width, image->height);
    rb_funcall(graphics, render_id, 1, DBL2NUM(0.0));

    glReadPixels(0, 0, image->width, image->height, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
    RGSS_VIEWPORT(RGSS_GAME.graphics.viewport);

    return Data_Wrap_Struct(rb_cImage, NULL, RGSS_Image_Free, image);
}

static VALUE RGSS_Graphics_GetResolution(VALUE graphics)
{
    if (RGSS_GAME.window == NULL)
        return RGSS_Size_New(0, 0);

    return RGSS_Size_New((int)RGSS_GAME.graphics.resolution[0], (int)RGSS_GAME.graphics.resolution[1]);
}

static VALUE RGSS_Graphics_SetResolution(VALUE graphics, VALUE size)
{
    RGSS_ASSERT_GAME;
    RGSS_Size *ivec = DATA_PTR(size);

    if (ivec->width < 0)
        rb_raise(rb_eArgError, "width must be greater than 0 (given %d)", ivec->width);
    if (ivec->height < 0)
        rb_raise(rb_eArgError, "height must be greater than 0 (given %d)", ivec->height);

    RGSS_GAME.graphics.resolution[0] = (float)ivec->width;
    RGSS_GAME.graphics.resolution[1] = (float)ivec->height;
    glm_ortho(0.0f, (float)ivec->width, (float)ivec->height, 0.0f, -1.0f, 1.0f, RGSS_GAME.graphics.projection);

    GLvoid *p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(p, RGSS_GAME.graphics.projection, RGSS_MAT4_SIZE);
    glUnmapBuffer(GL_UNIFORM_BUFFER);

    int w, h;
    glfwGetFramebufferSize(RGSS_GAME.window, &w, &h);
    RGSS_Graphics_Reshape(w, h);
}

static void RGSS_Graphics_ResizeCallback(GLFWwindow *window, int width, int height)
{
    RGSS_Graphics_Reshape(width, height);
    // TODO: Call block if set
}

static VALUE RGSS_Graphics_Restore(VALUE graphics)
{
    if (RGSS_GAME.window == NULL)
        return Qnil;

    RGSS_VIEWPORT(RGSS_GAME.graphics.viewport);
    glClearColor(RGSS_GAME.graphics.color[0], RGSS_GAME.graphics.color[1], RGSS_GAME.graphics.color[2],
                 RGSS_GAME.graphics.color[3]);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    return Qnil;
}

void RGSS_Graphics_Init(GLFWwindow *window, int width, int height, int vsync)
{
    glfwSwapInterval(vsync);
    RGSS_GAME.graphics.projection = RGSS_MAT4_NEW;
    RGSS_GAME.graphics.resolution[0] = (float)width;
    RGSS_GAME.graphics.resolution[1] = (float)height;

    glm_vec2_one(RGSS_GAME.graphics.ratio);
    RGSS_GAME.graphics.viewport = (RGSS_Rect){0, 0, width, height};
    glm_ortho(0.0f, RGSS_GAME.graphics.resolution[0], RGSS_GAME.graphics.resolution[1], 0.0f, -1.0f, 1.0f,
              RGSS_GAME.graphics.projection);

    glGenBuffers(1, &RGSS_GAME.graphics.ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, RGSS_GAME.graphics.ubo);
    glBufferData(GL_UNIFORM_BUFFER, RGSS_MAT4_SIZE, RGSS_GAME.graphics.projection, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, RGSS_GAME.graphics.ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);

    glfwSetFramebufferSizeCallback(window, RGSS_Graphics_ResizeCallback);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);

    if (RGSS_GAME.debug)
    {
        void *address;
        address = glfwGetProcAddress("glDebugMessageCallback");
        if (address)
        {
            PFGLDEBUGMESSAGECALLBACK proc = (PFGLDEBUGMESSAGECALLBACK) address;
            proc(RGSS_Graphics_GLCallback, NULL);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }
    }
}

void RGSS_Graphics_Deinit(GLFWwindow *window)
{
    glfwSetFramebufferSizeCallback(window, NULL);

    if (RGSS_GAME.graphics.projection)
        free(RGSS_GAME.graphics.projection);
    glDeleteBuffers(1, &RGSS_GAME.graphics.ubo);
}

void RGSS_Graphics_Render(double alpha)
{
    glClear(GL_COLOR_BUFFER_BIT);
    rb_funcall(rb_mGraphics, render_id, 1, DBL2NUM(alpha));

    RGSS_GAME.time.fps_count++;
    RGSS_GAME.time.total_frames++;
    glfwSwapBuffers(RGSS_GAME.window);
}

static VALUE RGSS_Graphics_GetFrameCount(VALUE graphics)
{
    return ULL2NUM(RGSS_GAME.window ? RGSS_GAME.time.total_frames : 0);
}

static VALUE RGSS_Graphics_GetProjection(VALUE graphics)
{
    RGSS_ASSERT_GAME;
    return Data_Wrap_Struct(rb_cMat4, NULL, RUBY_NEVER_FREE, RGSS_GAME.graphics.projection);
}

// static VALUE RGSS_Graphics_SetProjection(VALUE graphics, VALUE projection)
// {
//     RGSS_ASSERT_GAME;
//     if (RTEST(projection))
//     {
//         vec4 *mat4 = DATA_PTR(projection);
//         glm_mat4_copy(mat4, RGSS_GAME.graphics.projection);
//     }
//     else
//     {
//         glm_mat4_identity(RGSS_GAME.graphics.projection);
//     }
//     return projection;
// }

static VALUE RGSS_Graphics_Project(int argc, VALUE *argv, VALUE graphics)
{
    rb_need_block();

    VALUE x, y, w, h;
    rb_scan_args(argc, argv, "13", &x, &y, &w, &h);
    glBindBuffer(GL_UNIFORM_BUFFER, RGSS_GAME.graphics.ubo);

    if (argc == 1)
    {
        glBufferSubData(GL_UNIFORM_BUFFER, 0, RGSS_MAT4_SIZE, DATA_PTR(x));
    }
    else if (argc == 4)
    {
        mat4 mat;
        glm_ortho(NUM2FLT(x), NUM2FLT(x + w), NUM2FLT(y), NUM2FLT(y + h), -1.0f, 1.0f, mat);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, RGSS_MAT4_SIZE, mat);
    }
    else
    {
        rb_raise(rb_eArgError, "wrong number of arguments (given %d, expected 1 or 4)", argc);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);

    rb_yield(Qundef);

    glBindBuffer(GL_UNIFORM_BUFFER, RGSS_GAME.graphics.ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, RGSS_MAT4_SIZE, RGSS_GAME.graphics.projection);
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
    return Qnil;
}

static VALUE RGSS_Graphics_GetUniformBlock(VALUE graphics)
{
    RGSS_ASSERT_GAME;
    return UINT2NUM(RGSS_GAME.graphics.ubo);
}

void RGSS_Init_Graphics(VALUE parent)
{
    rb_mGraphics = rb_define_module_under(parent, "Graphics");
    rb_define_singleton_method0(rb_mGraphics, "frame_rate", RGSS_Graphics_GetFPS, 0);
    rb_define_singleton_method0(rb_mGraphics, "frame_count", RGSS_Graphics_GetFrameCount, 0);
    rb_define_singleton_method0(rb_mGraphics, "back_color", RGSS_Graphics_GetBackColor, 0);
    rb_define_singleton_method1(rb_mGraphics, "back_color=", RGSS_Graphics_SetBackColor, 1);
    rb_define_singleton_method0(rb_mGraphics, "clear", RGSS_Graphics_Clear, 0);
    rb_define_singleton_method0(rb_mGraphics, "restore", RGSS_Graphics_Restore, 0);
    rb_define_singleton_method0(rb_mGraphics, "resolution", RGSS_Graphics_GetResolution, 0);
    rb_define_singleton_method1(rb_mGraphics, "resolution=", RGSS_Graphics_SetResolution, 1);

    rb_define_singleton_method0(rb_mGraphics, "projection", RGSS_Graphics_GetProjection, 0);
    rb_define_singleton_method0(rb_mGraphics, "ubo", RGSS_Graphics_GetUniformBlock, 0);
    rb_define_singleton_methodm1(rb_mGraphics, "project", RGSS_Graphics_Project, -1);
    // rb_define_singleton_method1(rb_mGraphics, "projection=", RGSS_Graphics_SetProjection, 1);

    VALUE singleton = rb_singleton_class(rb_mGraphics);
    rb_define_alias(singleton, "fps", "frame_rate");

    render_id = rb_intern("render");
}