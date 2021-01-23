#include "graphics.h"
#include "game.h"

VALUE rb_mGraphics;
VALUE rb_eGLError;
VALUE rb_cShader;

void RGSS_Graphics_GLCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg,
                              const void *data)
{
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    RGSS_LOG_LEVEL level;
    char *kind;

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_LOW: level = RGSS_LOG_WARN; break;
        case GL_DEBUG_SEVERITY_MEDIUM: level = RGSS_LOG_ERROR; break;
        case GL_DEBUG_SEVERITY_HIGH: level = RGSS_LOG_FATAL; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: level = RGSS_LOG_DEBUG; break;
        default: level = RGSS_LOG_UNKNOWN; break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               kind = "error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: kind = "deprecated behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  kind = "undefined behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         kind = "portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         kind = "performance"; break;
        case GL_DEBUG_TYPE_MARKER:              kind = "marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          kind = "push group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           kind = "pop group"; break;
        case GL_DEBUG_TYPE_OTHER:               kind = "other"; break;
    }

    RGSS_Log(level, "%s (%s)", msg, kind);
    if (level == RGSS_LOG_FATAL)
        rb_raise(rb_eGLError, "%s", msg);
}

#define RGSS_VIEWPORT(rect)                                                                                            \
    glViewport(rect.x, rect.y, rect.width, rect.height);                                                               \
    glScissor(rect.x, rect.y, rect.width, rect.height)
#define RGSS_CLEAR_COLOR(c) glClearColor(c[0], c[1], c[2], c[3])

GLuint RGSS_CreateShader(const char *source, GLenum type)
{
    if (source == NULL)
        rb_raise(rb_eArgError, "shader source cannot be nil");

    GLuint id = glCreateShader(type);
    GLint i = strlen(source) + 1;

    glShaderSource(id, 1, &source, &i);
    glCompileShader(id);

    glGetShaderiv(id, GL_COMPILE_STATUS, &i);
    if (i == GL_TRUE)
        return id;

    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &i);
    char buffer[i];
    glGetShaderInfoLog(id, i, NULL, buffer);
    glDeleteShader(id);
    rb_raise(rb_eRuntimeError, "failed to compile shader: %s", buffer);
}

GLuint RGSS_CreateShaderFromFile(const char *filename, GLenum type)
{
    char *source = RGSS_ReadFileText(filename);
    GLuint shader = RGSS_CreateShader(source, type);
    xfree(source);
    return shader;
}

GLuint RGSS_CreateProgram(GLuint vertex, GLuint fragment, GLuint geometry)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    if (geometry != GL_NONE)
        glAttachShader(program, geometry);

    glLinkProgram(program);
    glDetachShader(program, vertex);
    glDetachShader(program, fragment);
    if (geometry != GL_NONE)
        glDetachShader(program, geometry);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        GLint length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char buffer[length];

        glGetProgramInfoLog(program, length, NULL, buffer);
        glDeleteProgram(program);
        rb_raise(rb_eRGSSError, "failed to link shader program: %s", buffer);
    }

    GLint index = glGetUniformBlockIndex(program, "RGSS");
    if (index != GL_INVALID_INDEX)
    {
        glUniformBlockBinding(program, index, 0);
        RGSS_LogDebug("Shader (ID:%u) bound to projection UBO at index %d", program, index);
    }
    RGSS_LogDebug("Shader (ID:%u) compilation and linkage completed successfully", program);
    return program;
}

GLuint RGSS_CreateProgramFromSource(const char *vert_src, const char *frag_src, const char *geom_src)
{
    GLuint vert = RGSS_CreateShader(vert_src, GL_VERTEX_SHADER);
    GLuint frag = RGSS_CreateShader(frag_src, GL_FRAGMENT_SHADER);
    GLuint geom = geom_src ? RGSS_CreateShader(geom_src, GL_GEOMETRY_SHADER) : GL_NONE;
    GLuint program = RGSS_CreateProgram(vert, frag, geom);

    glDeleteShader(vert);
    glDeleteShader(frag);
    glDeleteShader(geom);
    return program;
}

GLuint RGSS_CreateProgramFromFile(const char *vert_path, const char *frag_path, const char *geom_path)
{
    char *vert = RGSS_ReadFileText(vert_path);
    char *frag = RGSS_ReadFileText(frag_path);
    char *geom = geom_path ? RGSS_ReadFileText(geom_path) : NULL;
    GLuint program = RGSS_CreateProgramFromSource(vert, frag, geom);

    xfree(vert);
    xfree(geom);
    if (geom)
        xfree(geom);

    return program;
}

