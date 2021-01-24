#include "game.h"
#include "utf8.h"

#define INPUT RGSS_INPUT
#define INPUT_RELEASE     -2
#define INPUT_NONE        -1
#define INPUT_PRESS       0
#define INPUT_DOWN        1
#define INPUT_REPEAT_RATE 3

#define RGSS_TEXT_CALLBACK  "@on_text"
#define RGSS_FOCUS_CALLBACK "@on_focus"

#define RGSS_INPUT RGSS_GAME.input
#define KEY(k)     RGSS_INPUT.state.key[k]
#define MBTN(btn)  RGSS_INPUT.state.mouse[btn]
#define GPBTN(btn) RGSS_INPUT.state.gamepad[btn]

VALUE rb_mInput;


static inline void RGSS_Mapping_Init(vec_int_t *array, VALUE inputs, int max_value)
{
    if (!RTEST(inputs))
        return;

    int value;
    vec_init(array);
    if (RB_TYPE_P(inputs, T_ARRAY))
    {
        long len = rb_array_len(inputs);
        for (long i = 0; i < len; i++)
        {
            value = NUM2INT(rb_ary_entry(inputs, i));
            if (value < 0 || value > max_value)
            {
                rb_warn("invalid input specified: %d (out of range)", value);
                continue;
            }
            vec_push(array, value);
        }
    }
    else
    {
        value = NUM2INT(inputs);
        if (value < 0 || value > max_value)
        {
            rb_warn("invalid input specified: %d (out of range)", value);
            return;
        }
        vec_push(array, value);
    }
}


static VALUE RGSS_Input_Bind(int argc, VALUE *argv, VALUE input)
{

    RGSS_ASSERT_GAME;

    VALUE name, keys, mouse, gamepad;
    rb_scan_args(argc, argv, "13", &name, &keys, &mouse, &gamepad);

    RGSS_Mapping *mapping;
    ID id = RB_TYPE_P(name, T_SYMBOL) ? SYM2ID(name) : rb_intern_str(name);;

    HASH_FIND(hh, RGSS_INPUT.bindings, &id, sizeof(ID), mapping);
    if (mapping)
    {   
        // Free any mapping that may exist at key
        HASH_DEL(RGSS_INPUT.bindings, mapping);
        xfree(mapping);
    }

    mapping = ALLOC(RGSS_Mapping);
    memset(mapping, 0, sizeof(RGSS_Mapping));

    mapping->id = id;
    RGSS_Mapping_Init(&mapping->keys, keys, GLFW_KEY_LAST);
    RGSS_Mapping_Init(&mapping->mouse_buttons, mouse, GLFW_MOUSE_BUTTON_LAST);
    RGSS_Mapping_Init(&mapping->gamepad_buttons, gamepad, GLFW_GAMEPAD_BUTTON_LAST);

    HASH_ADD(hh, RGSS_INPUT.bindings, id, sizeof(ID), mapping);
    return Qtrue;
}

static VALUE RGSS_Input_Unbind(VALUE input, VALUE name)
{

    RGSS_Mapping *mapping;
    ID id = RB_TYPE_P(name, T_SYMBOL) ? SYM2ID(name) : rb_intern(StringValueCStr(name));

    HASH_FIND(hh, RGSS_INPUT.bindings, &id, sizeof(ID), mapping);

    if (mapping)
    {
        HASH_DEL(RGSS_INPUT.bindings, mapping);

        vec_deinit(&mapping->keys);
        vec_deinit(&mapping->mouse_buttons);
        vec_deinit(&mapping->gamepad_buttons);
        xfree(mapping);
        return Qtrue;
    }

    return Qfalse;
}

static VALUE RGSS_Input_EachBinding(VALUE input)
{
    RETURN_ENUMERATOR(input, 0, NULL);

    RGSS_Mapping *mapping, *temp;
    VALUE name, keys, mouse, gamepad;

    int i, value;

    HASH_ITER(hh, RGSS_INPUT.bindings, mapping, temp)
    {        
        name = ID2SYM(mapping->id);
        keys = rb_ary_new_capa(mapping->keys.length);
        mouse = rb_ary_new_capa(mapping->mouse_buttons.length);
        gamepad = rb_ary_new_capa(mapping->gamepad_buttons.length);

        vec_foreach(&mapping->keys, value, i)
            rb_ary_push(keys, INT2NUM(value));

        vec_foreach(&mapping->mouse_buttons, value, i)
            rb_ary_push(mouse, INT2NUM(value));

        vec_foreach(&mapping->gamepad_buttons, value, i)
            rb_ary_push(gamepad, INT2NUM(value));

        rb_yield_values(4, name, keys, mouse, gamepad);
    }

    return Qnil;
}

static void RGSS_Input_KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_UNKNOWN)
        return;

    if (action == GLFW_PRESS)
        RGSS_INPUT.state.key[key] = INPUT_PRESS;
    else if (action == GLFW_RELEASE)
        RGSS_INPUT.state.key[key] = INPUT_RELEASE;
}

