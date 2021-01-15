#include "rgss.h"
#include "uthash.h"
#include <GLFW/glfw3.h>

VALUE rb_mGLFW;
VALUE rb_cWindow;
VALUE rb_cMonitor;
VALUE rb_cCursor;

VALUE rb_cVideoMode;
VALUE rb_cGammaRamp;
VALUE rb_cGamepadState;

typedef struct
{
    GLFWwindow *pointer;
    VALUE value;
    UT_hash_handle hh;
} RGSS_HANDLE;

RGSS_HANDLE *handles;

static VALUE rb_glfwCreateCursor(VALUE glfw, VALUE image, VALUE x, VALUE y)
{
    GLFWimage *img = RTEST(image) ? DATA_PTR(image) : NULL;
    GLFWcursor *cursor = glfwCreateCursor(img, NUM2INT(x), NUM2INT(y));
    return cursor ? Data_Wrap_Struct(rb_cCursor, NULL, RUBY_NEVER_FREE, cursor) : Qnil;
}

static VALUE rb_glfwCreateStandardCursor(VALUE glfw, VALUE shape)
{
    GLFWcursor *cursor = glfwCreateStandardCursor(NUM2INT(shape));
    return cursor ? Data_Wrap_Struct(rb_cCursor, NULL, RUBY_NEVER_FREE, cursor) : Qnil;
}

static VALUE rb_glfwCreateWindow(int argc, VALUE *argv, VALUE glfw)
{
    VALUE width, height, title, monitor, share;
    rb_scan_args(argc, argv, "23", &width, &height, &title, &monitor, &share);

    const char *str = RTEST(title) ? StringValueCStr(title) : "";
    GLFWmonitor *m = RTEST(monitor) ? DATA_PTR(monitor) : NULL;
    GLFWwindow *s = RTEST(share) ? DATA_PTR(share) : NULL;

    GLFWwindow *window = glfwCreateWindow(NUM2INT(width), NUM2INT(height), str, m, s);
    if (window == NULL)
        return Qnil;

    RGSS_HANDLE *handle = xmalloc(sizeof(RGSS_HANDLE));
    handle->pointer = window;
    handle->value = Data_Wrap_Struct(rb_cWindow, NULL, RUBY_NEVER_FREE, window);

    HASH_ADD(hh, handles, pointer, sizeof(GLFWwindow *), handle);
    return handle->value;
}

static VALUE rb_glfwDefaultWindowHints(VALUE glfw)
{
    glfwDefaultWindowHints();
    return Qnil;
}

static VALUE rb_glfwDestroyCursor(VALUE glfw, VALUE cursor)
{
    if (RTEST(cursor))
        glfwDestroyCursor(DATA_PTR(cursor));
    return Qnil;
}

static VALUE rb_glfwDestroyWindow(VALUE glfw, VALUE window)
{
    if (RTEST(window))
        glfwDestroyWindow(DATA_PTR(window));
    return Qnil;
}

static VALUE rb_glfwExtensionSupported(VALUE glfw, VALUE extension)
{
    if (RTEST(extension))
        return RB_BOOL(glfwExtensionSupported(StringValueCStr(extension)));
    return Qfalse;
}

static VALUE rb_glfwFocusWindow(VALUE glfw, VALUE window)
{
    glfwFocusWindow(DATA_PTR(window));
    return Qnil;
}

static VALUE rb_glfwGetClipboardString(VALUE glfw)
{
    const char *text = glfwGetClipboardString(NULL);
    return text ? rb_str_new_cstr(text) : Qnil;
}

static VALUE rb_glfwGetCurrentContext(VALUE glfw)
{
    GLFWwindow *window = glfwGetCurrentContext();
    if (window == NULL)
        return Qnil;

    RGSS_HANDLE *handle;
    HASH_FIND(hh, handles, &window, sizeof(GLFWwindow *), handle);

    if (handle)
        return handle->value;

    handle = xmalloc(sizeof(RGSS_HANDLE));
    handle->pointer = window;
    handle->value = Data_Wrap_Struct(rb_cWindow, NULL, RUBY_NEVER_FREE, window);

    HASH_ADD(hh, handles, pointer, sizeof(GLFWwindow *), handle);
    return handle->value;
}

static VALUE rb_glfwGetCursorPos(VALUE glfw, VALUE window)
{
    double x, y;
    glfwGetCursorPos(DATA_PTR(window), &x, &y);
    return RGSS_Point_New((int)round(x), (int)round(y));
}

static VALUE rb_glfwGetError(VALUE glfw)
{
    const char *message;
    int code = glfwGetError(&message);

    VALUE str = message ? rb_str_new_cstr(message) : Qnil;
    return rb_ary_new_from_args(2, INT2NUM(code), str);
}

static VALUE rb_glfwGetFramebufferSize(VALUE glfw, VALUE window)
{
    int x, y;
    glfwGetFramebufferSize(DATA_PTR(window), &x, &y);
    return RGSS_Size_New(x, y);
}

static VALUE rb_glfwGetGamepadName(VALUE glfw, VALUE jid)
{
    const char *name = glfwGetGamepadName(NUM2INT(jid));
    return name ? rb_str_new_cstr(name) : Qnil;
}

static VALUE rb_glfwGetGamepadState(VALUE glfw, VALUE jid, VALUE state)
{
    return RB_BOOL(glfwGetGamepadState(NUM2INT(jid), DATA_PTR(state)));
}

static VALUE rb_glfwGetGammaRamp(VALUE glfw, VALUE monitor)
{
    const GLFWgammaramp *ramp = glfwGetGammaRamp(DATA_PTR(monitor));
    return ramp ? Data_Wrap_Struct(rb_cGammaRamp, NULL, RUBY_NEVER_FREE, (GLFWgammaramp*) ramp) : Qnil;
}

static VALUE rb_glfwGetInputMode(VALUE glfw, VALUE window, VALUE mode)
{
    return INT2NUM(glfwGetInputMode(DATA_PTR(window), NUM2INT(mode)));
}

static VALUE rb_glfwGetJoystickAxes(VALUE glfw, VALUE jid)
{
    int count;
    const float *axes = glfwGetJoystickAxes(NUM2INT(jid), &count);
    
    VALUE ary = rb_ary_new_capa(count);
    for (int i = 0; i < count; i++)
        rb_ary_store(ary, i, DBL2NUM(axes[i]));
    return ary;
}

static VALUE rb_glfwGetJoystickButtons(VALUE glfw, VALUE jid)
{
    int count;
    const unsigned char *buttons = glfwGetJoystickButtons(NUM2INT(jid), &count);
    
    VALUE ary = rb_ary_new_capa(count);
    for (int i = 0; i < count; i++)
        rb_ary_store(ary, i, UINT2NUM(buttons[i]));
    return ary;
}

static VALUE rb_glfwGetJoystickGUID(VALUE glfw, VALUE jid)
{
    const char *guid = glfwGetJoystickGUID(NUM2INT(jid));
    return guid ? rb_str_new_cstr(guid) : Qnil;
}

static VALUE rb_glfwGetJoystickHats(VALUE glfw, VALUE jid)
{
    int count;
    const unsigned char *hats = glfwGetJoystickHats(NUM2INT(jid), &count);

    VALUE ary = rb_ary_new_capa(count);
    for (int i = 0; i < count; i++)
        rb_ary_store(ary, i, UINT2NUM(hats[i]));
    return ary;
}

static VALUE rb_glfwGetJoystickName(VALUE glfw, VALUE jid)
{
    const char *name = glfwGetJoystickName(NUM2INT(jid));
    return name ? rb_str_new_cstr(name) : Qnil;
}

static VALUE rb_glfwGetKey(VALUE glfw, VALUE window, VALUE key)
{
    return INT2NUM(glfwGetKey(DATA_PTR(window), NUM2INT(key)));
}

static VALUE rb_glfwGetKeyName(VALUE glfw, VALUE key, VALUE scancode)
{
    const char *name = glfwGetKeyName(NUM2INT(key), NUM2INT(scancode));
    return name ? rb_str_new_cstr(name) : Qnil;
}

static VALUE rb_glfwGetKeyScancode(VALUE glfw, VALUE key)
{
    return INT2NUM(glfwGetKeyScancode(NUM2INT(key)));
}

static VALUE rb_glfwGetMonitorContentScale(VALUE glfw, VALUE monitor)
{
    float x, y;
    glfwGetMonitorContentScale(DATA_PTR(monitor), &x, &y);
    return rb_ary_new_from_args(2, DBL2NUM(x), DBL2NUM(y));
}

static VALUE rb_glfwGetMonitorName(VALUE glfw, VALUE monitor)
{
    const char *name = glfwGetMonitorName(DATA_PTR(monitor));
    return name ? rb_str_new_cstr(name) : Qnil;
}

static VALUE rb_glfwGetMonitorPhysicalSize(VALUE glfw, VALUE monitor)
{
    int x, y;
    glfwGetMonitorPhysicalSize(DATA_PTR(monitor), &x, &y);
    return RGSS_Size_New(x, y);
}

static VALUE rb_glfwGetMonitorPos(VALUE glfw, VALUE monitor)
{
    int x, y;
    glfwGetMonitorPos(DATA_PTR(monitor), &x, &y);
    return RGSS_Point_New(x, y);
}

static VALUE rb_glfwGetMonitorWorkarea(VALUE glfw, VALUE monitor)
{
    int x, y, w, h;
    glfwGetMonitorWorkarea(DATA_PTR(monitor), &x, &y, &w, &h);
    return RGSS_Rect_New(x, y, w, h);
}

static VALUE rb_glfwGetMonitors(VALUE glfw)
{
    int count;
    GLFWmonitor **monitors = glfwGetMonitors(&count);

    VALUE ary = rb_ary_new_capa(count);
    for (int i = 0; i < count; i++)
        rb_ary_store(ary, i, Data_Wrap_Struct(rb_cMonitor, NULL, RUBY_DEFAULT_FREE, monitors[i]));

    return ary;
}

static VALUE rb_glfwGetMouseButton(VALUE glfw, VALUE window, VALUE button)
{
    return INT2NUM(glfwGetMouseButton(DATA_PTR(window), NUM2INT(button)));
}

static VALUE rb_glfwGetPrimaryMonitor(VALUE glfw)
{
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    return monitor ? Data_Wrap_Struct(rb_cMonitor, NULL, RUBY_NEVER_FREE, monitor) : Qnil;
}

static VALUE rb_glfwGetProcAddress(VALUE glfw, VALUE function)
{
    void *proc = glfwGetProcAddress(StringValueCStr(function));
    return proc ? PTR2NUM(proc) : Qnil;
}

static VALUE rb_glfwGetRequiredInstanceExtensions(VALUE glfw)
{
    uint32_t count;
    const char **extensions = glfwGetRequiredInstanceExtensions(&count);

    VALUE ary = rb_ary_new_capa(count);
    for (uint32_t i = 0; i < count; i++)
        rb_ary_store(ary, i, rb_str_new_cstr(extensions[i]));

    return ary;
}

static VALUE rb_glfwGetTime(VALUE glfw)
{
    return DBL2NUM(glfwGetTime());
}