static VALUE RGSS_Shader_Alloc(VALUE klass)
{
    return Data_Wrap_Struct(klass, NULL, RUBY_NEVER_FREE, NULL);
}

static VALUE RGSS_Shader_Load(int argc, VALUE *argv, VALUE klass)
{
    VALUE v, f, g;
    rb_scan_args(argc, argv, "21", &v, &f, &g);

    char *vert_path = StringValueCStr(v);
    char *frag_path = StringValueCStr(f);
    char *geom_path = RTEST(g) ? StringValueCStr(g) : NULL;

    GLuint program = RGSS_CreateProgramFromFile(vert_path, frag_path, geom_path);
    if (program == GL_NONE)
        rb_raise(rb_eRGSSError, "failed to create shader program");

    return RGSS_SHADER_WRAP_CLASS(klass, program);
}

static VALUE RGSS_Shader_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE v, f, g;
    rb_scan_args(argc, argv, "21", &v, &f, &g);

    char *vert = StringValueCStr(v);
    char *frag = StringValueCStr(f);
    char *geom = RTEST(g) ? StringValueCStr(g) : NULL;

    GLuint id = RGSS_CreateProgramFromSource(vert, frag, geom);
    if (id == GL_NONE)
        rb_raise(rb_eRGSSError, "failed to create shader program");

    RDATA(self)->data = RGSS_SHADER_PTR(id);
    return self;
}

static VALUE RGSS_Shader_Use(VALUE self)
{
    GLuint id = RGSS_SHADER_UNWRAP(self);
    RGSS_ASSERT_SHADER(id);
    glUseProgram(id);
    return self;
}

static VALUE RGSS_Shader_GetID(VALUE self)
{
    return UINT2NUM(RGSS_SHADER_UNWRAP(self));
}

static VALUE RGSS_Shader_Locate(VALUE self, VALUE uniform)
{
    if (uniform == Qnil)
        return INT2NUM(-1);
    GLuint id = RGSS_SHADER_UNWRAP(self);
    RGSS_ASSERT_SHADER(id);
    return INT2NUM(glGetUniformLocation(id, StringValueCStr(uniform)));
}

static VALUE RGSS_Shader_Dispose(VALUE self)
{
    GLuint id = RGSS_SHADER_UNWRAP(self);
    if (id != GL_NONE)
    {
        glDeleteProgram(id);
        RDATA(self)->data = NULL;
    }
    return Qnil;
}

static VALUE RGSS_Shader_IsDisposed(VALUE self)
{
    return RB_BOOL(DATA_PTR(self));
}

// TODO: Uniform setters

static inline void RGSS_Graphics_SetProjection(mat4 ortho)
{
    glBindBuffer(GL_UNIFORM_BUFFER, RGSS_GRAPHICS.ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, RGSS_MAT4_SIZE, ortho);
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);
}

static void RGSS_Graphics_Reshape(int width, int height)
{
    if (RGSS_GAME.auto_ortho)
    {
        // Calculate ratios between window and internal resolution
        RGSS_GRAPHICS.ratio[0] = (float)width / RGSS_GRAPHICS.resolution[0];
        RGSS_GRAPHICS.ratio[1] = (float)height / RGSS_GRAPHICS.resolution[1];
        float ratio = RGSS_MIN(RGSS_GRAPHICS.ratio[0], RGSS_GRAPHICS.ratio[1]);

        // Calculate letterbox/pillar rendering coordinates as required
        int x, y, w, h;
        w = (int)roundf(RGSS_GRAPHICS.resolution[0] * ratio);
        h = (int)roundf(RGSS_GRAPHICS.resolution[1] * ratio);
        x = (int)roundf(((float)width - RGSS_GRAPHICS.resolution[0] * ratio) * 0.5f);
        y = (int)roundf(((float)height - RGSS_GRAPHICS.resolution[1] * ratio) * 0.5f);

        RGSS_GRAPHICS.viewport = (RGSS_Rect){x, y, w, h};
        glViewport(x, y, w, h);

        // Ensure the clipping area is also cleared
        glDisable(GL_SCISSOR_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, y, w, h);
        RGSS_CLEAR_COLOR(RGSS_GRAPHICS.color);
    }
    else
    {
        glm_vec2_one(RGSS_GRAPHICS.ratio);
        RGSS_GRAPHICS.viewport = (RGSS_Rect){0, 0, width, height};
        glScissor(0, 0, width, height);
        glViewport(0, 0, width, height);
    }
}

