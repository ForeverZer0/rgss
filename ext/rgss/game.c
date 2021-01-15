
#include "glad.h"
#include "game.h"

VALUE rb_mGame;
VALUE rb_mAudio;

#define RGSS_MAX_TPS 240.0
#define RGSS_MIN_TPS 1.0
#define RGSS_DEFAULT_TPS (1.0 / 30.0)
#define IVAR_TITLE "@title"
#define IVAR_ICON "@icon"
#define IVAR_CLOSING "@close_procs"

RGSS_Game RGSS_GAME;

static void RGSS_Game_ErrorCallback(int code, const char *message)
{
    switch (code)
    {
        case GLFW_NO_ERROR: break;
        case GLFW_NOT_INITIALIZED: rb_raise(rb_eRuntimeError, "Game has not been created");
        case GLFW_NO_WINDOW_CONTEXT:
        case GLFW_NO_CURRENT_CONTEXT: rb_raise(rb_eRuntimeError, "no OpenGL context current on calling thread");
        case GLFW_INVALID_ENUM: rb_raise(rb_eArgError, "an invalid enum value was specified");
        case GLFW_INVALID_VALUE: rb_raise(rb_eArgError, "an invalid argument was specified");
        case GLFW_OUT_OF_MEMORY: rb_raise(rb_eNoMemError, "out of memory");
        case GLFW_API_UNAVAILABLE: 
        case GLFW_VERSION_UNAVAILABLE: rb_raise(rb_eRuntimeError, "platform does not support OpenGL 3.3");
        case GLFW_PLATFORM_ERROR: 
        case GLFW_FORMAT_UNAVAILABLE: rb_raise(rb_eRuntimeError, "%s", message);
        default: rb_raise(rb_eRuntimeError, "%s", message);
    }
}

static inline void RGSS_Game_ParseOpt(VALUE opts, const char *name, int ifnone, int *result)
{
    if (result == NULL)
        return;

    if (opts == Qnil)
        *result = ifnone;
    else
    {
        VALUE opt = rb_hash_aref(opts, STR2SYM(name));
        *result = (opt == Qnil) ? ifnone : RTEST(opt);
    }
}

static void RGSS_Game_CloseCallback(GLFWwindow *window)
{
    VALUE ary = rb_iv_get(rb_mGame, IVAR_CLOSING);
    if (ary == Qnil)
        return;

    VALUE args = rb_ary_new();
    long length = rb_array_len(ary);
    for (long i = 0; i < length; i++)
    {
        rb_proc_call(rb_ary_entry(ary, i), args);
    }
}

static VALUE RGSS_Game_Handle(VALUE game)
{
    return RGSS_GAME.window ? Data_Wrap_Struct(rb_cWindow, NULL, RUBY_NEVER_FREE, RGSS_GAME.window) : Qnil;
}

static VALUE RGSS_Game_GetTitle(VALUE game)
{
    return rb_iv_get(game, IVAR_TITLE);
}

static VALUE RGSS_Game_SetTitle(VALUE game, VALUE title)
{
    RGSS_ASSERT_GAME;
    rb_iv_set(game, IVAR_TITLE, title);
    glfwSetWindowTitle(RGSS_GAME.window, RTEST(title) ? StringValueCStr(title) : "");
    return title;
}

static VALUE RGSS_Game_GetFullscreen(VALUE game)
{
    RGSS_ASSERT_GAME;
    GLFWmonitor *monitor = glfwGetWindowMonitor(RGSS_GAME.window);
    return RB_BOOL(monitor != NULL);
}

static VALUE RGSS_Game_SetFullscreen(VALUE game, VALUE fullscreen)
{
    RGSS_ASSERT_GAME;
    GLFWmonitor *monitor = RTEST(fullscreen) ? glfwGetPrimaryMonitor() : NULL;
    GLFWmonitor *game_monitor = glfwGetWindowMonitor(RGSS_GAME.window);

    if (monitor != game_monitor)
    {
        int rect[4];
        int rate = GLFW_DONT_CARE;

        if (monitor)
        {
            glfwGetWindowPos(RGSS_GAME.window, &RGSS_GAME.rect[0], &RGSS_GAME.rect[1]);
            glfwGetWindowSize(RGSS_GAME.window, &RGSS_GAME.rect[2], &RGSS_GAME.rect[3]);

            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            rect[0] = 0;
            rect[1] = 0;
            rect[2] = mode->width;
            rect[3] = mode->height;
            rate = mode->refreshRate;
        }
        else
        {
            memcpy(rect, RGSS_GAME.rect, sizeof(int) * 4);
        }

        glfwSetWindowMonitor(RGSS_GAME.window, monitor, rect[0], rect[1], rect[2], rect[3], rate);
        
    }
    return fullscreen;
}