static void RGSS_Input_MouseCallback(GLFWwindow *window, int key, int action, int mods)
{

    if (key == GLFW_KEY_UNKNOWN)
        return;

    if (action == GLFW_PRESS)
        RGSS_INPUT.state.mouse[key] = INPUT_PRESS;
    else if (action == GLFW_RELEASE)
        RGSS_INPUT.state.mouse[key] = INPUT_RELEASE;
}

void RGSS_Input_Update(void)
{

    for (int i = 0; i < GLFW_KEY_LAST + 1; i++)
    {
        if (KEY(i) >= INPUT_PRESS)
        {
            if (++KEY(i) > INPUT_REPEAT_RATE)
                KEY(i) = INPUT_DOWN;
        }
        else if (KEY(i) == INPUT_RELEASE)
            KEY(i) = INPUT_NONE;
    }

    for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST + 1; i++)
    {
        if (MBTN(i) >= INPUT_PRESS)
        {
            if (++MBTN(i) > INPUT_REPEAT_RATE)
                MBTN(i) = INPUT_DOWN;
        }
        else if (MBTN(i) == INPUT_RELEASE)
            MBTN(i) = INPUT_NONE;
    }

    RGSS_INPUT.state.scroll_x = 0.0;
    RGSS_INPUT.state.scroll_y = 0.0;
    RGSS_INPUT.cursor_valid = false;
}

static VALUE RGSS_Input_IsTriggered(VALUE input, VALUE sym)
{
    ID id = SYM2ID(sym);

    RGSS_Mapping *mapping;
    HASH_FIND(hh, RGSS_INPUT.bindings, &id, sizeof(ID), mapping);

    if (mapping == NULL)
        return Qfalse;

    int state, value, i;

    // Check keyboard keys
    vec_foreach(&mapping->keys, value, i)
    {
        state = RGSS_INPUT.state.key[value];
        if (state == INPUT_PRESS)
            return Qtrue;
    }

    // Check mouse input
    vec_foreach(&mapping->mouse_buttons, value, i)
    {
        state = RGSS_INPUT.state.mouse[value];
        if (state == INPUT_PRESS)
            return Qtrue;
    }

    // TODO: Check if gamepad connected, then query for input if so

    return Qfalse;
}

static VALUE RGSS_Input_IsDown(VALUE input, VALUE sym)
{
    ID id = SYM2ID(sym);

    RGSS_Mapping *mapping;
    HASH_FIND(hh, RGSS_INPUT.bindings, &id, sizeof(ID), mapping);

    if (mapping == NULL)
        return Qfalse;

    int state, value, i;
    
    // Check keyboard keys
    vec_foreach(&mapping->keys, value, i)
    {
        state = RGSS_INPUT.state.key[value];
        if (state >= INPUT_PRESS)
            return Qtrue;
    }

    // Check mouse input
    vec_foreach(&mapping->mouse_buttons, value, i)
    {
        state = RGSS_INPUT.state.mouse[value];
        if (state >= INPUT_PRESS)
            return Qtrue;
    }

    // TODO: Check if gamepad connected, then query for input if so

    return Qfalse;
}

static VALUE RGSS_Input_IsRepeated(VALUE input, VALUE sym)
{
    ID id = SYM2ID(sym);

    RGSS_Mapping *mapping;
    HASH_FIND(hh, RGSS_INPUT.bindings, &id, sizeof(ID), mapping);

    if (mapping == NULL)
        return Qfalse;

    int state, value, i;
    
    // Check keyboard keys
    vec_foreach(&mapping->keys, value, i)
    {
        state = RGSS_INPUT.state.key[value];
        if (state == INPUT_REPEAT_RATE)
            return Qtrue;
    }

    // Check mouse input
    vec_foreach(&mapping->mouse_buttons, value, i)
    {
        state = RGSS_INPUT.state.mouse[value];
        if (state == INPUT_REPEAT_RATE)
            return Qtrue;
    }

    // TODO: Check if gamepad connected, then query for input if so

    return Qfalse;
}

static VALUE RGSS_Input_IsReleased(VALUE input, VALUE sym)
{
    ID id = SYM2ID(sym);

    RGSS_Mapping *mapping;
    HASH_FIND(hh, RGSS_INPUT.bindings, &id, sizeof(ID), mapping);

    if (mapping == NULL)
        return Qfalse;

    int state, value, i;
    
    // Check keyboard keys
    vec_foreach(&mapping->keys, value, i)
    {
        state = RGSS_INPUT.state.key[value];
        if (state == INPUT_RELEASE)
            return Qtrue;
    }

    // Check mouse input
    vec_foreach(&mapping->mouse_buttons, value, i)
    {
        state = RGSS_INPUT.state.mouse[value];
        if (state == INPUT_RELEASE)
            return Qtrue;
    }

    // TODO: Check if gamepad connected, then query for input if so

    return Qfalse;
}