static VALUE rb_glfwGetTimerFrequency(VALUE glfw)
{
    return ULL2NUM(glfwGetTimerFrequency());
}

static VALUE rb_glfwGetTimerValue(VALUE glfw)
{
    return ULL2NUM(glfwGetTimerValue());
}

static VALUE rb_glfwGetVersion(VALUE glfw)
{
    int major, minor, revision;
    glfwGetVersion(&major, &minor, &revision);
    return rb_sprintf("%d.%d.%d", major, minor, revision);
}

static VALUE rb_glfwGetVersionString(VALUE glfw)
{
    const char *str = glfwGetVersionString();
    return str ? rb_str_new_cstr(str) : Qnil;
}

static VALUE rb_glfwGetVideoMode(VALUE glfw, VALUE monitor)
{
    const GLFWvidmode *mode = glfwGetVideoMode(DATA_PTR(monitor));
    if (mode == NULL)
        return Qnil;

    GLFWvidmode *result = ALLOC(GLFWvidmode);
    memcpy(result, mode, sizeof(GLFWvidmode));
    return Data_Wrap_Struct(rb_cVideoMode, NULL, RUBY_DEFAULT_FREE, result);
}

static VALUE rb_glfwGetVideoModes(VALUE glfw, VALUE monitor)
{
    int count;
    const GLFWvidmode *modes = glfwGetVideoModes(DATA_PTR(monitor), &count);

    VALUE ary = rb_ary_new_capa(count);
    for (int i = 0; i < count; i++)
    {
        GLFWvidmode *mode = ALLOC(GLFWvidmode);
        memcpy(mode, &modes[i], sizeof(GLFWvidmode));
        rb_ary_store(ary, i, Data_Wrap_Struct(rb_cMonitor, NULL, RUBY_DEFAULT_FREE, mode));
    }
    return ary;
}

static VALUE rb_glfwGetWindowAttrib(VALUE glfw, VALUE window, VALUE attrib)
{
    return INT2NUM(glfwGetWindowAttrib(DATA_PTR(window), NUM2INT(attrib)));
}

static VALUE rb_glfwGetWindowContentScale(VALUE glfw, VALUE window)
{
    float x, y;
    glfwGetWindowContentScale(DATA_PTR(window), &x, &y);
    return rb_ary_new_from_args(2, DBL2NUM(x), DBL2NUM(y));
    // TODO: vec2
}

static VALUE rb_glfwGetWindowFrameSize(VALUE glfw, VALUE window)
{
    int l, t, r, b;
    glfwGetWindowFrameSize(DATA_PTR(window), &l, &t, &r, &b);
    return rb_ary_new_from_args(4, INT2NUM(l), INT2NUM(t), INT2NUM(r), INT2NUM(b));
}

static VALUE rb_glfwGetWindowMonitor(VALUE glfw, VALUE window)
{
    GLFWmonitor *monitor = glfwGetWindowMonitor(DATA_PTR(window));
    return monitor ? Data_Wrap_Struct(rb_cMonitor, NULL, RUBY_NEVER_FREE, monitor) : Qnil;
}

static VALUE rb_glfwGetWindowOpacity(VALUE glfw, VALUE window)
{
    return DBL2NUM(glfwGetWindowOpacity(DATA_PTR(window)));
}

static VALUE rb_glfwGetWindowPos(VALUE glfw, VALUE window)
{
    int x, y;
    glfwGetWindowPos(DATA_PTR(window), &x, &y);
    return RGSS_Point_New(x, y);
}

static VALUE rb_glfwGetWindowSize(VALUE glfw, VALUE window)
{
    int x, y;
    glfwGetWindowSize(DATA_PTR(window), &x, &y);
    return RGSS_Size_New(x, y);
}

static VALUE rb_glfwHideWindow(VALUE glfw, VALUE window)
{
    glfwHideWindow(DATA_PTR(window));
    return Qnil;
}

static VALUE rb_glfwIconifyWindow(VALUE glfw, VALUE window)
{
    glfwIconifyWindow(DATA_PTR(window));
    return Qnil;
}

static VALUE rb_glfwInit(VALUE glfw)
{
    return RB_BOOL(glfwInit());
}

static VALUE rb_glfwInitHint(VALUE glfw, VALUE hint, VALUE value)
{
    glfwInitHint(NUM2INT(hint), NUM2INT(value));
    return Qnil;
}

static VALUE rb_glfwJoystickIsGamepad(VALUE glfw, VALUE jid)
{
    return RB_BOOL(glfwJoystickIsGamepad(NUM2INT(jid)));
}

static VALUE rb_glfwJoystickPresent(VALUE glfw, VALUE jid)
{
    return glfwJoystickPresent(NUM2INT(jid));
}

static VALUE rb_glfwMakeContextCurrent(VALUE glfw, VALUE window)
{
    glfwMakeContextCurrent(DATA_PTR(window));
    return Qnil;
}

static VALUE rb_glfwMaximizeWindow(VALUE glfw, VALUE window)
{
    glfwMaximizeWindow(DATA_PTR(window));
    return Qnil;
}

static VALUE rb_glfwPollEvents(VALUE glfw)
{
    glfwPollEvents();
    return Qnil;
}

static VALUE rb_glfwPostEmptyEvent(VALUE glfw)
{
    glfwPostEmptyEvent();
    return Qnil;
}

static VALUE rb_glfwRawMouseMotionSupported(VALUE glfw)
{
    return RB_BOOL(glfwRawMouseMotionSupported());
}

static VALUE rb_glfwRequestWindowAttention(VALUE glfw, VALUE window)
{
    glfwRequestWindowAttention(DATA_PTR(window));
    return Qnil;
}

static VALUE rb_glfwRestoreWindow(VALUE glfw, VALUE window)
{
    glfwRestoreWindow(DATA_PTR(window));
    return Qnil;
}

static VALUE rb_glfwSetClipboardString(VALUE glfw, VALUE text)
{
    glfwSetClipboardString(NULL, RTEST(text) ? StringValueCStr(text) : NULL);
    return text;
}

static VALUE rb_glfwSetCursor(VALUE glfw, VALUE window, VALUE cursor)
{
    glfwSetCursor(DATA_PTR(window), RTEST(cursor) ? DATA_PTR(cursor) : NULL);
    return cursor;
}

static VALUE rb_glfwSetCursorPos(VALUE glfw, VALUE window, VALUE point)
{
    int *vec = DATA_PTR(point);
    glfwSetCursorPos(DATA_PTR(window), (double)vec[0], (double)vec[1]);
    return Qnil;
}

static VALUE rb_glfwSetGamma(VALUE glfw, VALUE monitor, VALUE gamma)
{
    glfwSetGamma(DATA_PTR(monitor), NUM2FLT(gamma));
    return Qnil;
}

static VALUE rb_glfwSetGammaRamp(VALUE glfw, VALUE monitor, VALUE ramp)
{
    glfwSetGammaRamp(DATA_PTR(monitor), DATA_PTR(ramp));
    return Qnil;
}

static VALUE rb_glfwSetInputMode(VALUE glfw, VALUE window, VALUE mode, VALUE value)
{
    glfwSetInputMode(DATA_PTR(window), NUM2INT(mode), NUM2INT(value));
    return Qnil;
}

static VALUE rb_glfwSetTime(VALUE glfw, VALUE time)
{
    glfwSetTime(NUM2DBL(time));
    return time;
}

static VALUE rb_glfwSetWindowAspectRatio(VALUE glfw, VALUE window, VALUE numerator, VALUE denominator)
{
    glfwSetWindowAspectRatio(DATA_PTR(window), NUM2INT(numerator), NUM2INT(denominator));
    return Qnil;
}

static VALUE rb_glfwSetWindowAttrib(VALUE glfw, VALUE window, VALUE attrib, VALUE value)
{
    glfwSetWindowAttrib(DATA_PTR(window), NUM2INT(attrib), NUM2INT(value));
    return Qnil;
}

static VALUE rb_glfwSetWindowIcon(VALUE glfw, VALUE window, VALUE images)
{
    GLFWimage *img = NULL;
    GLFWwindow *handle = DATA_PTR(window);
    int count;
    if (RB_TYPE_P(images, T_ARRAY))
    {
        count = rb_array_len(images);
        GLFWimage array[count];

        for (int i = 0; i < count; i++)
        {
            img = DATA_PTR(rb_ary_entry(images, i));
            array[i] = *img;
        }
        glfwSetWindowIcon(handle, count, array);
    }
    else
    {
        if (RTEST(images))
        {
            count = 1;
            img = DATA_PTR(images);
        }
        glfwSetWindowIcon(handle, count, img);
    }
    return Qnil;
}

static VALUE rb_glfwSetWindowMonitor(VALUE glfw, VALUE window, VALUE monitor, VALUE x, VALUE y, VALUE w, VALUE h,
                                     VALUE rate)
{
    GLFWmonitor *ptr = RTEST(monitor) ? DATA_PTR(monitor) : NULL;
    glfwSetWindowMonitor(DATA_PTR(window), ptr, NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h), NUM2INT(rate));
    return Qnil;
}

static VALUE rb_glfwSetWindowOpacity(VALUE glfw, VALUE window, VALUE opacity)
{
    glfwSetWindowOpacity(DATA_PTR(window), NUM2FLT(opacity));
    return opacity;
}

static VALUE rb_glfwSetWindowPos(VALUE glfw, VALUE window, VALUE point)
{
    int *vec = DATA_PTR(point);
    glfwSetWindowPos(DATA_PTR(window), vec[0], vec[1]);
    return point;
}

static VALUE rb_glfwSetWindowShouldClose(VALUE glfw, VALUE window, VALUE close)
{
    glfwSetWindowShouldClose(DATA_PTR(window), NUM2INT(close));
    return Qnil;
}

static VALUE rb_glfwSetWindowSize(VALUE glfw, VALUE window, VALUE size)
{
    int *vec = DATA_PTR(size);
    glfwSetWindowSize(DATA_PTR(window), vec[0], vec[1]);
    return size;
}

static VALUE rb_glfwSetWindowSizeLimits(VALUE glfw, VALUE window, VALUE min_x, VALUE min_y, VALUE max_x, VALUE max_y)
{
    glfwSetWindowSizeLimits(DATA_PTR(window), NUM2INT(min_x), NUM2INT(min_y), NUM2INT(max_x), NUM2INT(max_y));
    return Qnil;
}

static VALUE rb_glfwSetWindowTitle(VALUE glfw, VALUE window, VALUE title)
{
    const char *str = RTEST(title) ? StringValueCStr(title) : "";
    glfwSetWindowTitle(DATA_PTR(window), str);
    return title;
}

static VALUE rb_glfwShowWindow(VALUE glfw, VALUE window)
{
    glfwShowWindow(DATA_PTR(window));
    return Qnil;
}

static VALUE rb_glfwSwapBuffers(VALUE glfw, VALUE window)
{
    glfwSwapBuffers(DATA_PTR(window));
    return Qnil;
}

static VALUE rb_glfwSwapInterval(VALUE glfw, VALUE interval)
{
    glfwSwapInterval(NUM2INT(interval));
    return Qnil;
}