static VALUE RGSS_Game_Center(VALUE game)
{
    if (RGSS_GAME.window == NULL)
        return Qnil;

    GLFWmonitor *monitor = glfwGetWindowMonitor(RGSS_GAME.window);
    if (monitor != NULL)
        return Qnil;

    monitor = glfwGetPrimaryMonitor();
    int sx, sy, sw, sh;
    glfwGetMonitorWorkarea(monitor, &sx, &sy, &sw, &sh);

    int ww, wh;
    glfwGetWindowSize(RGSS_GAME.window, &ww, &wh);

    int cx = sx + ((sw - ww) / 2);
    int cy = sy + ((sh - wh) / 2);
    glfwSetWindowPos(RGSS_GAME.window, cx, cy);
    return Qnil;
}

static VALUE RGSS_Game_Create(int argc, VALUE *argv, VALUE game)
{
    VALUE width, height, title, opts;
    rb_scan_args(argc, argv, "21:", &width, &height, &title, &opts);

    if (!glfwInit())
        rb_raise(rb_eRuntimeError, "failed to initialize GLFW");

    int w = NUM2INT(width);
    int h = NUM2INT(height);
    if (w < 1)
        rb_raise(rb_eArgError, "width must be greater than 0");
    if (h < 1)
        rb_raise(rb_eArgError, "height must be greater than 0");

    int resizable, fullscreen, vsync, topmost, decorated, visible, locked, debug;
    RGSS_Game_ParseOpt(opts, "resizable", GLFW_FALSE, &resizable);
    RGSS_Game_ParseOpt(opts, "fullscren", GLFW_FALSE, &fullscreen);
    RGSS_Game_ParseOpt(opts, "vsync", GLFW_TRUE, &vsync);
    RGSS_Game_ParseOpt(opts, "topmost", GLFW_FALSE, &topmost);
    RGSS_Game_ParseOpt(opts, "decorated", GLFW_TRUE, &decorated);
    RGSS_Game_ParseOpt(opts, "visible", GLFW_TRUE, &visible);
    RGSS_Game_ParseOpt(opts, "locked", GLFW_FALSE, &locked);
    RGSS_Game_ParseOpt(opts, "debug", GLFW_FALSE, &RGSS_GAME.debug);

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_DECORATED, decorated);
    glfwWindowHint(GLFW_RESIZABLE, resizable);
    glfwWindowHint(GLFW_FLOATING, topmost);
    glfwWindowHint(GLFW_VISIBLE, visible);
    if (RGSS_GAME.debug)
    {
        rb_gv_set("$DEBUG", Qtrue);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, debug);
    }

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);

    rb_iv_set(game, IVAR_TITLE, title);
    const char *str = RTEST(title) ? StringValueCStr(title) : "";
    RGSS_GAME.window = glfwCreateWindow(w, h, str, NULL, NULL);
    if (RGSS_GAME.window == NULL)
        rb_raise(rb_eRuntimeError, "failed to create OpenGL context");

    glfwMakeContextCurrent(RGSS_GAME.window);
    if (!gladLoadGL())
    {
        GLADloadproc proc = (GLADloadproc) glfwGetProcAddress;
        if (!gladLoadGLLoader(proc))
            rb_raise(rb_eRuntimeError, "failed to link OpenGL");
    }

    if (fullscreen)
        RGSS_Game_SetFullscreen(game, Qtrue);
    if (locked)
        glfwSetWindowAspectRatio(RGSS_GAME.window, w, h);

    RGSS_Graphics_Init(RGSS_GAME.window, w, h, vsync);
    RGSS_Input_Init(RGSS_GAME.window);

    glfwSetWindowCloseCallback(RGSS_GAME.window, RGSS_Game_CloseCallback);
    RGSS_Game_Center(game);
    RGSS_GAME.speed = 1.0;
    return Qnil;
}

static inline void RGSS_Game_UpdateTime(void)
{
    if (RGSS_GAME.time.time_count < 1.0)
        return;

    RGSS_GAME.time.fps = RGSS_GAME.time.fps_count;
    RGSS_GAME.time.fps_count = 0.0;
    RGSS_GAME.time.ticks = RGSS_GAME.time.tick_count;
    RGSS_GAME.time.tick_count = 0.0;
    RGSS_GAME.time.time_count -= 1.0;
}

static inline double RGSS_Game_GetDelta(void)
{
    double now = glfwGetTime();
    double delta = now - RGSS_GAME.time.last;
    RGSS_GAME.time.last = now;
    RGSS_GAME.time.time_count += delta;
    return delta;
}