static VALUE RGSS_Input_IsUp(VALUE input, VALUE sym)
{
    ID id = SYM2ID(sym);

    RGSS_Mapping *mapping;
    HASH_FIND(hh, RGSS_INPUT.bindings, &id, sizeof(ID), mapping);

    if (mapping == NULL)
        return Qfalse;

    int state, value, i;
    
    // Check keyboard keys
    vec_foreach(&mapping->keys, value, i)
    {
        state = RGSS_INPUT.state.key[value];
        if (state <= INPUT_NONE)
            return Qtrue;
    }

    // Check mouse input
    vec_foreach(&mapping->mouse_buttons, value, i)
    {
        state = RGSS_INPUT.state.mouse[value];
        if (state <= INPUT_NONE)
            return Qtrue;
    }

    // TODO: Check if gamepad connected, then query for input if so

    return Qfalse;
}

static VALUE RGSS_Input_GetScrollX(VALUE inpue)
{
    return DBL2NUM(RGSS_INPUT.state.scroll_x);
}

static VALUE RGSS_Input_GetScrollY(VALUE inpue)
{
    return DBL2NUM(RGSS_INPUT.state.scroll_y);
}

static void RGSS_Input_ScrollCallback(GLFWwindow *window, double x, double y)
{
    RGSS_INPUT.state.scroll_x += x;
    RGSS_INPUT.state.scroll_y += y;
}

static VALUE RGSS_Input_Clear(VALUE input)
{
    memset(RGSS_INPUT.state.key, INPUT_NONE, sizeof(RGSS_INPUT.state.key));
    memset(RGSS_INPUT.state.mouse, INPUT_NONE, sizeof(RGSS_INPUT.state.mouse));
    memset(RGSS_INPUT.state.gamepad, INPUT_NONE, sizeof(RGSS_INPUT.state.gamepad));

    RGSS_INPUT.state.scroll_x = 0.0;
    RGSS_INPUT.state.scroll_y = 0.0;
    RGSS_INPUT.cursor_valid = false;
}

static void RGSS_Input_FocusCallback(GLFWwindow *window, int focused)
{
    if (!focused)
        RGSS_Input_Clear(rb_mInput);

    VALUE proc = rb_iv_get(rb_mInput, RGSS_FOCUS_CALLBACK);
    if (RTEST(proc))
    {
        rb_proc_call(proc, rb_ary_new_from_args(1, RB_BOOL(focused)));
    }
}

static VALUE RGSS_Input_OnFocus(VALUE input)
{
    VALUE proc, current;

    current = rb_iv_get(rb_mInput, RGSS_FOCUS_CALLBACK);
    proc = rb_block_given_p() ? rb_block_proc() : Qnil;

    rb_iv_set(rb_mInput, RGSS_FOCUS_CALLBACK, proc);
    return current;
}

void RGSS_Input_Init(GLFWwindow *window)
{
    glfwSetKeyCallback(window, RGSS_Input_KeyCallback);
    glfwSetMouseButtonCallback(window, RGSS_Input_MouseCallback);
    glfwSetScrollCallback(window, RGSS_Input_ScrollCallback);
    glfwSetWindowFocusCallback(window, RGSS_Input_FocusCallback);

    RGSS_INPUT.bindings = NULL;
    RGSS_INPUT.cursor = NULL;
    RGSS_Input_Clear(rb_mInput);

    char *mappings = getenv("SDL_GAMECONTROLLERCONFIG");
    if (mappings != NULL)
    {
        RGSS_LogDebug("SDL_GAMECONTROLLERCONFIG environment variable found");
        if (glfwUpdateGamepadMappings(mappings))
            RGSS_LogInfo("Successfully updated gamepad mappings from environment");
        else
            RGSS_LogError("Faled to parse/update mappings from envoronment variable");
    }
}

void RGSS_Input_Deinit(GLFWwindow *window)
{
    if (window == NULL)
        return;

    glfwSetKeyCallback(window, NULL);
    glfwSetMouseButtonCallback(window, NULL);
    glfwSetScrollCallback(window, NULL);
    glfwSetWindowFocusCallback(window, NULL);
    
    RGSS_INPUT.bindings = NULL;
    RGSS_INPUT.cursor = NULL;
    RGSS_Input_Clear(rb_mInput);
}


static void RGSS_Input_TextCallback(GLFWwindow *window, unsigned int codepoint)
{
    VALUE proc = rb_iv_get(rb_mInput, RGSS_TEXT_CALLBACK);
    if (!RTEST(proc))
    {
        glfwSetCharCallback(RGSS_GAME.window, NULL);
        return;
    }

    // TODO: Cache and fire during update?
    // Just do a begin/end text capture class or method/block?

    size_t size = utf8codepointcalcsize(&codepoint);

    VALUE args = rb_utf8_str_new((char *)&codepoint, size);
    rb_proc_call(proc, rb_ary_new_from_args(1, args));
}