static VALUE rb_glfwTerminate(VALUE glfw)
{
    glfwTerminate();
    return Qnil;
}

static VALUE rb_glfwUpdateGamepadMappings(VALUE glfw, VALUE mappings)
{
    if (!RTEST(mappings))
        return INT2NUM(0);

    const char *str = StringValueCStr(mappings);
    return INT2NUM(glfwUpdateGamepadMappings(str));
}

static VALUE rb_glfwVulkanSupported(VALUE glfw)
{
    return RB_BOOL(glfwVulkanSupported());
}

static VALUE rb_glfwWaitEvents(VALUE glfw)
{
    glfwWaitEvents();
    return Qnil;
}

static VALUE rb_glfwWaitEventsTimeout(VALUE glfw, VALUE timeout)
{
    glfwWaitEventsTimeout(NUM2DBL(timeout));
    return Qnil;
}

static VALUE rb_glfwWindowHint(VALUE glfw, VALUE hint, VALUE value)
{
    glfwWindowHint(NUM2INT(hint), NUM2INT(value));
    return Qnil;
}

static VALUE rb_glfwWindowHintString(VALUE glfw, VALUE hint, VALUE value)
{
    glfwWindowHintString(NUM2INT(hint), StringValueCStr(value));
    return Qnil;
}

static VALUE rb_glfwWindowShouldClose(VALUE glfw, VALUE window)
{
    return RB_BOOL(glfwWindowShouldClose(DATA_PTR(window)));
}

#define GLFW_BLOCK_CALLBACK(ivar, func)                                                                                \
    static VALUE rb_##func(VALUE glfw, VALUE window)                                                                   \
    {                                                                                                                  \
        VALUE proc, current = rb_iv_get(window, ivar);                                                                 \
        if (rb_block_given_p())                                                                                        \
        {                                                                                                              \
            func(DATA_PTR(window), rb_cb_##func);                                                                      \
            proc = rb_block_proc();                                                                                    \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            func(DATA_PTR(window), NULL);                                                                              \
            proc = Qnil;                                                                                               \
        }                                                                                                              \
        rb_iv_set(window, ivar, proc);                                                                                 \
        return current;                                                                                                \
    }

#define UNWRAP_PROC(handle, proc, ivar)                                                                                \
    RGSS_HANDLE *handle;                                                                                                \
    HASH_FIND(hh, handles, &window, sizeof(GLFWwindow *), handle);                                                     \
    if (handle == NULL)                                                                                                \
        return;                                                                                                        \
    VALUE proc = rb_iv_get(handle->value, ivar)

#define CB_WINDOW_POS       "@proc_window_position"
#define CB_WINDOW_SIZE      "@proc_size"
#define CB_CURSOR_POS       "@proc_cursor_pos"
#define CB_FRAMEBUFFER_SIZE "@proc_framebuffer_size"
#define CB_DROP             "@proc_drop"
#define CB_SCROLL           "@proc_scroll"
#define CB_FOCUS            "@proc_focus"
#define CB_MINIMIZE         "@proc_minimize"
#define CB_MAXIMIZE         "@proc_maximize"
#define CB_CLOSE            "@proc_close"
#define CB_REFRESH          "@proc_refresh"
#define CB_SCALE            "@proc_scale"
#define CB_KEY              "@proc_key"
#define CB_MBTN             "@proc_mbtn"
#define CB_ENTER            "@proc_enter"
#define CB_CHAR             "@proc_char"
#define CB_ERROR            "@proc_error"
#define CB_JOYSTICK         "@proc_joystick"
#define CB_MONITOR          "@proc_monitor"

static void rb_cb_glfwSetWindowPosCallback(GLFWwindow *window, int x, int y)
{
    UNWRAP_PROC(handle, proc, CB_WINDOW_POS);
    rb_proc_call(proc, rb_ary_new_from_args(3, handle->value, INT2NUM(x), INT2NUM(y)));
}

static void rb_cb_glfwSetWindowSizeCallback(GLFWwindow *window, int x, int y)
{
    UNWRAP_PROC(handle, proc, CB_WINDOW_SIZE);
    rb_proc_call(proc, rb_ary_new_from_args(3, handle->value, INT2NUM(x), INT2NUM(y)));
}

static void rb_cb_glfwSetCursorPosCallback(GLFWwindow *window, double x, double y)
{
    UNWRAP_PROC(handle, proc, CB_CURSOR_POS);
    rb_proc_call(proc, rb_ary_new_from_args(3, handle->value, INT2NUM((int)round(x)), INT2NUM((int)round(y))));
}

static void rb_cb_glfwSetFramebufferSizeCallback(GLFWwindow *window, int x, int y)
{
    UNWRAP_PROC(handle, proc, CB_FRAMEBUFFER_SIZE);
    rb_proc_call(proc, rb_ary_new_from_args(3, handle->value, INT2NUM(x), INT2NUM(y)));
}

static void rb_cb_glfwSetDropCallback(GLFWwindow *window, int count, const char **paths)
{
    UNWRAP_PROC(handle, proc, CB_DROP);
    VALUE path;
    VALUE ary = rb_ary_new_capa(count);

    for (int i = 0; i < count; i++)
    {
        path = rb_str_new_cstr(paths[i]);
        rb_ary_store(ary, i, path);
    }
    rb_proc_call(proc, rb_ary_new_from_args(3, handle->value, INT2NUM(count), ary));
}

static void rb_cb_glfwSetScrollCallback(GLFWwindow *window, double x, double y)
{
    UNWRAP_PROC(handle, proc, CB_SCROLL);
    rb_proc_call(proc, rb_ary_new_from_args(3, handle->value, DBL2NUM(x), DBL2NUM(y)));
}

static void rb_cb_glfwSetWindowFocusCallback(GLFWwindow *window, int value)
{
    UNWRAP_PROC(handle, proc, CB_FOCUS);
    rb_proc_call(proc, rb_ary_new_from_args(2, handle->value, RB_BOOL(value)));
}

static void rb_cb_glfwSetWindowIconifyCallback(GLFWwindow *window, int value)
{
    UNWRAP_PROC(handle, proc, CB_MINIMIZE);
    rb_proc_call(proc, rb_ary_new_from_args(2, handle->value, RB_BOOL(value)));
}

static void rb_cb_glfwSetWindowMaximizeCallback(GLFWwindow *window, int value)
{
    UNWRAP_PROC(handle, proc, CB_MAXIMIZE);
    rb_proc_call(proc, rb_ary_new_from_args(2, handle->value, RB_BOOL(value)));
}

static void rb_cb_glfwSetWindowCloseCallback(GLFWwindow *window)
{
    UNWRAP_PROC(handle, proc, CB_CLOSE);
    rb_proc_call(proc, rb_ary_new_from_args(1, handle->value));
}

static void rb_cb_glfwSetWindowRefreshCallback(GLFWwindow *window)
{
    UNWRAP_PROC(handle, proc, CB_REFRESH);
    rb_proc_call(proc, rb_ary_new_from_args(1, handle->value));
}

static void rb_cb_glfwSetWindowContentScaleCallback(GLFWwindow *window, float x, float y)
{
    UNWRAP_PROC(handle, proc, CB_SCALE);
    rb_proc_call(proc, rb_ary_new_from_args(3, handle->value, DBL2NUM(x), DBL2NUM(y)));
}

static void rb_cb_glfwSetKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    UNWRAP_PROC(handle, proc, CB_KEY);
    rb_proc_call(proc, rb_ary_new_from_args(5, handle->value, INT2NUM(key), INT2NUM(scancode), INT2NUM(action), INT2NUM(mods)));
}

static void rb_cb_glfwSetMouseButtonCallback(GLFWwindow *window, int key, int action, int mods)
{
    UNWRAP_PROC(handle, proc, CB_MBTN);
    rb_proc_call(proc, rb_ary_new_from_args(4, handle->value, INT2NUM(key), INT2NUM(action), INT2NUM(mods)));
}

static void rb_cb_glfwSetCharCallback(GLFWwindow *window, unsigned int codepoint)
{
    UNWRAP_PROC(handle, proc, CB_CHAR);
    rb_proc_call(proc, rb_ary_new_from_args(2, handle->value, UINT2NUM(codepoint)));
}

static void rb_cb_glfwSetCursorEnterCallback(GLFWwindow *window, int value)
{
    UNWRAP_PROC(handle, proc, CB_ENTER);
    rb_proc_call(proc, rb_ary_new_from_args(2, handle->value, RB_BOOL(value)));
}

GLFW_BLOCK_CALLBACK(CB_WINDOW_POS, glfwSetWindowPosCallback)
GLFW_BLOCK_CALLBACK(CB_WINDOW_SIZE, glfwSetWindowSizeCallback)
GLFW_BLOCK_CALLBACK(CB_CURSOR_POS, glfwSetCursorPosCallback)
GLFW_BLOCK_CALLBACK(CB_FRAMEBUFFER_SIZE, glfwSetFramebufferSizeCallback)
GLFW_BLOCK_CALLBACK(CB_DROP, glfwSetDropCallback)
GLFW_BLOCK_CALLBACK(CB_SCROLL, glfwSetScrollCallback)
GLFW_BLOCK_CALLBACK(CB_FOCUS, glfwSetWindowFocusCallback)
GLFW_BLOCK_CALLBACK(CB_MINIMIZE, glfwSetWindowIconifyCallback)
GLFW_BLOCK_CALLBACK(CB_MAXIMIZE, glfwSetWindowMaximizeCallback)
GLFW_BLOCK_CALLBACK(CB_CLOSE, glfwSetWindowCloseCallback)
GLFW_BLOCK_CALLBACK(CB_REFRESH, glfwSetWindowRefreshCallback)
GLFW_BLOCK_CALLBACK(CB_SCALE, glfwSetWindowContentScaleCallback)
GLFW_BLOCK_CALLBACK(CB_KEY, glfwSetKeyCallback)
GLFW_BLOCK_CALLBACK(CB_MBTN, glfwSetMouseButtonCallback)
GLFW_BLOCK_CALLBACK(CB_ENTER, glfwSetCursorEnterCallback)
GLFW_BLOCK_CALLBACK(CB_CHAR, glfwSetCharCallback)

static void rb_cb_glfwSetErrorCallback(int code, const char *message)
{
    VALUE proc = rb_iv_get(rb_mGLFW, CB_ERROR);
    VALUE str = message ? rb_str_new_cstr(message) : Qnil;
    rb_proc_call(proc, rb_ary_new_from_args(2, INT2NUM(code), str));
}

static VALUE rb_glfwSetErrorCallback(VALUE glfw)
{
    VALUE proc, current = rb_iv_get(glfw, CB_ERROR);
    if (rb_block_given_p())
    {
        proc = rb_block_proc();
        glfwSetErrorCallback(rb_cb_glfwSetErrorCallback);
    }
    else
    {
        proc = Qnil;
        glfwSetErrorCallback(NULL);
    }
    rb_iv_set(glfw, CB_ERROR, proc);
    return current;
}

static void rb_cb_glfwSetJoystickCallback(int jid, int event)
{
    VALUE proc = rb_iv_get(rb_mGLFW, CB_JOYSTICK);
    rb_proc_call(proc, rb_ary_new_from_args(2, INT2NUM(jid), INT2NUM(event)));
}