static VALUE RGSS_Game_Main(int argc, VALUE *argv, VALUE game)
{
    RGSS_ASSERT_GAME;
    VALUE tps;
    rb_scan_args(argc, argv, "01", &tps);

    if (RTEST(tps))
        RGSS_GAME.time.target_tps = 1.0 / NUM2DBL(tps);
    if (RGSS_GAME.time.target_tps < 1.0)
        RGSS_GAME.time.target_tps = RGSS_DEFAULT_TPS;

    ID update = rb_intern("update");
    double delta;
    double accumulator = 0.0;

    while (!glfwWindowShouldClose(RGSS_GAME.window))
    {
        delta = RGSS_Game_GetDelta();
        accumulator = (accumulator + delta) * RGSS_GAME.speed;

        while (accumulator >= RGSS_GAME.time.target_tps)
        {
            accumulator -= RGSS_GAME.time.target_tps;

            // rb_funcall(game, update, 1, DBL2NUM(delta));
            rb_funcall(game, update, 1, DBL2NUM(RGSS_GAME.time.target_tps));
            RGSS_Input_Update();
            RGSS_GAME.time.tick_count++;
            RGSS_GAME.time.total_ticks++;
        }

        RGSS_Graphics_Render(accumulator / RGSS_GAME.time.target_tps);
        glfwPollEvents();
        RGSS_Game_UpdateTime();
    }

    return Qnil;
}

static VALUE RGSS_Game_GetSpeed(VALUE game)
{
    return DBL2NUM(RGSS_GAME.window ? 0.0 : RGSS_GAME.speed);
}

static VALUE RGSS_Game_SetSpeed(VALUE game, VALUE speed)
{
    RGSS_ASSERT_GAME;
    RGSS_GAME.speed = RGSS_MAX(1.0, NUM2DBL(speed));
    return speed;
}

static VALUE RGSS_Game_Close(int argc, VALUE *argv, VALUE game)
{
    RGSS_ASSERT_GAME;

    VALUE close;
    rb_scan_args(argc, argv, "01", &close);

    glfwSetWindowShouldClose(RGSS_GAME.window, argc > 0 ? RTEST(close) : GLFW_TRUE);
    return Qnil;
}

static VALUE RGSS_Game_Terminate(VALUE game)
{
    // TODO: Cleanup 
    glfwTerminate();
    return Qnil;
}

static VALUE RGSS_Game_GetIcon(VALUE game)
{
    return rb_iv_get(game, IVAR_ICON);
}

static VALUE RGSS_Game_SetIcon(VALUE game, VALUE image)
{
    RGSS_ASSERT_GAME;

    if (RTEST(image))
    {
        GLFWimage *img;
        VALUE ivar;

        if (RB_TYPE_P(image, T_STRING))
        {
            img = xmalloc(sizeof(GLFWimage));
            RGSS_Image_Load(StringValueCStr(image), &img->width, &img->height, &img->pixels);
            ivar = Data_Wrap_Struct(rb_cImage, NULL, RGSS_Image_Free, img);
        }
        else if (RB_TYPE_P(image, T_DATA))
        {
            img = DATA_PTR(image);
            ivar = image;
        }
        else
        {
            rb_raise(rb_eTypeError, "%s is not an RGSS::Image, String, or NilClass", CLASS_NAME(image));
        }

        rb_iv_set(game, IVAR_ICON, ivar);
        glfwSetWindowIcon(RGSS_GAME.window, 1, img);
    }
    else
    {
        rb_iv_set(game, IVAR_ICON, Qnil);
        glfwSetWindowIcon(RGSS_GAME.window, 0, NULL);
    }
    return image;
}

static VALUE RGSS_Game_GetTicks(VALUE game)
{
    return ULL2NUM(RGSS_GAME.window ? RGSS_GAME.time.total_ticks : 0);
}

static VALUE RGSS_Game_Focus(VALUE game)
{
    if (RGSS_GAME.window)
        glfwFocusWindow(RGSS_GAME.window);
    return Qnil;
}

static VALUE RGSS_Game_RequestAttention(VALUE game)
{
    if (RGSS_GAME.window)
        glfwRequestWindowAttention(RGSS_GAME.window);
    return Qnil;
}

static VALUE RGSS_Game_Minimize(VALUE game)
{
    if (RGSS_GAME.window)
        glfwIconifyWindow(RGSS_GAME.window);
    return Qnil;
}

static VALUE RGSS_Game_Maximize(VALUE game)
{
    if (RGSS_GAME.window)
        glfwMaximizeWindow(RGSS_GAME.window);
    return Qnil;
}

static VALUE RGSS_Game_IsMinimized(VALUE game)
{
    return RGSS_GAME.window ? RB_BOOL(glfwGetWindowAttrib(RGSS_GAME.window, GLFW_ICONIFIED)) : Qnil;
}