static VALUE RGSS_Graphics_GetFPS(VALUE graphics)
{
    return DBL2NUM(RGSS_GAME.window ? RGSS_GAME.time.fps : 0.0);
}

static VALUE RGSS_Graphics_GetBackColor(VALUE graphics)
{
    RGSS_ASSERT_GAME;
    float *color = RGSS_VEC4_NEW;
    memcpy(color, RGSS_GRAPHICS.color, RGSS_VEC4_SIZE);
    return Data_Wrap_Struct(rb_cColor, NULL, free, color);
}

static VALUE RGSS_Graphics_SetBackColor(VALUE graphics, VALUE color)
{
    RGSS_ASSERT_GAME;
    if (RTEST(color))
    {
        float *vec = DATA_PTR(color);
        memcpy(RGSS_GRAPHICS.color, vec, RGSS_VEC4_SIZE);
    }
    else
    {
        memset(RGSS_GRAPHICS.color, 0, RGSS_VEC4_SIZE);
    }
    RGSS_CLEAR_COLOR(RGSS_GRAPHICS.color);
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

    // Copy current projection into a temp variable
    mat4 temp;
    memcpy(temp, RGSS_GRAPHICS.projection, RGSS_MAT4_SIZE);

    // Change the projection to match the resolution
    RGSS_Rect rect = {0, 0, (int)RGSS_GRAPHICS.resolution[0], (int)RGSS_GRAPHICS.resolution[1]};
    RGSS_VIEWPORT(rect);
    glm_ortho(0.0f, rect.width, rect.height, 0.0, -1.0f, 1.0f, RGSS_GRAPHICS.projection);
    RGSS_Graphics_SetProjection(RGSS_GRAPHICS.projection);

    // Render the scene and copy the framebuffer pixels
    unsigned char *pixels = xmalloc(sizeof(int) * rect.width * rect.height);
    rb_funcall(graphics, RGSS_ID_RENDER, 1, DBL2NUM(0.0));
    glReadPixels(0, 0, rect.width, rect.height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    RGSS_VIEWPORT(RGSS_GRAPHICS.viewport);

    // Restore the projection matrix
    memcpy(RGSS_GRAPHICS.projection, temp, RGSS_MAT4_SIZE);
    RGSS_Graphics_SetProjection(RGSS_GRAPHICS.projection);

    // Return result as an image
    return RGSS_Image_New(rect.width, rect.height, pixels);
}

static VALUE RGSS_Graphics_GetResolution(VALUE graphics)
{
    if (RGSS_GAME.window == NULL)
        return RGSS_Size_New(0, 0);

    return RGSS_Size_New((int)RGSS_GRAPHICS.resolution[0], (int)RGSS_GRAPHICS.resolution[1]);
}

static VALUE RGSS_Graphics_SetResolution(VALUE graphics, VALUE size)
{
    RGSS_ASSERT_GAME;
    RGSS_Size *ivec = DATA_PTR(size);
    RGSS_SizeNotEmpty(ivec->width, ivec->height);

    RGSS_GRAPHICS.resolution[0] = (float)ivec->width;
    RGSS_GRAPHICS.resolution[1] = (float)ivec->height;
    glm_ortho(0.0f, (float)ivec->width, (float)ivec->height, 0.0f, -1.0f, 1.0f, RGSS_GRAPHICS.projection);

    GLvoid *p = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(p, RGSS_GRAPHICS.projection, RGSS_MAT4_SIZE);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
    RGSS_LogInfo("Internal resolution set to %dx%d.", ivec[0], ivec[1]);
    
    int w, h;
    glfwGetFramebufferSize(RGSS_GAME.window, &w, &h);
    RGSS_Graphics_Reshape(w, h);
}

static void RGSS_Graphics_ResizeCallback(GLFWwindow *window, int width, int height)
{
    RGSS_Graphics_Reshape(width, height);
    // TODO: Call block if set
}

VALUE RGSS_Graphics_Restore(VALUE graphics)
{
    if (RGSS_GAME.window == NULL)
        return Qnil;
    // TODO: Restore projection?
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    RGSS_VIEWPORT(RGSS_GRAPHICS.viewport);
    RGSS_CLEAR_COLOR(RGSS_GRAPHICS.color);
    return Qnil;
}

void RGSS_Graphics_Init(GLFWwindow *window, int width, int height, int vsync)
{
    glfwSwapInterval(vsync);
    RGSS_LogInfo(vsync ? "Enabled vertical synchronization" : "Frame limiting disabled, let 'er rip");
    RGSS_GRAPHICS.projection = RGSS_MAT4_NEW;
    RGSS_GRAPHICS.resolution[0] = (float)width;
    RGSS_GRAPHICS.resolution[1] = (float)height;
    RGSS_LogInfo("Internal resolution initialized to %dx%d", width, height);

    glm_vec2_one(RGSS_GRAPHICS.ratio);
    RGSS_GRAPHICS.viewport = (RGSS_Rect){0, 0, width, height};
    glm_ortho(0.0f, RGSS_GRAPHICS.resolution[0], RGSS_GRAPHICS.resolution[1], 0.0f, -1.0f, 1.0f,
              RGSS_GRAPHICS.projection);

    glGenBuffers(1, &RGSS_GRAPHICS.ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, RGSS_GRAPHICS.ubo);
    glBufferData(GL_UNIFORM_BUFFER, RGSS_MAT4_SIZE, RGSS_GRAPHICS.projection, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, RGSS_GRAPHICS.ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, GL_NONE);

    glfwSetFramebufferSizeCallback(window, RGSS_Graphics_ResizeCallback);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);

    if (GLAD_GL_KHR_debug)
    {
        RGSS_LogDebug("Enabled synchronous OpenGL debugging");
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(RGSS_Graphics_GLCallback, NULL);
    }

    vec_init(&RGSS_GRAPHICS.batch.items);

    GLuint id = RGSS_CreateProgramFromSource(SPRITE_VERT_SRC, SPRITE_FRAG_SRC, NULL);
    RGSS_GRAPHICS.shader.id = id;
    RGSS_GRAPHICS.shader.model = glGetUniformLocation(id, "model");
    RGSS_GRAPHICS.shader.color = glGetUniformLocation(id, "color");
    RGSS_GRAPHICS.shader.tone = glGetUniformLocation(id, "tone");
    RGSS_GRAPHICS.shader.flash = glGetUniformLocation(id, "flash");
    RGSS_GRAPHICS.shader.hue = glGetUniformLocation(id, "hue");
    RGSS_GRAPHICS.shader.opacity = glGetUniformLocation(id, "opacity");
    RGSS_LogDebug("Successfully compiled and linked sprite shader");


    // TODO:
    const char *v = "/home/eric/open_rpg/lib/rgss/shaders/particles-vert.glsl";
    const char *f = "/home/eric/open_rpg/lib/rgss/shaders/particles-frag.glsl";
    id = RGSS_CreateProgramFromFile(v, f, NULL);
    RGSS_GRAPHICS.particle_shader.id = id;
    RGSS_GRAPHICS.particle_shader.color = glGetUniformLocation(id, "color");
    RGSS_GRAPHICS.particle_shader.tone = glGetUniformLocation(id, "tone");
    RGSS_GRAPHICS.particle_shader.flash = glGetUniformLocation(id, "flash");
    RGSS_GRAPHICS.particle_shader.hue = glGetUniformLocation(id, "hue");
    RGSS_GRAPHICS.particle_shader.opacity = glGetUniformLocation(id, "opacity");
    RGSS_GRAPHICS.particle_shader.textured = glGetUniformLocation(id, "textured");
    RGSS_LogDebug("Successfully compiled and linked particle shader");
}