static VALUE rb_glfwSetJoystickCallback(VALUE glfw)
{
    VALUE proc, current = rb_iv_get(glfw, CB_JOYSTICK);
    if (rb_block_given_p())
    {
        proc = rb_block_proc();
        glfwSetJoystickCallback(rb_cb_glfwSetJoystickCallback);
    }
    else
    {
        proc = Qnil;
        glfwSetJoystickCallback(NULL);
    }
    rb_iv_set(glfw, CB_JOYSTICK, proc);
    return current;
}

static void rb_cb_glfwSetMonitorCallback(GLFWmonitor *monitor, int event)
{
    VALUE proc = rb_iv_get(rb_mGLFW, CB_MONITOR);
    VALUE m = Data_Wrap_Struct(rb_cMonitor, NULL, RUBY_NEVER_FREE, monitor);
    rb_proc_call(proc, rb_ary_new_from_args(2, m, INT2NUM(event)));
}

static VALUE rb_glfwSetMonitorCallback(VALUE glfw)
{
    VALUE proc, current = rb_iv_get(glfw, CB_MONITOR);
    if (rb_block_given_p())
    {
        proc = rb_block_proc();
        glfwSetMonitorCallback(rb_cb_glfwSetMonitorCallback);
    }
    else
    {
        proc = Qnil;
        glfwSetMonitorCallback(NULL);
    }
    rb_iv_set(glfw, CB_MONITOR, proc);
    return current;
}

static void glfw_gamma_free(void *data)
{
    GLFWgammaramp *ramp = data;
    
    if (ramp->red)
        xfree(ramp->red);
    if (ramp->green)
        xfree(ramp->green);
    if (ramp->blue)
        xfree(ramp->blue);
    
    xfree(ramp);
}

static VALUE glfw_gamma_initialize(VALUE self, VALUE red, VALUE green, VALUE blue)
{
    GLFWgammaramp *ramp;
    long size_red, size_green, size_blue;

    size_red = rb_array_len(red);
    size_green = rb_array_len(green);
    size_blue = rb_array_len(blue);

    if (size_red != size_green || size_green != size_blue)
        rb_raise(rb_eArgError, "unequal length gamma ramps provided");

    ramp = DATA_PTR(self);
    ramp->size = (unsigned int) size_red;
    ramp->red = xmalloc(sizeof(unsigned short) * ramp->size);
    ramp->green = xmalloc(sizeof(unsigned short) * ramp->size);
    ramp->blue = xmalloc(sizeof(unsigned short) * ramp->size);

    for (long i = 0; i < size_red; i++)
    {
        ramp->red[i] = NUM2USHORT(rb_ary_entry(red, i));
        ramp->green[i] = NUM2USHORT(rb_ary_entry(green, i));
        ramp->blue[i] = NUM2USHORT(rb_ary_entry(blue, i));        
    }

    return self;
}

static VALUE glfw_gamma_each(VALUE self)
{
    RETURN_ENUMERATOR(self, 0, NULL);

    VALUE args;
    GLFWgammaramp *ramp = DATA_PTR(self);
    for (unsigned int i = 0; i < ramp->size; i++)
    {
        args = rb_ary_new_from_args(3, USHORT2NUM(ramp->red[i]), USHORT2NUM(ramp->green[i]), USHORT2NUM(ramp->blue[i]));
        rb_yield(args);
    }
    return self;
}

static VALUE glfw_gamma_size(VALUE self)
{
    GLFWgammaramp *ramp = DATA_PTR(self);
    return UINT2NUM(ramp->size);
}

#define VIDEO_ATTRIB(field, name)\
static VALUE name(VALUE self)\
{\
    GLFWvidmode *mode = DATA_PTR(self);\
    return INT2NUM(mode->field);\
}

VIDEO_ATTRIB(redBits, glfw_video_red)
VIDEO_ATTRIB(greenBits, glfw_video_green)
VIDEO_ATTRIB(blueBits, glfw_video_blue)
VIDEO_ATTRIB(width, glfw_video_width)
VIDEO_ATTRIB(height, glfw_video_height)
VIDEO_ATTRIB(refreshRate, glfw_video_rate)

#define GAMEPAD_ATTRIB(field, name)

static VALUE glfw_gamepad_axes(VALUE gamepad, VALUE axis)
{
    GLFWgamepadstate *state = DATA_PTR(gamepad);
    return DBL2NUM(state->axes[NUM2INT(axis)]);
}

static VALUE glfw_gamepad_button(VALUE gamepad, VALUE button)
{
    GLFWgamepadstate *state = DATA_PTR(gamepad);
    return RB_BOOL(state->buttons[NUM2INT(button)]);
}

static VALUE glfw_gamma_alloc(VALUE klass)
{
    GLFWgammaramp *ramp = ALLOC(GLFWgammaramp);
    memset(ramp, 0, sizeof(GLFWgammaramp));
    return Data_Wrap_Struct(klass, NULL, glfw_gamma_free, ramp);
}

static VALUE glfw_video_alloc(VALUE klass)
{
    GLFWvidmode *video = ALLOC(GLFWvidmode);
    memset(video, 0, sizeof(GLFWvidmode));
    return Data_Wrap_Struct(klass, NULL, RUBY_DEFAULT_FREE, video);
}

static VALUE glfw_gamepad_alloc(VALUE klass)
{
    GLFWgamepadstate *gamepad = ALLOC(GLFWgamepadstate);
    memset(gamepad, 0, sizeof(GLFWgamepadstate));
    return Data_Wrap_Struct(klass, NULL, RUBY_DEFAULT_FREE, gamepad);
}