static VALUE RGSS_Input_OnText(VALUE input)
{
    GLFWcharfun callback;
    VALUE proc, current;

    current = rb_iv_get(input, RGSS_TEXT_CALLBACK);
    if (rb_block_given_p())
    {
        proc = rb_block_proc();
        callback = RGSS_Input_TextCallback;
    }
    else
    {
        proc = Qnil;
        callback = NULL;
    }

    rb_iv_set(input, RGSS_TEXT_CALLBACK, proc);
    glfwSetCharCallback(RGSS_GAME.window, callback);

    return current;
}

static VALUE RGSS_Input_IsCursorEnabled(VALUE input)
{
    RGSS_ASSERT_GAME;
    int mode = glfwGetInputMode(RGSS_GAME.window, GLFW_CURSOR);
    return RB_BOOL(mode != GLFW_CURSOR_DISABLED);
}

static VALUE RGSS_Input_IsCursorHidden(VALUE input)
{
    RGSS_ASSERT_GAME;
    int mode = glfwGetInputMode(RGSS_GAME.window, GLFW_CURSOR);
    return RB_BOOL(mode != GLFW_CURSOR_HIDDEN);
}

static VALUE RGSS_Input_IsCursorVisible(VALUE input)
{
    RGSS_ASSERT_GAME;
    int mode = glfwGetInputMode(RGSS_GAME.window, GLFW_CURSOR);
    return RB_BOOL(mode == GLFW_CURSOR_NORMAL);
}