void RGSS_Graphics_Deinit(GLFWwindow *window)
{
    glfwSetFramebufferSizeCallback(window, NULL);

    if (RGSS_GRAPHICS.projection)
        free(RGSS_GRAPHICS.projection);
    glDeleteBuffers(1, &RGSS_GRAPHICS.ubo);

    // TODO: Iterate and destroy children
    vec_deinit(&RGSS_GRAPHICS.batch.items);
}

void RGSS_Graphics_Render(double alpha)
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (RGSS_GRAPHICS.batch.invalid)
    {
        vec_sort(&RGSS_GRAPHICS.batch.items, RGSS_Batch_Sort);
        RGSS_GRAPHICS.batch.invalid = false;
    }

    VALUE n = DBL2NUM(alpha);
    VALUE obj;
    int i;
    vec_foreach(&RGSS_GRAPHICS.batch.items, obj, i)
    {
        rb_funcall2(obj, RGSS_ID_RENDER, 1, &n);
    }

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
    return Data_Wrap_Struct(rb_cMat4, NULL, RUBY_NEVER_FREE, RGSS_GRAPHICS.projection);
}

static VALUE RGSS_Graphics_Project(int argc, VALUE *argv, VALUE graphics)
{
    VALUE x, y, w, h;
    rb_scan_args(argc, argv, "13", &x, &y, &w, &h);

    if (argc == 1)
    {
        if (NIL_P(x))
        {
            mat4 identity;
            glm_mat4_identity(identity);
            RGSS_Graphics_SetProjection(identity);
        }
        else
        {
            RGSS_Graphics_SetProjection(DATA_PTR(x));
        }
    }
    else if (argc == 4)
    {
        GLfloat l, t, r, b;
        l = NUM2FLT(x), t = NUM2FLT(y);
        r = l + NUM2FLT(w), b = t + NUM2FLT(h);
        mat4 ortho;
        glm_ortho(l, r, t, b, -1.0f, 1.0f, ortho);
        RGSS_Graphics_SetProjection(ortho);
    }
    else
    {
        rb_raise(rb_eArgError, "wrong number of arguments (given %d, expected 1 or 4)", argc);
    }

    if (rb_block_given_p())
    {
        rb_yield(Qundef);
        RGSS_Graphics_SetProjection(RGSS_GRAPHICS.projection);
    }

    return Qnil;
}