static VALUE RGSS_Game_IsMaximized(VALUE game)
{
    return RGSS_GAME.window ? RB_BOOL(glfwGetWindowAttrib(RGSS_GAME.window, GLFW_MAXIMIZED)) : Qnil;
}

static VALUE RGSS_Game_Restore(VALUE game)
{
    if (RGSS_GAME.window)
        glfwRestoreWindow(RGSS_GAME.window);
    return Qnil;
}

static VALUE RGSS_Game_IsFocused(VALUE game)
{
    return RGSS_GAME.window ? RB_BOOL(glfwGetWindowAttrib(RGSS_GAME.window, GLFW_FOCUSED)) : Qnil;
}

static VALUE RGSS_Game_IsTopmost(VALUE game)
{
    return RGSS_GAME.window ? RB_BOOL(glfwGetWindowAttrib(RGSS_GAME.window, GLFW_FLOATING)) : Qnil;
}

static VALUE RGSS_Game_IsClosing(VALUE game)
{
    return RGSS_GAME.window ? RB_BOOL(glfwWindowShouldClose(RGSS_GAME.window)) : Qnil;
}

static VALUE RGSS_Game_OnClose(VALUE game)
{
    rb_need_block();

    VALUE ary = rb_iv_get(game, IVAR_CLOSING);
    if (ary == Qnil)
    {
        ary = rb_ary_new();
        rb_iv_set(game, IVAR_CLOSING, ary);
    }

    rb_ary_push(ary, rb_block_proc());
    return Qnil;
}


void RGSS_Init_Game(VALUE parent)
{
    rb_mGame =  rb_define_module_under(parent, "Game");
    rb_mAudio = rb_define_module_under(parent, "Audio");

    rb_define_singleton_methodm1(rb_mGame, "create", RGSS_Game_Create, -1);
    rb_define_singleton_method0(rb_mGame, "terminate", RGSS_Game_Terminate, 0);
    rb_define_singleton_methodm1(rb_mGame, "main", RGSS_Game_Main, -1);
    rb_define_singleton_method0(rb_mGame, "window", RGSS_Game_Handle, 0);
    rb_define_singleton_methodm1(rb_mGame, "close", RGSS_Game_Close, -1);
    rb_define_singleton_method0(rb_mGame, "center", RGSS_Game_Center, 0);

    rb_define_singleton_method0(rb_mGame, "fullscreen", RGSS_Game_GetFullscreen, 0);
    rb_define_singleton_method1(rb_mGame, "fullscreen=", RGSS_Game_SetFullscreen, 1);
    rb_define_singleton_method0(rb_mGame, "title", RGSS_Game_GetTitle, 0);
    rb_define_singleton_method1(rb_mGame, "title=", RGSS_Game_SetTitle, 1);
    rb_define_singleton_method0(rb_mGame, "icon", RGSS_Game_GetIcon, 0);
    rb_define_singleton_method1(rb_mGame, "icon=", RGSS_Game_SetIcon, 1);
    rb_define_singleton_method0(rb_mGame, "request_attention", RGSS_Game_RequestAttention, 0);
    rb_define_singleton_method0(rb_mGame, "focus", RGSS_Game_Focus, 0);
    rb_define_singleton_method0(rb_mGame, "minimize", RGSS_Game_Minimize, 0);
    rb_define_singleton_method0(rb_mGame, "maximize", RGSS_Game_Maximize, 0);
    rb_define_singleton_method0(rb_mGame, "restore", RGSS_Game_Restore, 0);
    rb_define_singleton_method0(rb_mGame, "minimized?", RGSS_Game_IsMinimized, 0);
    rb_define_singleton_method0(rb_mGame, "maximized?", RGSS_Game_IsMaximized, 0);
    rb_define_singleton_method0(rb_mGame, "topmost?", RGSS_Game_IsTopmost, 0);
    rb_define_singleton_method0(rb_mGame, "focused?", RGSS_Game_IsFocused, 0);
    rb_define_singleton_method0(rb_mGame, "closing?", RGSS_Game_IsClosing, 0);

    rb_define_singleton_method0(rb_mGame, "ticks", RGSS_Game_GetTicks, 0);
    rb_define_singleton_method0(rb_mGame, "speed", RGSS_Game_GetSpeed, 0);
    rb_define_singleton_method1(rb_mGame, "speed=", RGSS_Game_SetSpeed, 1);

    rb_define_singleton_method0(rb_mGame, "on_close", RGSS_Game_OnClose, 0);

    glfwSetErrorCallback(RGSS_Game_ErrorCallback);
}