static VALUE RGSS_Input_EnableCursor(VALUE input, VALUE value)
{
    RGSS_ASSERT_GAME;
    glfwSetInputMode(RGSS_GAME.window, GLFW_CURSOR, RTEST(value) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    return Qnil;
}

static VALUE RGSS_Input_HideCursor(VALUE input, VALUE value)
{
    RGSS_ASSERT_GAME;
    glfwSetInputMode(RGSS_GAME.window, GLFW_CURSOR, RTEST(value) ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
    return Qnil;
}

static inline void RGSS_Input_UpdateCursor(void)
{
    if (RGSS_INPUT.cursor_valid)
        return;

    double x, y;
    glfwGetCursorPos(RGSS_GAME.window, &x, &y);

    RGSS_Rect viewport = RGSS_GAME.graphics.viewport;
    vec3 vec;
    float cursor[3] = { (float) x, (viewport.height + (viewport.y * 2)) - y, 0 };
    if (RGSS_GAME.graphics.viewport.x != 0)
        cursor[1] = viewport.height - y;

    float vp[4] = { viewport.x, viewport.y, viewport.width, viewport.height };
    glm_unproject(cursor, RGSS_GAME.graphics.projection, vp, vec);

    RGSS_INPUT.cursor_position.x = (int) roundf(vec[0]);
    RGSS_INPUT.cursor_position.y = (int) roundf(vec[1]);
    RGSS_INPUT.cursor_valid = true;
}

static VALUE RGSS_Input_GetCursorX(VALUE input)
{
    RGSS_Input_UpdateCursor();
    return INT2NUM(RGSS_INPUT.cursor_position.x);
}

static VALUE RGSS_Input_GetCursorY(VALUE input)
{
    RGSS_Input_UpdateCursor();
    return INT2NUM(RGSS_INPUT.cursor_position.y);
}

static VALUE RGSS_Input_GetCursor(VALUE input)
{
    RGSS_ASSERT_GAME;
    RGSS_Input_UpdateCursor();
    return Data_Wrap_Struct(rb_cPoint, NULL, RUBY_NEVER_FREE, &RGSS_INPUT.cursor_position);
}

static VALUE RGSS_Input_HitTest(int argc, VALUE *argv, VALUE input)
{
    if (argc != 1 && argc != 2 && argc != 4)
        rb_raise(rb_eArgError, "wrong number of arguments (given %d, expected 1, 2, or 4)");

    RGSS_Rect rect;
    RGSS_ParseRect(argc, argv, &rect); // TODO: Get rid of this
    RGSS_Point *pnt = DATA_PTR(RGSS_Input_GetCursor(input));
  
    return RB_BOOL(rect.x <= pnt->x && pnt->x < rect.x + rect.width && rect.y <= pnt->y && pnt->y < rect.y + rect.height);
}


static VALUE RGSS_Input_SetCursorImage(int argc, VALUE *argv, VALUE input)
{
    RGSS_ASSERT_GAME;

    VALUE source, x, y;
    rb_scan_args(argc, argv, "12", &source, &x, &y);

    GLFWimage image;
    GLFWcursor *cursor = NULL;

    if (RTEST(source))
    {
        int xhot = RTEST(x) ? NUM2INT(x) : 0;
        int yhot = RTEST(y) ? NUM2INT(y) : 0;


        if (RB_TYPE_P(source, T_DATA))
        {
            image = *((GLFWimage*) DATA_PTR(source));
            cursor = glfwCreateCursor(&image, xhot, yhot);
        }
        else if (RB_TYPE_P(source, T_STRING))
        {
            RGSS_Image_Load(StringValueCStr(source), &image.width, &image.height, &image.pixels);
            cursor = glfwCreateCursor(&image, xhot, yhot);
            xfree(image.pixels);
        }
        else
        {
            int shape = NUM2INT(source);
            cursor = glfwCreateStandardCursor(shape);
        }
    }

    if (RGSS_INPUT.cursor)
    {
        glfwDestroyCursor(RGSS_INPUT.cursor);
    }

    RGSS_INPUT.cursor = cursor;
    glfwSetCursor(RGSS_GAME.window, cursor);

    return source;
}

void RGSS_Init_Input(VALUE parent)
{
    rb_mInput = rb_define_module_under(parent, "Input");

    rb_define_singleton_methodm1(rb_mInput, "bind", RGSS_Input_Bind, -1);
    rb_define_singleton_method1(rb_mInput, "unbind", RGSS_Input_Unbind, 1);
    rb_define_singleton_method0(rb_mInput, "each_binding", RGSS_Input_EachBinding, 0);
    rb_define_singleton_method1(rb_mInput, "trigger?", RGSS_Input_IsTriggered, 1);
    rb_define_singleton_method1(rb_mInput, "down?", RGSS_Input_IsDown, 1);
    rb_define_singleton_method1(rb_mInput, "repeat?", RGSS_Input_IsRepeated, 1);
    rb_define_singleton_method1(rb_mInput, "release?", RGSS_Input_IsReleased, 1);
    rb_define_singleton_method1(rb_mInput, "up?", RGSS_Input_IsUp, 1);
    rb_define_singleton_method0(rb_mInput, "scroll_x", RGSS_Input_GetScrollX, 0);
    rb_define_singleton_method0(rb_mInput, "scroll_y", RGSS_Input_GetScrollY, 0);
    rb_define_singleton_methodm1(rb_mInput, "hit_test", RGSS_Input_HitTest, -1);

    VALUE singleton = rb_singleton_class(rb_mInput);
    rb_define_alias(singleton, "press?", "down?");
    rb_define_alias(singleton, "none?", "up?");

    rb_define_singleton_method0(rb_mInput, "on_text", RGSS_Input_OnText, 0);
    rb_define_singleton_method0(rb_mInput, "on_focus", RGSS_Input_OnFocus, 0);

    rb_define_singleton_method0(rb_mInput, "cursor", RGSS_Input_GetCursor, 0);
    rb_define_singleton_method1(rb_mInput, "enable_cursor", RGSS_Input_EnableCursor, 1);
    rb_define_singleton_method1(rb_mInput, "hide_cursor", RGSS_Input_HideCursor, 1);
    rb_define_singleton_method0(rb_mInput, "cursor_enabled?", RGSS_Input_IsCursorEnabled, 0);
    rb_define_singleton_method0(rb_mInput, "cursor_hidden?", RGSS_Input_IsCursorHidden, 0);
    rb_define_singleton_method0(rb_mInput, "cursor_visible?", RGSS_Input_IsCursorVisible, 0);
    rb_define_singleton_method0(rb_mInput, "cursor_x", RGSS_Input_GetCursorX, 0);
    rb_define_singleton_method0(rb_mInput, "cursor_y", RGSS_Input_GetCursorY, 0);
    rb_define_singleton_methodm1(rb_mInput, "cursor_image", RGSS_Input_SetCursorImage, -1);

    rb_define_const(rb_mInput, "KEY_SPACE", INT2NUM(GLFW_KEY_SPACE));
    rb_define_const(rb_mInput, "KEY_APOSTROPHE", INT2NUM(GLFW_KEY_APOSTROPHE));
    rb_define_const(rb_mInput, "KEY_COMMA", INT2NUM(GLFW_KEY_COMMA));
    rb_define_const(rb_mInput, "KEY_MINUS", INT2NUM(GLFW_KEY_MINUS));
    rb_define_const(rb_mInput, "KEY_PERIOD", INT2NUM(GLFW_KEY_PERIOD));
    rb_define_const(rb_mInput, "KEY_SLASH", INT2NUM(GLFW_KEY_SLASH));
    rb_define_const(rb_mInput, "KEY_0", INT2NUM(GLFW_KEY_0));
    rb_define_const(rb_mInput, "KEY_1", INT2NUM(GLFW_KEY_1));
    rb_define_const(rb_mInput, "KEY_2", INT2NUM(GLFW_KEY_2));
    rb_define_const(rb_mInput, "KEY_3", INT2NUM(GLFW_KEY_3));
    rb_define_const(rb_mInput, "KEY_4", INT2NUM(GLFW_KEY_4));
    rb_define_const(rb_mInput, "KEY_5", INT2NUM(GLFW_KEY_5));
    rb_define_const(rb_mInput, "KEY_6", INT2NUM(GLFW_KEY_6));
    rb_define_const(rb_mInput, "KEY_7", INT2NUM(GLFW_KEY_7));
    rb_define_const(rb_mInput, "KEY_8", INT2NUM(GLFW_KEY_8));
    rb_define_const(rb_mInput, "KEY_9", INT2NUM(GLFW_KEY_9));
    rb_define_const(rb_mInput, "KEY_SEMICOLON", INT2NUM(GLFW_KEY_SEMICOLON));
    rb_define_const(rb_mInput, "KEY_EQUAL", INT2NUM(GLFW_KEY_EQUAL));
    rb_define_const(rb_mInput, "KEY_A", INT2NUM(GLFW_KEY_A));
    rb_define_const(rb_mInput, "KEY_B", INT2NUM(GLFW_KEY_B));
    rb_define_const(rb_mInput, "KEY_C", INT2NUM(GLFW_KEY_C));
    rb_define_const(rb_mInput, "KEY_D", INT2NUM(GLFW_KEY_D));
    rb_define_const(rb_mInput, "KEY_E", INT2NUM(GLFW_KEY_E));
    rb_define_const(rb_mInput, "KEY_F", INT2NUM(GLFW_KEY_F));
    rb_define_const(rb_mInput, "KEY_G", INT2NUM(GLFW_KEY_G));
    rb_define_const(rb_mInput, "KEY_H", INT2NUM(GLFW_KEY_H));
    rb_define_const(rb_mInput, "KEY_I", INT2NUM(GLFW_KEY_I));
    rb_define_const(rb_mInput, "KEY_J", INT2NUM(GLFW_KEY_J));
    rb_define_const(rb_mInput, "KEY_K", INT2NUM(GLFW_KEY_K));
    rb_define_const(rb_mInput, "KEY_L", INT2NUM(GLFW_KEY_L));
    rb_define_const(rb_mInput, "KEY_M", INT2NUM(GLFW_KEY_M));
    rb_define_const(rb_mInput, "KEY_N", INT2NUM(GLFW_KEY_N));
    rb_define_const(rb_mInput, "KEY_O", INT2NUM(GLFW_KEY_O));
    rb_define_const(rb_mInput, "KEY_P", INT2NUM(GLFW_KEY_P));
    rb_define_const(rb_mInput, "KEY_Q", INT2NUM(GLFW_KEY_Q));
    rb_define_const(rb_mInput, "KEY_R", INT2NUM(GLFW_KEY_R));
    rb_define_const(rb_mInput, "KEY_S", INT2NUM(GLFW_KEY_S));
    rb_define_const(rb_mInput, "KEY_T", INT2NUM(GLFW_KEY_T));
    rb_define_const(rb_mInput, "KEY_U", INT2NUM(GLFW_KEY_U));
    rb_define_const(rb_mInput, "KEY_V", INT2NUM(GLFW_KEY_V));
    rb_define_const(rb_mInput, "KEY_W", INT2NUM(GLFW_KEY_W));
    rb_define_const(rb_mInput, "KEY_X", INT2NUM(GLFW_KEY_X));
    rb_define_const(rb_mInput, "KEY_Y", INT2NUM(GLFW_KEY_Y));
    rb_define_const(rb_mInput, "KEY_Z", INT2NUM(GLFW_KEY_Z));
    rb_define_const(rb_mInput, "KEY_LEFT_BRACKET", INT2NUM(GLFW_KEY_LEFT_BRACKET));
    rb_define_const(rb_mInput, "KEY_BACKSLASH", INT2NUM(GLFW_KEY_BACKSLASH));
    rb_define_const(rb_mInput, "KEY_RIGHT_BRACKET", INT2NUM(GLFW_KEY_RIGHT_BRACKET));
    rb_define_const(rb_mInput, "KEY_GRAVE_ACCENT", INT2NUM(GLFW_KEY_GRAVE_ACCENT));
    rb_define_const(rb_mInput, "KEY_WORLD_1", INT2NUM(GLFW_KEY_WORLD_1));
    rb_define_const(rb_mInput, "KEY_WORLD_2", INT2NUM(GLFW_KEY_WORLD_2));

    /* Function keys */
    rb_define_const(rb_mInput, "KEY_ESCAPE", INT2NUM(GLFW_KEY_ESCAPE));
    rb_define_const(rb_mInput, "KEY_ENTER", INT2NUM(GLFW_KEY_ENTER));
    rb_define_const(rb_mInput, "KEY_TAB", INT2NUM(GLFW_KEY_TAB));
    rb_define_const(rb_mInput, "KEY_BACKSPACE", INT2NUM(GLFW_KEY_BACKSPACE));
    rb_define_const(rb_mInput, "KEY_INSERT", INT2NUM(GLFW_KEY_INSERT));
    rb_define_const(rb_mInput, "KEY_DELETE", INT2NUM(GLFW_KEY_DELETE));
    rb_define_const(rb_mInput, "KEY_RIGHT", INT2NUM(GLFW_KEY_RIGHT));
    rb_define_const(rb_mInput, "KEY_LEFT", INT2NUM(GLFW_KEY_LEFT));
    rb_define_const(rb_mInput, "KEY_DOWN", INT2NUM(GLFW_KEY_DOWN));
    rb_define_const(rb_mInput, "KEY_UP", INT2NUM(GLFW_KEY_UP));
    rb_define_const(rb_mInput, "KEY_PAGE_UP", INT2NUM(GLFW_KEY_PAGE_UP));
    rb_define_const(rb_mInput, "KEY_PAGE_DOWN", INT2NUM(GLFW_KEY_PAGE_DOWN));
    rb_define_const(rb_mInput, "KEY_HOME", INT2NUM(GLFW_KEY_HOME));
    rb_define_const(rb_mInput, "KEY_END", INT2NUM(GLFW_KEY_END));
    rb_define_const(rb_mInput, "KEY_CAPS_LOCK", INT2NUM(GLFW_KEY_CAPS_LOCK));
    rb_define_const(rb_mInput, "KEY_SCROLL_LOCK", INT2NUM(GLFW_KEY_SCROLL_LOCK));
    rb_define_const(rb_mInput, "KEY_NUM_LOCK", INT2NUM(GLFW_KEY_NUM_LOCK));
    rb_define_const(rb_mInput, "KEY_PRINT_SCREEN", INT2NUM(GLFW_KEY_PRINT_SCREEN));
    rb_define_const(rb_mInput, "KEY_PAUSE", INT2NUM(GLFW_KEY_PAUSE));
    rb_define_const(rb_mInput, "KEY_F1", INT2NUM(GLFW_KEY_F1));
    rb_define_const(rb_mInput, "KEY_F2", INT2NUM(GLFW_KEY_F2));
    rb_define_const(rb_mInput, "KEY_F3", INT2NUM(GLFW_KEY_F3));
    rb_define_const(rb_mInput, "KEY_F4", INT2NUM(GLFW_KEY_F4));
    rb_define_const(rb_mInput, "KEY_F5", INT2NUM(GLFW_KEY_F5));
    rb_define_const(rb_mInput, "KEY_F6", INT2NUM(GLFW_KEY_F6));
    rb_define_const(rb_mInput, "KEY_F7", INT2NUM(GLFW_KEY_F7));
    rb_define_const(rb_mInput, "KEY_F8", INT2NUM(GLFW_KEY_F8));
    rb_define_const(rb_mInput, "KEY_F9", INT2NUM(GLFW_KEY_F9));
    rb_define_const(rb_mInput, "KEY_F10", INT2NUM(GLFW_KEY_F10));
    rb_define_const(rb_mInput, "KEY_F11", INT2NUM(GLFW_KEY_F11));
    rb_define_const(rb_mInput, "KEY_F12", INT2NUM(GLFW_KEY_F12));
    rb_define_const(rb_mInput, "KEY_F13", INT2NUM(GLFW_KEY_F13));
    rb_define_const(rb_mInput, "KEY_F14", INT2NUM(GLFW_KEY_F14));
    rb_define_const(rb_mInput, "KEY_F15", INT2NUM(GLFW_KEY_F15));
    rb_define_const(rb_mInput, "KEY_F16", INT2NUM(GLFW_KEY_F16));
    rb_define_const(rb_mInput, "KEY_F17", INT2NUM(GLFW_KEY_F17));
    rb_define_const(rb_mInput, "KEY_F18", INT2NUM(GLFW_KEY_F18));
    rb_define_const(rb_mInput, "KEY_F19", INT2NUM(GLFW_KEY_F19));
    rb_define_const(rb_mInput, "KEY_F20", INT2NUM(GLFW_KEY_F20));
    rb_define_const(rb_mInput, "KEY_F21", INT2NUM(GLFW_KEY_F21));
    rb_define_const(rb_mInput, "KEY_F22", INT2NUM(GLFW_KEY_F22));
    rb_define_const(rb_mInput, "KEY_F23", INT2NUM(GLFW_KEY_F23));
    rb_define_const(rb_mInput, "KEY_F24", INT2NUM(GLFW_KEY_F24));
    rb_define_const(rb_mInput, "KEY_F25", INT2NUM(GLFW_KEY_F25));
    rb_define_const(rb_mInput, "KEY_KP_0", INT2NUM(GLFW_KEY_KP_0));
    rb_define_const(rb_mInput, "KEY_KP_1", INT2NUM(GLFW_KEY_KP_1));
    rb_define_const(rb_mInput, "KEY_KP_2", INT2NUM(GLFW_KEY_KP_2));
    rb_define_const(rb_mInput, "KEY_KP_3", INT2NUM(GLFW_KEY_KP_3));
    rb_define_const(rb_mInput, "KEY_KP_4", INT2NUM(GLFW_KEY_KP_4));
    rb_define_const(rb_mInput, "KEY_KP_5", INT2NUM(GLFW_KEY_KP_5));
    rb_define_const(rb_mInput, "KEY_KP_6", INT2NUM(GLFW_KEY_KP_6));
    rb_define_const(rb_mInput, "KEY_KP_7", INT2NUM(GLFW_KEY_KP_7));
    rb_define_const(rb_mInput, "KEY_KP_8", INT2NUM(GLFW_KEY_KP_8));
    rb_define_const(rb_mInput, "KEY_KP_9", INT2NUM(GLFW_KEY_KP_9));
    rb_define_const(rb_mInput, "KEY_KP_DECIMAL", INT2NUM(GLFW_KEY_KP_DECIMAL));
    rb_define_const(rb_mInput, "KEY_KP_DIVIDE", INT2NUM(GLFW_KEY_KP_DIVIDE));
    rb_define_const(rb_mInput, "KEY_KP_MULTIPLY", INT2NUM(GLFW_KEY_KP_MULTIPLY));
    rb_define_const(rb_mInput, "KEY_KP_SUBTRACT", INT2NUM(GLFW_KEY_KP_SUBTRACT));
    rb_define_const(rb_mInput, "KEY_KP_ADD", INT2NUM(GLFW_KEY_KP_ADD));
    rb_define_const(rb_mInput, "KEY_KP_ENTER", INT2NUM(GLFW_KEY_KP_ENTER));
    rb_define_const(rb_mInput, "KEY_KP_EQUAL", INT2NUM(GLFW_KEY_KP_EQUAL));
    rb_define_const(rb_mInput, "KEY_LEFT_SHIFT", INT2NUM(GLFW_KEY_LEFT_SHIFT));
    rb_define_const(rb_mInput, "KEY_LEFT_CONTROL", INT2NUM(GLFW_KEY_LEFT_CONTROL));
    rb_define_const(rb_mInput, "KEY_LEFT_ALT", INT2NUM(GLFW_KEY_LEFT_ALT));
    rb_define_const(rb_mInput, "KEY_LEFT_SUPER", INT2NUM(GLFW_KEY_LEFT_SUPER));
    rb_define_const(rb_mInput, "KEY_RIGHT_SHIFT", INT2NUM(GLFW_KEY_RIGHT_SHIFT));
    rb_define_const(rb_mInput, "KEY_RIGHT_CONTROL", INT2NUM(GLFW_KEY_RIGHT_CONTROL));
    rb_define_const(rb_mInput, "KEY_RIGHT_ALT", INT2NUM(GLFW_KEY_RIGHT_ALT));
    rb_define_const(rb_mInput, "KEY_RIGHT_SUPER", INT2NUM(GLFW_KEY_RIGHT_SUPER));
    rb_define_const(rb_mInput, "KEY_MENU", INT2NUM(GLFW_KEY_MENU));

    rb_define_const(rb_mInput, "MOUSE_BUTTON_1", INT2NUM(GLFW_MOUSE_BUTTON_1));
    rb_define_const(rb_mInput, "MOUSE_BUTTON_2", INT2NUM(GLFW_MOUSE_BUTTON_2));
    rb_define_const(rb_mInput, "MOUSE_BUTTON_3", INT2NUM(GLFW_MOUSE_BUTTON_3));
    rb_define_const(rb_mInput, "MOUSE_BUTTON_4", INT2NUM(GLFW_MOUSE_BUTTON_4));
    rb_define_const(rb_mInput, "MOUSE_BUTTON_5", INT2NUM(GLFW_MOUSE_BUTTON_5));
    rb_define_const(rb_mInput, "MOUSE_BUTTON_6", INT2NUM(GLFW_MOUSE_BUTTON_6));
    rb_define_const(rb_mInput, "MOUSE_BUTTON_7", INT2NUM(GLFW_MOUSE_BUTTON_7));
    rb_define_const(rb_mInput, "MOUSE_BUTTON_8", INT2NUM(GLFW_MOUSE_BUTTON_8));
    rb_define_const(rb_mInput, "MOUSE_BUTTON_LEFT", INT2NUM(GLFW_MOUSE_BUTTON_LEFT));
    rb_define_const(rb_mInput, "MOUSE_BUTTON_RIGHT", INT2NUM(GLFW_MOUSE_BUTTON_RIGHT));
    rb_define_const(rb_mInput, "MOUSE_BUTTON_MIDDLE", INT2NUM(GLFW_MOUSE_BUTTON_MIDDLE));
}