static VALUE RGSS_Graphics_GetUniformBlock(VALUE graphics)
{
    RGSS_ASSERT_GAME;
    return UINT2NUM(RGSS_GRAPHICS.ubo);
}

static VALUE RGSS_Graphics_GetBatch(VALUE graphics)
{
    RGSS_ASSERT_GAME;
    return Data_Wrap_Struct(rb_cBatch, NULL, RUBY_NEVER_FREE, &RGSS_GRAPHICS.batch);
}

void RGSS_Init_Graphics(VALUE parent)
{
    rb_mGraphics = rb_define_module_under(parent, "Graphics");
    rb_eGLError = rb_define_class_under(parent, "GLError", rb_eStandardError);
    rb_define_singleton_method0(rb_mGraphics, "frame_rate", RGSS_Graphics_GetFPS, 0);
    rb_define_singleton_method0(rb_mGraphics, "frame_count", RGSS_Graphics_GetFrameCount, 0);
    rb_define_singleton_method0(rb_mGraphics, "back_color", RGSS_Graphics_GetBackColor, 0);
    rb_define_singleton_method1(rb_mGraphics, "back_color=", RGSS_Graphics_SetBackColor, 1);
    rb_define_singleton_method0(rb_mGraphics, "clear", RGSS_Graphics_Clear, 0);
    rb_define_singleton_method0(rb_mGraphics, "restore", RGSS_Graphics_Restore, 0);
    rb_define_singleton_method0(rb_mGraphics, "resolution", RGSS_Graphics_GetResolution, 0);
    rb_define_singleton_method1(rb_mGraphics, "resolution=", RGSS_Graphics_SetResolution, 1);

    rb_define_singleton_methodm1(rb_mGraphics, "project", RGSS_Graphics_Project, -1);
    rb_define_singleton_method0(rb_mGraphics, "projection", RGSS_Graphics_GetProjection, 0);
    rb_define_singleton_method0(rb_mGraphics, "ubo", RGSS_Graphics_GetUniformBlock, 0);
    rb_define_singleton_method0(rb_mGraphics, "batch", RGSS_Graphics_GetBatch, 0);

    VALUE singleton = rb_singleton_class(rb_mGraphics);
    rb_define_alias(singleton, "fps", "frame_rate");

    rb_cShader = rb_define_class_under(rb_mGraphics, "Shader", rb_cObject);
    rb_define_alloc_func(rb_cShader, RGSS_Shader_Alloc);
    rb_define_methodm1(rb_cShader, "initialize", RGSS_Shader_Initialize, -1);
    rb_define_method0(rb_cShader, "id", RGSS_Shader_GetID, 0);
    rb_define_method0(rb_cShader, "use", RGSS_Shader_Use, 0);
    rb_define_method0(rb_cShader, "disposed", RGSS_Shader_Dispose, 0);
    rb_define_method0(rb_cShader, "disposed?", RGSS_Shader_IsDisposed, 0);
    rb_define_method1(rb_cShader, "locate", RGSS_Shader_Locate, 1);
    rb_include_module(rb_cShader, rb_mGL);

    rb_define_singleton_methodm1(rb_cShader, "load", RGSS_Shader_Load, -1);
}