void RGSS_Init_GLFW(VALUE parent)
{
    rb_mGLFW = rb_define_module_under(parent, "GLFW");
    rb_cWindow = rb_define_class_under(rb_mGLFW, "Window", rb_cObject);
    rb_cMonitor = rb_define_class_under(rb_mGLFW, "Monitor", rb_cObject);
    rb_cCursor = rb_define_class_under(rb_mGLFW, "Cursor", rb_cObject);
    rb_cGamepadState = rb_define_class_under(rb_mGLFW, "GamepadState", rb_cObject);
    rb_cVideoMode = rb_define_class_under(rb_mGLFW, "VideoMode", rb_cObject);
    rb_cGammaRamp = rb_define_class_under(rb_mGLFW, "GammaRamp", rb_cObject);

    rb_define_alloc_func(rb_cWindow, RGSS_Handle_Alloc);
    rb_define_alloc_func(rb_cMonitor, RGSS_Handle_Alloc);
    rb_define_alloc_func(rb_cCursor, RGSS_Handle_Alloc);
    rb_define_alloc_func(rb_cGamepadState, glfw_gamepad_alloc);
    rb_define_alloc_func(rb_cGammaRamp, glfw_gamma_alloc);
    rb_define_alloc_func(rb_cVideoMode, glfw_video_alloc);

    rb_define_method1(rb_cGamepadState, "axis", glfw_gamepad_axes, 1);
    rb_define_method1(rb_cGamepadState, "button", glfw_gamepad_button, 1);

    rb_define_method3(rb_cGammaRamp, "initialize", glfw_gamma_initialize, 3);
    rb_define_method0(rb_cGammaRamp, "size", glfw_gamma_size, 0);
    rb_define_method0(rb_cGammaRamp, "each", glfw_gamma_each, 0);

    rb_define_method0(rb_cVideoMode, "red_bits", glfw_video_red, 0);
    rb_define_method0(rb_cVideoMode, "green_bits", glfw_video_green, 0);
    rb_define_method0(rb_cVideoMode, "blue_bits", glfw_video_blue, 0);
    rb_define_method0(rb_cVideoMode, "width", glfw_video_width, 0);
    rb_define_method0(rb_cVideoMode, "height", glfw_video_height, 0);
    rb_define_method0(rb_cVideoMode, "refresh_rate", glfw_video_rate, 0);

    rb_define_module_function3(rb_mGLFW, "glfwCreateCursor", rb_glfwCreateCursor, 3);
    rb_define_module_function1(rb_mGLFW, "glfwCreateStandardCursor", rb_glfwCreateStandardCursor, 1);
    rb_define_module_functionm1(rb_mGLFW, "glfwCreateWindow", rb_glfwCreateWindow, -1);
    rb_define_module_function0(rb_mGLFW, "glfwDefaultWindowHints", rb_glfwDefaultWindowHints, 0);
    rb_define_module_function1(rb_mGLFW, "glfwDestroyCursor", rb_glfwDestroyCursor, 1);
    rb_define_module_function1(rb_mGLFW, "glfwDestroyWindow", rb_glfwDestroyWindow, 1);
    rb_define_module_function1(rb_mGLFW, "glfwExtensionSupported", rb_glfwExtensionSupported, 1);
    rb_define_module_function1(rb_mGLFW, "glfwFocusWindow", rb_glfwFocusWindow, 1);
    rb_define_module_function0(rb_mGLFW, "glfwGetClipboardString", rb_glfwGetClipboardString, 0);
    rb_define_module_function0(rb_mGLFW, "glfwGetCurrentContext", rb_glfwGetCurrentContext, 0);
    rb_define_module_function1(rb_mGLFW, "glfwGetCursorPos", rb_glfwGetCursorPos, 1);
    rb_define_module_function0(rb_mGLFW, "glfwGetError", rb_glfwGetError, 0);
    rb_define_module_function1(rb_mGLFW, "glfwGetFramebufferSize", rb_glfwGetFramebufferSize, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetGamepadName", rb_glfwGetGamepadName, 1);
    rb_define_module_function2(rb_mGLFW, "glfwGetGamepadState", rb_glfwGetGamepadState, 2);
    rb_define_module_function1(rb_mGLFW, "glfwGetGammaRamp", rb_glfwGetGammaRamp, 1);
    rb_define_module_function2(rb_mGLFW, "glfwGetInputMode", rb_glfwGetInputMode, 2);
    rb_define_module_function1(rb_mGLFW, "glfwGetJoystickAxes", rb_glfwGetJoystickAxes, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetJoystickButtons", rb_glfwGetJoystickButtons, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetJoystickGUID", rb_glfwGetJoystickGUID, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetJoystickHats", rb_glfwGetJoystickHats, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetJoystickName", rb_glfwGetJoystickName, 1);
    rb_define_module_function2(rb_mGLFW, "glfwGetKey", rb_glfwGetKey, 2);
    rb_define_module_function2(rb_mGLFW, "glfwGetKeyName", rb_glfwGetKeyName, 2);
    rb_define_module_function1(rb_mGLFW, "glfwGetKeyScancode", rb_glfwGetKeyScancode, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetMonitorContentScale", rb_glfwGetMonitorContentScale, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetMonitorName", rb_glfwGetMonitorName, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetMonitorPhysicalSize", rb_glfwGetMonitorPhysicalSize, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetMonitorPos", rb_glfwGetMonitorPos, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetMonitorWorkarea", rb_glfwGetMonitorWorkarea, 1);
    rb_define_module_function0(rb_mGLFW, "glfwGetMonitors", rb_glfwGetMonitors, 0);
    rb_define_module_function2(rb_mGLFW, "glfwGetMouseButton", rb_glfwGetMouseButton, 2);
    rb_define_module_function0(rb_mGLFW, "glfwGetPrimaryMonitor", rb_glfwGetPrimaryMonitor, 0);
    rb_define_module_function1(rb_mGLFW, "glfwGetProcAddress", rb_glfwGetProcAddress, 1);
    rb_define_module_function0(rb_mGLFW, "glfwGetRequiredInstanceExtensions", rb_glfwGetRequiredInstanceExtensions, 0);
    rb_define_module_function0(rb_mGLFW, "glfwGetTime", rb_glfwGetTime, 0);
    rb_define_module_function0(rb_mGLFW, "glfwGetTimerFrequency", rb_glfwGetTimerFrequency, 0);
    rb_define_module_function0(rb_mGLFW, "glfwGetTimerValue", rb_glfwGetTimerValue, 0);
    rb_define_module_function0(rb_mGLFW, "glfwGetVersion", rb_glfwGetVersion, 0);
    rb_define_module_function0(rb_mGLFW, "glfwGetVersionString", rb_glfwGetVersionString, 0);
    rb_define_module_function1(rb_mGLFW, "glfwGetVideoMode", rb_glfwGetVideoMode, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetVideoModes", rb_glfwGetVideoModes, 1);
    rb_define_module_function2(rb_mGLFW, "glfwGetWindowAttrib", rb_glfwGetWindowAttrib, 2);
    rb_define_module_function1(rb_mGLFW, "glfwGetWindowContentScale", rb_glfwGetWindowContentScale, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetWindowFrameSize", rb_glfwGetWindowFrameSize, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetWindowMonitor", rb_glfwGetWindowMonitor, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetWindowOpacity", rb_glfwGetWindowOpacity, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetWindowPos", rb_glfwGetWindowPos, 1);
    rb_define_module_function1(rb_mGLFW, "glfwGetWindowSize", rb_glfwGetWindowSize, 1);
    rb_define_module_function1(rb_mGLFW, "glfwHideWindow", rb_glfwHideWindow, 1);
    rb_define_module_function1(rb_mGLFW, "glfwIconifyWindow", rb_glfwIconifyWindow, 1);
    rb_define_module_function0(rb_mGLFW, "glfwInit", rb_glfwInit, 0);
    rb_define_module_function2(rb_mGLFW, "glfwInitHint", rb_glfwInitHint, 2);
    rb_define_module_function1(rb_mGLFW, "glfwJoystickIsGamepad", rb_glfwJoystickIsGamepad, 1);
    rb_define_module_function1(rb_mGLFW, "glfwJoystickPresent", rb_glfwJoystickPresent, 1);
    rb_define_module_function1(rb_mGLFW, "glfwMakeContextCurrent", rb_glfwMakeContextCurrent, 1);
    rb_define_module_function1(rb_mGLFW, "glfwMaximizeWindow", rb_glfwMaximizeWindow, 1);
    rb_define_module_function0(rb_mGLFW, "glfwPollEvents", rb_glfwPollEvents, 0);
    rb_define_module_function0(rb_mGLFW, "glfwPostEmptyEvent", rb_glfwPostEmptyEvent, 0);
    rb_define_module_function0(rb_mGLFW, "glfwRawMouseMotionSupported", rb_glfwRawMouseMotionSupported, 0);
    rb_define_module_function1(rb_mGLFW, "glfwRequestWindowAttention", rb_glfwRequestWindowAttention, 1);
    rb_define_module_function1(rb_mGLFW, "glfwRestoreWindow", rb_glfwRestoreWindow, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetClipboardString", rb_glfwSetClipboardString, 1);
    rb_define_module_function2(rb_mGLFW, "glfwSetCursor", rb_glfwSetCursor, 2);
    rb_define_module_function2(rb_mGLFW, "glfwSetCursorPos", rb_glfwSetCursorPos, 2);
    rb_define_module_function2(rb_mGLFW, "glfwSetGamma", rb_glfwSetGamma, 2);
    rb_define_module_function2(rb_mGLFW, "glfwSetGammaRamp", rb_glfwSetGammaRamp, 2);
    rb_define_module_function3(rb_mGLFW, "glfwSetInputMode", rb_glfwSetInputMode, 3);
    rb_define_module_function1(rb_mGLFW, "glfwSetTime", rb_glfwSetTime, 1);
    rb_define_module_function3(rb_mGLFW, "glfwSetWindowAspectRatio", rb_glfwSetWindowAspectRatio, 3);
    rb_define_module_function3(rb_mGLFW, "glfwSetWindowAttrib", rb_glfwSetWindowAttrib, 3);
    rb_define_module_function2(rb_mGLFW, "glfwSetWindowIcon", rb_glfwSetWindowIcon, 2);
    rb_define_module_function7(rb_mGLFW, "glfwSetWindowMonitor", rb_glfwSetWindowMonitor, 7);
    rb_define_module_function2(rb_mGLFW, "glfwSetWindowOpacity", rb_glfwSetWindowOpacity, 2);
    rb_define_module_function2(rb_mGLFW, "glfwSetWindowPos", rb_glfwSetWindowPos, 2);
    rb_define_module_function2(rb_mGLFW, "glfwSetWindowShouldClose", rb_glfwSetWindowShouldClose, 2);
    rb_define_module_function2(rb_mGLFW, "glfwSetWindowSize", rb_glfwSetWindowSize, 2);
    rb_define_module_function5(rb_mGLFW, "glfwSetWindowSizeLimits", rb_glfwSetWindowSizeLimits, 5);
    rb_define_module_function2(rb_mGLFW, "glfwSetWindowTitle", rb_glfwSetWindowTitle, 2);
    rb_define_module_function1(rb_mGLFW, "glfwShowWindow", rb_glfwShowWindow, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSwapBuffers", rb_glfwSwapBuffers, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSwapInterval", rb_glfwSwapInterval, 1);
    rb_define_module_function0(rb_mGLFW, "glfwTerminate", rb_glfwTerminate, 0);
    rb_define_module_function1(rb_mGLFW, "glfwUpdateGamepadMappings", rb_glfwUpdateGamepadMappings, 1);
    rb_define_module_function0(rb_mGLFW, "glfwVulkanSupported", rb_glfwVulkanSupported, 0);
    rb_define_module_function0(rb_mGLFW, "glfwWaitEvents", rb_glfwWaitEvents, 0);
    rb_define_module_function1(rb_mGLFW, "glfwWaitEventsTimeout", rb_glfwWaitEventsTimeout, 1);
    rb_define_module_function2(rb_mGLFW, "glfwWindowHint", rb_glfwWindowHint, 2);
    rb_define_module_function2(rb_mGLFW, "glfwWindowHintString", rb_glfwWindowHintString, 2);
    rb_define_module_function1(rb_mGLFW, "glfwWindowShouldClose", rb_glfwWindowShouldClose, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetWindowPosCallback", rb_glfwSetWindowPosCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetCursorPosCallback", rb_glfwSetCursorPosCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetWindowSizeCallback", rb_glfwSetWindowSizeCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetFramebufferSizeCallback", rb_glfwSetFramebufferSizeCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetDropCallback", rb_glfwSetDropCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetScrollCallback", rb_glfwSetScrollCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetWindowFocusCallback", rb_glfwSetWindowFocusCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetWindowIconifyCallback", rb_glfwSetWindowIconifyCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetWindowMaximizeCallback", rb_glfwSetWindowMaximizeCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetWindowCloseCallback", rb_glfwSetWindowCloseCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetWindowContentScaleCallback", rb_glfwSetWindowContentScaleCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetWindowRefreshCallback", rb_glfwSetWindowRefreshCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetKeyCallback", rb_glfwSetKeyCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetMouseButtonCallback", rb_glfwSetMouseButtonCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetCharCallback", rb_glfwSetCharCallback, 1);
    rb_define_module_function1(rb_mGLFW, "glfwSetCursorEnterCallback", rb_glfwSetCursorEnterCallback, 1);
    rb_define_module_function0(rb_mGLFW, "glfwSetErrorCallback", rb_glfwSetErrorCallback, 0);
    rb_define_module_function0(rb_mGLFW, "glfwSetJoystickCallback", rb_glfwSetJoystickCallback, 0);
    rb_define_module_function0(rb_mGLFW, "glfwSetMonitorCallback", rb_glfwSetMonitorCallback, 0);

#pragma region Constants

    rb_define_const(rb_mGLFW, "GLFW_VERSION_MAJOR", INT2NUM(GLFW_VERSION_MAJOR));
    rb_define_const(rb_mGLFW, "GLFW_VERSION_MINOR", INT2NUM(GLFW_VERSION_MINOR));
    rb_define_const(rb_mGLFW, "GLFW_VERSION_REVISION", INT2NUM(GLFW_VERSION_REVISION));
    rb_define_const(rb_mGLFW, "GLFW_TRUE", INT2NUM(GLFW_TRUE));
    rb_define_const(rb_mGLFW, "GLFW_FALSE", INT2NUM(GLFW_FALSE));
    rb_define_const(rb_mGLFW, "GLFW_RELEASE", INT2NUM(GLFW_RELEASE));
    rb_define_const(rb_mGLFW, "GLFW_PRESS", INT2NUM(GLFW_PRESS));
    rb_define_const(rb_mGLFW, "GLFW_REPEAT", INT2NUM(GLFW_REPEAT));
    rb_define_const(rb_mGLFW, "GLFW_HAT_CENTERED", INT2NUM(GLFW_HAT_CENTERED));
    rb_define_const(rb_mGLFW, "GLFW_HAT_UP", INT2NUM(GLFW_HAT_UP));
    rb_define_const(rb_mGLFW, "GLFW_HAT_RIGHT", INT2NUM(GLFW_HAT_RIGHT));
    rb_define_const(rb_mGLFW, "GLFW_HAT_DOWN", INT2NUM(GLFW_HAT_DOWN));
    rb_define_const(rb_mGLFW, "GLFW_HAT_LEFT", INT2NUM(GLFW_HAT_LEFT));
    rb_define_const(rb_mGLFW, "GLFW_HAT_RIGHT_UP", INT2NUM(GLFW_HAT_RIGHT_UP));
    rb_define_const(rb_mGLFW, "GLFW_HAT_RIGHT_DOWN", INT2NUM(GLFW_HAT_RIGHT_DOWN));
    rb_define_const(rb_mGLFW, "GLFW_HAT_LEFT_UP", INT2NUM(GLFW_HAT_LEFT_UP));
    rb_define_const(rb_mGLFW, "GLFW_HAT_LEFT_DOWN", INT2NUM(GLFW_HAT_LEFT_DOWN));
    rb_define_const(rb_mGLFW, "GLFW_KEY_UNKNOWN", INT2NUM(GLFW_KEY_UNKNOWN));
    rb_define_const(rb_mGLFW, "GLFW_KEY_SPACE", INT2NUM(GLFW_KEY_SPACE));
    rb_define_const(rb_mGLFW, "GLFW_KEY_APOSTROPHE", INT2NUM(GLFW_KEY_APOSTROPHE));
    rb_define_const(rb_mGLFW, "GLFW_KEY_COMMA", INT2NUM(GLFW_KEY_COMMA));
    rb_define_const(rb_mGLFW, "GLFW_KEY_MINUS", INT2NUM(GLFW_KEY_MINUS));
    rb_define_const(rb_mGLFW, "GLFW_KEY_PERIOD", INT2NUM(GLFW_KEY_PERIOD));
    rb_define_const(rb_mGLFW, "GLFW_KEY_SLASH", INT2NUM(GLFW_KEY_SLASH));
    rb_define_const(rb_mGLFW, "GLFW_KEY_0", INT2NUM(GLFW_KEY_0));
    rb_define_const(rb_mGLFW, "GLFW_KEY_1", INT2NUM(GLFW_KEY_1));
    rb_define_const(rb_mGLFW, "GLFW_KEY_2", INT2NUM(GLFW_KEY_2));
    rb_define_const(rb_mGLFW, "GLFW_KEY_3", INT2NUM(GLFW_KEY_3));
    rb_define_const(rb_mGLFW, "GLFW_KEY_4", INT2NUM(GLFW_KEY_4));
    rb_define_const(rb_mGLFW, "GLFW_KEY_5", INT2NUM(GLFW_KEY_5));
    rb_define_const(rb_mGLFW, "GLFW_KEY_6", INT2NUM(GLFW_KEY_6));
    rb_define_const(rb_mGLFW, "GLFW_KEY_7", INT2NUM(GLFW_KEY_7));
    rb_define_const(rb_mGLFW, "GLFW_KEY_8", INT2NUM(GLFW_KEY_8));
    rb_define_const(rb_mGLFW, "GLFW_KEY_9", INT2NUM(GLFW_KEY_9));
    rb_define_const(rb_mGLFW, "GLFW_KEY_SEMICOLON", INT2NUM(GLFW_KEY_SEMICOLON));
    rb_define_const(rb_mGLFW, "GLFW_KEY_EQUAL", INT2NUM(GLFW_KEY_EQUAL));
    rb_define_const(rb_mGLFW, "GLFW_KEY_A", INT2NUM(GLFW_KEY_A));
    rb_define_const(rb_mGLFW, "GLFW_KEY_B", INT2NUM(GLFW_KEY_B));
    rb_define_const(rb_mGLFW, "GLFW_KEY_C", INT2NUM(GLFW_KEY_C));
    rb_define_const(rb_mGLFW, "GLFW_KEY_D", INT2NUM(GLFW_KEY_D));
    rb_define_const(rb_mGLFW, "GLFW_KEY_E", INT2NUM(GLFW_KEY_E));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F", INT2NUM(GLFW_KEY_F));
    rb_define_const(rb_mGLFW, "GLFW_KEY_G", INT2NUM(GLFW_KEY_G));
    rb_define_const(rb_mGLFW, "GLFW_KEY_H", INT2NUM(GLFW_KEY_H));
    rb_define_const(rb_mGLFW, "GLFW_KEY_I", INT2NUM(GLFW_KEY_I));
    rb_define_const(rb_mGLFW, "GLFW_KEY_J", INT2NUM(GLFW_KEY_J));
    rb_define_const(rb_mGLFW, "GLFW_KEY_K", INT2NUM(GLFW_KEY_K));
    rb_define_const(rb_mGLFW, "GLFW_KEY_L", INT2NUM(GLFW_KEY_L));
    rb_define_const(rb_mGLFW, "GLFW_KEY_M", INT2NUM(GLFW_KEY_M));
    rb_define_const(rb_mGLFW, "GLFW_KEY_N", INT2NUM(GLFW_KEY_N));
    rb_define_const(rb_mGLFW, "GLFW_KEY_O", INT2NUM(GLFW_KEY_O));
    rb_define_const(rb_mGLFW, "GLFW_KEY_P", INT2NUM(GLFW_KEY_P));
    rb_define_const(rb_mGLFW, "GLFW_KEY_Q", INT2NUM(GLFW_KEY_Q));
    rb_define_const(rb_mGLFW, "GLFW_KEY_R", INT2NUM(GLFW_KEY_R));
    rb_define_const(rb_mGLFW, "GLFW_KEY_S", INT2NUM(GLFW_KEY_S));
    rb_define_const(rb_mGLFW, "GLFW_KEY_T", INT2NUM(GLFW_KEY_T));
    rb_define_const(rb_mGLFW, "GLFW_KEY_U", INT2NUM(GLFW_KEY_U));
    rb_define_const(rb_mGLFW, "GLFW_KEY_V", INT2NUM(GLFW_KEY_V));
    rb_define_const(rb_mGLFW, "GLFW_KEY_W", INT2NUM(GLFW_KEY_W));
    rb_define_const(rb_mGLFW, "GLFW_KEY_X", INT2NUM(GLFW_KEY_X));
    rb_define_const(rb_mGLFW, "GLFW_KEY_Y", INT2NUM(GLFW_KEY_Y));
    rb_define_const(rb_mGLFW, "GLFW_KEY_Z", INT2NUM(GLFW_KEY_Z));
    rb_define_const(rb_mGLFW, "GLFW_KEY_LEFT_BRACKET", INT2NUM(GLFW_KEY_LEFT_BRACKET));
    rb_define_const(rb_mGLFW, "GLFW_KEY_BACKSLASH", INT2NUM(GLFW_KEY_BACKSLASH));
    rb_define_const(rb_mGLFW, "GLFW_KEY_RIGHT_BRACKET", INT2NUM(GLFW_KEY_RIGHT_BRACKET));
    rb_define_const(rb_mGLFW, "GLFW_KEY_GRAVE_ACCENT", INT2NUM(GLFW_KEY_GRAVE_ACCENT));
    rb_define_const(rb_mGLFW, "GLFW_KEY_WORLD_1", INT2NUM(GLFW_KEY_WORLD_1));
    rb_define_const(rb_mGLFW, "GLFW_KEY_WORLD_2", INT2NUM(GLFW_KEY_WORLD_2));
    rb_define_const(rb_mGLFW, "GLFW_KEY_ESCAPE", INT2NUM(GLFW_KEY_ESCAPE));
    rb_define_const(rb_mGLFW, "GLFW_KEY_ENTER", INT2NUM(GLFW_KEY_ENTER));
    rb_define_const(rb_mGLFW, "GLFW_KEY_TAB", INT2NUM(GLFW_KEY_TAB));
    rb_define_const(rb_mGLFW, "GLFW_KEY_BACKSPACE", INT2NUM(GLFW_KEY_BACKSPACE));
    rb_define_const(rb_mGLFW, "GLFW_KEY_INSERT", INT2NUM(GLFW_KEY_INSERT));
    rb_define_const(rb_mGLFW, "GLFW_KEY_DELETE", INT2NUM(GLFW_KEY_DELETE));
    rb_define_const(rb_mGLFW, "GLFW_KEY_RIGHT", INT2NUM(GLFW_KEY_RIGHT));
    rb_define_const(rb_mGLFW, "GLFW_KEY_LEFT", INT2NUM(GLFW_KEY_LEFT));
    rb_define_const(rb_mGLFW, "GLFW_KEY_DOWN", INT2NUM(GLFW_KEY_DOWN));
    rb_define_const(rb_mGLFW, "GLFW_KEY_UP", INT2NUM(GLFW_KEY_UP));
    rb_define_const(rb_mGLFW, "GLFW_KEY_PAGE_UP", INT2NUM(GLFW_KEY_PAGE_UP));
    rb_define_const(rb_mGLFW, "GLFW_KEY_PAGE_DOWN", INT2NUM(GLFW_KEY_PAGE_DOWN));
    rb_define_const(rb_mGLFW, "GLFW_KEY_HOME", INT2NUM(GLFW_KEY_HOME));
    rb_define_const(rb_mGLFW, "GLFW_KEY_END", INT2NUM(GLFW_KEY_END));
    rb_define_const(rb_mGLFW, "GLFW_KEY_CAPS_LOCK", INT2NUM(GLFW_KEY_CAPS_LOCK));
    rb_define_const(rb_mGLFW, "GLFW_KEY_SCROLL_LOCK", INT2NUM(GLFW_KEY_SCROLL_LOCK));
    rb_define_const(rb_mGLFW, "GLFW_KEY_NUM_LOCK", INT2NUM(GLFW_KEY_NUM_LOCK));
    rb_define_const(rb_mGLFW, "GLFW_KEY_PRINT_SCREEN", INT2NUM(GLFW_KEY_PRINT_SCREEN));
    rb_define_const(rb_mGLFW, "GLFW_KEY_PAUSE", INT2NUM(GLFW_KEY_PAUSE));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F1", INT2NUM(GLFW_KEY_F1));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F2", INT2NUM(GLFW_KEY_F2));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F3", INT2NUM(GLFW_KEY_F3));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F4", INT2NUM(GLFW_KEY_F4));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F5", INT2NUM(GLFW_KEY_F5));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F6", INT2NUM(GLFW_KEY_F6));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F7", INT2NUM(GLFW_KEY_F7));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F8", INT2NUM(GLFW_KEY_F8));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F9", INT2NUM(GLFW_KEY_F9));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F10", INT2NUM(GLFW_KEY_F10));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F11", INT2NUM(GLFW_KEY_F11));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F12", INT2NUM(GLFW_KEY_F12));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F13", INT2NUM(GLFW_KEY_F13));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F14", INT2NUM(GLFW_KEY_F14));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F15", INT2NUM(GLFW_KEY_F15));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F16", INT2NUM(GLFW_KEY_F16));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F17", INT2NUM(GLFW_KEY_F17));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F18", INT2NUM(GLFW_KEY_F18));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F19", INT2NUM(GLFW_KEY_F19));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F20", INT2NUM(GLFW_KEY_F20));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F21", INT2NUM(GLFW_KEY_F21));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F22", INT2NUM(GLFW_KEY_F22));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F23", INT2NUM(GLFW_KEY_F23));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F24", INT2NUM(GLFW_KEY_F24));
    rb_define_const(rb_mGLFW, "GLFW_KEY_F25", INT2NUM(GLFW_KEY_F25));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_0", INT2NUM(GLFW_KEY_KP_0));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_1", INT2NUM(GLFW_KEY_KP_1));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_2", INT2NUM(GLFW_KEY_KP_2));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_3", INT2NUM(GLFW_KEY_KP_3));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_4", INT2NUM(GLFW_KEY_KP_4));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_5", INT2NUM(GLFW_KEY_KP_5));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_6", INT2NUM(GLFW_KEY_KP_6));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_7", INT2NUM(GLFW_KEY_KP_7));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_8", INT2NUM(GLFW_KEY_KP_8));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_9", INT2NUM(GLFW_KEY_KP_9));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_DECIMAL", INT2NUM(GLFW_KEY_KP_DECIMAL));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_DIVIDE", INT2NUM(GLFW_KEY_KP_DIVIDE));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_MULTIPLY", INT2NUM(GLFW_KEY_KP_MULTIPLY));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_SUBTRACT", INT2NUM(GLFW_KEY_KP_SUBTRACT));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_ADD", INT2NUM(GLFW_KEY_KP_ADD));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_ENTER", INT2NUM(GLFW_KEY_KP_ENTER));
    rb_define_const(rb_mGLFW, "GLFW_KEY_KP_EQUAL", INT2NUM(GLFW_KEY_KP_EQUAL));
    rb_define_const(rb_mGLFW, "GLFW_KEY_LEFT_SHIFT", INT2NUM(GLFW_KEY_LEFT_SHIFT));
    rb_define_const(rb_mGLFW, "GLFW_KEY_LEFT_CONTROL", INT2NUM(GLFW_KEY_LEFT_CONTROL));
    rb_define_const(rb_mGLFW, "GLFW_KEY_LEFT_ALT", INT2NUM(GLFW_KEY_LEFT_ALT));
    rb_define_const(rb_mGLFW, "GLFW_KEY_LEFT_SUPER", INT2NUM(GLFW_KEY_LEFT_SUPER));
    rb_define_const(rb_mGLFW, "GLFW_KEY_RIGHT_SHIFT", INT2NUM(GLFW_KEY_RIGHT_SHIFT));
    rb_define_const(rb_mGLFW, "GLFW_KEY_RIGHT_CONTROL", INT2NUM(GLFW_KEY_RIGHT_CONTROL));
    rb_define_const(rb_mGLFW, "GLFW_KEY_RIGHT_ALT", INT2NUM(GLFW_KEY_RIGHT_ALT));
    rb_define_const(rb_mGLFW, "GLFW_KEY_RIGHT_SUPER", INT2NUM(GLFW_KEY_RIGHT_SUPER));
    rb_define_const(rb_mGLFW, "GLFW_KEY_MENU", INT2NUM(GLFW_KEY_MENU));
    rb_define_const(rb_mGLFW, "GLFW_KEY_LAST", INT2NUM(GLFW_KEY_LAST));
    rb_define_const(rb_mGLFW, "GLFW_MOD_SHIFT", INT2NUM(GLFW_MOD_SHIFT));
    rb_define_const(rb_mGLFW, "GLFW_MOD_CONTROL", INT2NUM(GLFW_MOD_CONTROL));
    rb_define_const(rb_mGLFW, "GLFW_MOD_ALT", INT2NUM(GLFW_MOD_ALT));
    rb_define_const(rb_mGLFW, "GLFW_MOD_SUPER", INT2NUM(GLFW_MOD_SUPER));
    rb_define_const(rb_mGLFW, "GLFW_MOD_CAPS_LOCK", INT2NUM(GLFW_MOD_CAPS_LOCK));
    rb_define_const(rb_mGLFW, "GLFW_MOD_NUM_LOCK", INT2NUM(GLFW_MOD_NUM_LOCK));
    rb_define_const(rb_mGLFW, "GLFW_MOUSE_BUTTON_1", INT2NUM(GLFW_MOUSE_BUTTON_1));
    rb_define_const(rb_mGLFW, "GLFW_MOUSE_BUTTON_2", INT2NUM(GLFW_MOUSE_BUTTON_2));
    rb_define_const(rb_mGLFW, "GLFW_MOUSE_BUTTON_3", INT2NUM(GLFW_MOUSE_BUTTON_3));
    rb_define_const(rb_mGLFW, "GLFW_MOUSE_BUTTON_4", INT2NUM(GLFW_MOUSE_BUTTON_4));
    rb_define_const(rb_mGLFW, "GLFW_MOUSE_BUTTON_5", INT2NUM(GLFW_MOUSE_BUTTON_5));
    rb_define_const(rb_mGLFW, "GLFW_MOUSE_BUTTON_6", INT2NUM(GLFW_MOUSE_BUTTON_6));
    rb_define_const(rb_mGLFW, "GLFW_MOUSE_BUTTON_7", INT2NUM(GLFW_MOUSE_BUTTON_7));
    rb_define_const(rb_mGLFW, "GLFW_MOUSE_BUTTON_8", INT2NUM(GLFW_MOUSE_BUTTON_8));
    rb_define_const(rb_mGLFW, "GLFW_MOUSE_BUTTON_LAST", INT2NUM(GLFW_MOUSE_BUTTON_LAST));
    rb_define_const(rb_mGLFW, "GLFW_MOUSE_BUTTON_LEFT", INT2NUM(GLFW_MOUSE_BUTTON_LEFT));
    rb_define_const(rb_mGLFW, "GLFW_MOUSE_BUTTON_RIGHT", INT2NUM(GLFW_MOUSE_BUTTON_RIGHT));
    rb_define_const(rb_mGLFW, "GLFW_MOUSE_BUTTON_MIDDLE", INT2NUM(GLFW_MOUSE_BUTTON_MIDDLE));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_1", INT2NUM(GLFW_JOYSTICK_1));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_2", INT2NUM(GLFW_JOYSTICK_2));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_3", INT2NUM(GLFW_JOYSTICK_3));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_4", INT2NUM(GLFW_JOYSTICK_4));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_5", INT2NUM(GLFW_JOYSTICK_5));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_6", INT2NUM(GLFW_JOYSTICK_6));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_7", INT2NUM(GLFW_JOYSTICK_7));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_8", INT2NUM(GLFW_JOYSTICK_8));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_9", INT2NUM(GLFW_JOYSTICK_9));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_10", INT2NUM(GLFW_JOYSTICK_10));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_11", INT2NUM(GLFW_JOYSTICK_11));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_12", INT2NUM(GLFW_JOYSTICK_12));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_13", INT2NUM(GLFW_JOYSTICK_13));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_14", INT2NUM(GLFW_JOYSTICK_14));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_15", INT2NUM(GLFW_JOYSTICK_15));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_16", INT2NUM(GLFW_JOYSTICK_16));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_LAST", INT2NUM(GLFW_JOYSTICK_LAST));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_A", INT2NUM(GLFW_GAMEPAD_BUTTON_A));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_B", INT2NUM(GLFW_GAMEPAD_BUTTON_B));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_X", INT2NUM(GLFW_GAMEPAD_BUTTON_X));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_Y", INT2NUM(GLFW_GAMEPAD_BUTTON_Y));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_LEFT_BUMPER", INT2NUM(GLFW_GAMEPAD_BUTTON_LEFT_BUMPER));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER", INT2NUM(GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_BACK", INT2NUM(GLFW_GAMEPAD_BUTTON_BACK));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_START", INT2NUM(GLFW_GAMEPAD_BUTTON_START));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_GUIDE", INT2NUM(GLFW_GAMEPAD_BUTTON_GUIDE));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_LEFT_THUMB", INT2NUM(GLFW_GAMEPAD_BUTTON_LEFT_THUMB));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_RIGHT_THUMB", INT2NUM(GLFW_GAMEPAD_BUTTON_RIGHT_THUMB));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_DPAD_UP", INT2NUM(GLFW_GAMEPAD_BUTTON_DPAD_UP));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_DPAD_RIGHT", INT2NUM(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_DPAD_DOWN", INT2NUM(GLFW_GAMEPAD_BUTTON_DPAD_DOWN));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_DPAD_LEFT", INT2NUM(GLFW_GAMEPAD_BUTTON_DPAD_LEFT));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_LAST", INT2NUM(GLFW_GAMEPAD_BUTTON_LAST));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_CROSS", INT2NUM(GLFW_GAMEPAD_BUTTON_CROSS));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_CIRCLE", INT2NUM(GLFW_GAMEPAD_BUTTON_CIRCLE));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_SQUARE", INT2NUM(GLFW_GAMEPAD_BUTTON_SQUARE));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_BUTTON_TRIANGLE", INT2NUM(GLFW_GAMEPAD_BUTTON_TRIANGLE));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_AXIS_LEFT_X", INT2NUM(GLFW_GAMEPAD_AXIS_LEFT_X));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_AXIS_LEFT_Y", INT2NUM(GLFW_GAMEPAD_AXIS_LEFT_Y));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_AXIS_RIGHT_X", INT2NUM(GLFW_GAMEPAD_AXIS_RIGHT_X));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_AXIS_RIGHT_Y", INT2NUM(GLFW_GAMEPAD_AXIS_RIGHT_Y));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_AXIS_LEFT_TRIGGER", INT2NUM(GLFW_GAMEPAD_AXIS_LEFT_TRIGGER));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER", INT2NUM(GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER));
    rb_define_const(rb_mGLFW, "GLFW_GAMEPAD_AXIS_LAST", INT2NUM(GLFW_GAMEPAD_AXIS_LAST));
    rb_define_const(rb_mGLFW, "GLFW_NO_ERROR", INT2NUM(GLFW_NO_ERROR));
    rb_define_const(rb_mGLFW, "GLFW_NOT_INITIALIZED", INT2NUM(GLFW_NOT_INITIALIZED));
    rb_define_const(rb_mGLFW, "GLFW_NO_CURRENT_CONTEXT", INT2NUM(GLFW_NO_CURRENT_CONTEXT));
    rb_define_const(rb_mGLFW, "GLFW_INVALID_ENUM", INT2NUM(GLFW_INVALID_ENUM));
    rb_define_const(rb_mGLFW, "GLFW_INVALID_VALUE", INT2NUM(GLFW_INVALID_VALUE));
    rb_define_const(rb_mGLFW, "GLFW_OUT_OF_MEMORY", INT2NUM(GLFW_OUT_OF_MEMORY));
    rb_define_const(rb_mGLFW, "GLFW_API_UNAVAILABLE", INT2NUM(GLFW_API_UNAVAILABLE));
    rb_define_const(rb_mGLFW, "GLFW_VERSION_UNAVAILABLE", INT2NUM(GLFW_VERSION_UNAVAILABLE));
    rb_define_const(rb_mGLFW, "GLFW_PLATFORM_ERROR", INT2NUM(GLFW_PLATFORM_ERROR));
    rb_define_const(rb_mGLFW, "GLFW_FORMAT_UNAVAILABLE", INT2NUM(GLFW_FORMAT_UNAVAILABLE));
    rb_define_const(rb_mGLFW, "GLFW_NO_WINDOW_CONTEXT", INT2NUM(GLFW_NO_WINDOW_CONTEXT));
    rb_define_const(rb_mGLFW, "GLFW_FOCUSED", INT2NUM(GLFW_FOCUSED));
    rb_define_const(rb_mGLFW, "GLFW_ICONIFIED", INT2NUM(GLFW_ICONIFIED));
    rb_define_const(rb_mGLFW, "GLFW_RESIZABLE", INT2NUM(GLFW_RESIZABLE));
    rb_define_const(rb_mGLFW, "GLFW_VISIBLE", INT2NUM(GLFW_VISIBLE));
    rb_define_const(rb_mGLFW, "GLFW_DECORATED", INT2NUM(GLFW_DECORATED));
    rb_define_const(rb_mGLFW, "GLFW_AUTO_ICONIFY", INT2NUM(GLFW_AUTO_ICONIFY));
    rb_define_const(rb_mGLFW, "GLFW_FLOATING", INT2NUM(GLFW_FLOATING));
    rb_define_const(rb_mGLFW, "GLFW_MAXIMIZED", INT2NUM(GLFW_MAXIMIZED));
    rb_define_const(rb_mGLFW, "GLFW_CENTER_CURSOR", INT2NUM(GLFW_CENTER_CURSOR));
    rb_define_const(rb_mGLFW, "GLFW_TRANSPARENT_FRAMEBUFFER", INT2NUM(GLFW_TRANSPARENT_FRAMEBUFFER));
    rb_define_const(rb_mGLFW, "GLFW_HOVERED", INT2NUM(GLFW_HOVERED));
    rb_define_const(rb_mGLFW, "GLFW_FOCUS_ON_SHOW", INT2NUM(GLFW_FOCUS_ON_SHOW));
    rb_define_const(rb_mGLFW, "GLFW_RED_BITS", INT2NUM(GLFW_RED_BITS));
    rb_define_const(rb_mGLFW, "GLFW_GREEN_BITS", INT2NUM(GLFW_GREEN_BITS));
    rb_define_const(rb_mGLFW, "GLFW_BLUE_BITS", INT2NUM(GLFW_BLUE_BITS));
    rb_define_const(rb_mGLFW, "GLFW_ALPHA_BITS", INT2NUM(GLFW_ALPHA_BITS));
    rb_define_const(rb_mGLFW, "GLFW_DEPTH_BITS", INT2NUM(GLFW_DEPTH_BITS));
    rb_define_const(rb_mGLFW, "GLFW_STENCIL_BITS", INT2NUM(GLFW_STENCIL_BITS));
    rb_define_const(rb_mGLFW, "GLFW_ACCUM_RED_BITS", INT2NUM(GLFW_ACCUM_RED_BITS));
    rb_define_const(rb_mGLFW, "GLFW_ACCUM_GREEN_BITS", INT2NUM(GLFW_ACCUM_GREEN_BITS));
    rb_define_const(rb_mGLFW, "GLFW_ACCUM_BLUE_BITS", INT2NUM(GLFW_ACCUM_BLUE_BITS));
    rb_define_const(rb_mGLFW, "GLFW_ACCUM_ALPHA_BITS", INT2NUM(GLFW_ACCUM_ALPHA_BITS));
    rb_define_const(rb_mGLFW, "GLFW_AUX_BUFFERS", INT2NUM(GLFW_AUX_BUFFERS));
    rb_define_const(rb_mGLFW, "GLFW_STEREO", INT2NUM(GLFW_STEREO));
    rb_define_const(rb_mGLFW, "GLFW_SAMPLES", INT2NUM(GLFW_SAMPLES));
    rb_define_const(rb_mGLFW, "GLFW_SRGB_CAPABLE", INT2NUM(GLFW_SRGB_CAPABLE));
    rb_define_const(rb_mGLFW, "GLFW_REFRESH_RATE", INT2NUM(GLFW_REFRESH_RATE));
    rb_define_const(rb_mGLFW, "GLFW_DOUBLEBUFFER", INT2NUM(GLFW_DOUBLEBUFFER));
    rb_define_const(rb_mGLFW, "GLFW_CLIENT_API", INT2NUM(GLFW_CLIENT_API));
    rb_define_const(rb_mGLFW, "GLFW_CONTEXT_VERSION_MAJOR", INT2NUM(GLFW_CONTEXT_VERSION_MAJOR));
    rb_define_const(rb_mGLFW, "GLFW_CONTEXT_VERSION_MINOR", INT2NUM(GLFW_CONTEXT_VERSION_MINOR));
    rb_define_const(rb_mGLFW, "GLFW_CONTEXT_REVISION", INT2NUM(GLFW_CONTEXT_REVISION));
    rb_define_const(rb_mGLFW, "GLFW_CONTEXT_ROBUSTNESS", INT2NUM(GLFW_CONTEXT_ROBUSTNESS));
    rb_define_const(rb_mGLFW, "GLFW_OPENGL_FORWARD_COMPAT", INT2NUM(GLFW_OPENGL_FORWARD_COMPAT));
    rb_define_const(rb_mGLFW, "GLFW_OPENGL_DEBUG_CONTEXT", INT2NUM(GLFW_OPENGL_DEBUG_CONTEXT));
    rb_define_const(rb_mGLFW, "GLFW_OPENGL_PROFILE", INT2NUM(GLFW_OPENGL_PROFILE));
    rb_define_const(rb_mGLFW, "GLFW_CONTEXT_RELEASE_BEHAVIOR", INT2NUM(GLFW_CONTEXT_RELEASE_BEHAVIOR));
    rb_define_const(rb_mGLFW, "GLFW_CONTEXT_NO_ERROR", INT2NUM(GLFW_CONTEXT_NO_ERROR));
    rb_define_const(rb_mGLFW, "GLFW_CONTEXT_CREATION_API", INT2NUM(GLFW_CONTEXT_CREATION_API));
    rb_define_const(rb_mGLFW, "GLFW_SCALE_TO_MONITOR", INT2NUM(GLFW_SCALE_TO_MONITOR));
    rb_define_const(rb_mGLFW, "GLFW_COCOA_RETINA_FRAMEBUFFER", INT2NUM(GLFW_COCOA_RETINA_FRAMEBUFFER));
    rb_define_const(rb_mGLFW, "GLFW_COCOA_FRAME_NAME", INT2NUM(GLFW_COCOA_FRAME_NAME));
    rb_define_const(rb_mGLFW, "GLFW_COCOA_GRAPHICS_SWITCHING", INT2NUM(GLFW_COCOA_GRAPHICS_SWITCHING));
    rb_define_const(rb_mGLFW, "GLFW_X11_CLASS_NAME", INT2NUM(GLFW_X11_CLASS_NAME));
    rb_define_const(rb_mGLFW, "GLFW_X11_INSTANCE_NAME", INT2NUM(GLFW_X11_INSTANCE_NAME));
    rb_define_const(rb_mGLFW, "GLFW_NO_API", INT2NUM(GLFW_NO_API));
    rb_define_const(rb_mGLFW, "GLFW_OPENGL_API", INT2NUM(GLFW_OPENGL_API));
    rb_define_const(rb_mGLFW, "GLFW_OPENGL_ES_API", INT2NUM(GLFW_OPENGL_ES_API));
    rb_define_const(rb_mGLFW, "GLFW_NO_ROBUSTNESS", INT2NUM(GLFW_NO_ROBUSTNESS));
    rb_define_const(rb_mGLFW, "GLFW_NO_RESET_NOTIFICATION", INT2NUM(GLFW_NO_RESET_NOTIFICATION));
    rb_define_const(rb_mGLFW, "GLFW_LOSE_CONTEXT_ON_RESET", INT2NUM(GLFW_LOSE_CONTEXT_ON_RESET));
    rb_define_const(rb_mGLFW, "GLFW_OPENGL_ANY_PROFILE", INT2NUM(GLFW_OPENGL_ANY_PROFILE));
    rb_define_const(rb_mGLFW, "GLFW_OPENGL_CORE_PROFILE", INT2NUM(GLFW_OPENGL_CORE_PROFILE));
    rb_define_const(rb_mGLFW, "GLFW_OPENGL_COMPAT_PROFILE", INT2NUM(GLFW_OPENGL_COMPAT_PROFILE));
    rb_define_const(rb_mGLFW, "GLFW_CURSOR", INT2NUM(GLFW_CURSOR));
    rb_define_const(rb_mGLFW, "GLFW_STICKY_KEYS", INT2NUM(GLFW_STICKY_KEYS));
    rb_define_const(rb_mGLFW, "GLFW_STICKY_MOUSE_BUTTONS", INT2NUM(GLFW_STICKY_MOUSE_BUTTONS));
    rb_define_const(rb_mGLFW, "GLFW_LOCK_KEY_MODS", INT2NUM(GLFW_LOCK_KEY_MODS));
    rb_define_const(rb_mGLFW, "GLFW_RAW_MOUSE_MOTION", INT2NUM(GLFW_RAW_MOUSE_MOTION));
    rb_define_const(rb_mGLFW, "GLFW_CURSOR_NORMAL", INT2NUM(GLFW_CURSOR_NORMAL));
    rb_define_const(rb_mGLFW, "GLFW_CURSOR_HIDDEN", INT2NUM(GLFW_CURSOR_HIDDEN));
    rb_define_const(rb_mGLFW, "GLFW_CURSOR_DISABLED", INT2NUM(GLFW_CURSOR_DISABLED));
    rb_define_const(rb_mGLFW, "GLFW_ANY_RELEASE_BEHAVIOR", INT2NUM(GLFW_ANY_RELEASE_BEHAVIOR));
    rb_define_const(rb_mGLFW, "GLFW_RELEASE_BEHAVIOR_FLUSH", INT2NUM(GLFW_RELEASE_BEHAVIOR_FLUSH));
    rb_define_const(rb_mGLFW, "GLFW_RELEASE_BEHAVIOR_NONE", INT2NUM(GLFW_RELEASE_BEHAVIOR_NONE));
    rb_define_const(rb_mGLFW, "GLFW_NATIVE_CONTEXT_API", INT2NUM(GLFW_NATIVE_CONTEXT_API));
    rb_define_const(rb_mGLFW, "GLFW_EGL_CONTEXT_API", INT2NUM(GLFW_EGL_CONTEXT_API));
    rb_define_const(rb_mGLFW, "GLFW_OSMESA_CONTEXT_API", INT2NUM(GLFW_OSMESA_CONTEXT_API));
    rb_define_const(rb_mGLFW, "GLFW_ARROW_CURSOR", INT2NUM(GLFW_ARROW_CURSOR));
    rb_define_const(rb_mGLFW, "GLFW_IBEAM_CURSOR", INT2NUM(GLFW_IBEAM_CURSOR));
    rb_define_const(rb_mGLFW, "GLFW_CROSSHAIR_CURSOR", INT2NUM(GLFW_CROSSHAIR_CURSOR));
    rb_define_const(rb_mGLFW, "GLFW_HAND_CURSOR", INT2NUM(GLFW_HAND_CURSOR));
    rb_define_const(rb_mGLFW, "GLFW_HRESIZE_CURSOR", INT2NUM(GLFW_HRESIZE_CURSOR));
    rb_define_const(rb_mGLFW, "GLFW_VRESIZE_CURSOR", INT2NUM(GLFW_VRESIZE_CURSOR));
    rb_define_const(rb_mGLFW, "GLFW_CONNECTED", INT2NUM(GLFW_CONNECTED));
    rb_define_const(rb_mGLFW, "GLFW_DISCONNECTED", INT2NUM(GLFW_DISCONNECTED));
    rb_define_const(rb_mGLFW, "GLFW_JOYSTICK_HAT_BUTTONS", INT2NUM(GLFW_JOYSTICK_HAT_BUTTONS));
    rb_define_const(rb_mGLFW, "GLFW_COCOA_CHDIR_RESOURCES", INT2NUM(GLFW_COCOA_CHDIR_RESOURCES));
    rb_define_const(rb_mGLFW, "GLFW_COCOA_MENUBAR", INT2NUM(GLFW_COCOA_MENUBAR));
    rb_define_const(rb_mGLFW, "GLFW_DONT_CARE", INT2NUM(GLFW_DONT_CARE));

#pragma endregion
}
