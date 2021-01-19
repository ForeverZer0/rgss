#include "game.h"

VALUE rb_cBatch;

int RGSS_Batch_Sort(const void *obj1, const void* obj2)
{
    RGSS_Renderable *a = DATA_PTR(*(VALUE*)obj1);
    RGSS_Renderable *b = DATA_PTR(*(VALUE*)obj2);

    if (a->entity.depth < b->entity.depth)
        return -1;
    return a->entity.depth > b->entity.depth;
}

static void RGSS_Batch_Mark(void * data)
{
    RGSS_Batch *batch = data;

    VALUE item;
    int i;

    vec_foreach(&batch->items, item, i)
    {
        rb_gc_mark(item);
    }
}  

static void RGSS_Batch_Free(void * data)
{
    RGSS_Batch *batch = data;
    vec_deinit(&batch->items);
    xfree(data);
}

static VALUE RGSS_Batch_Alloc(VALUE klass)
{
    RGSS_Batch *batch = ALLOC(RGSS_Batch);
    memset(batch, 0, sizeof(RGSS_Batch));
    vec_init(&batch->items);
    return Data_Wrap_Struct(klass, RGSS_Batch_Mark, RGSS_Batch_Free, batch);
}

static VALUE RGSS_Batch_Each(VALUE self)
{
    RETURN_ENUMERATOR(self, 0, NULL);

    RGSS_Batch *batch = DATA_PTR(self);
    VALUE item;
    int i;

    if (batch->invalid)
    {
        vec_sort(&batch->items, RGSS_Batch_Sort);
        batch->invalid = false;
    }

    vec_foreach(&batch->items, item, i)
    {
        rb_yield(item);
    }
    return self;
}

static VALUE RGSS_Batch_GetSize(VALUE self)
{
    RGSS_Batch *batch = DATA_PTR(self);
    return INT2NUM(batch->items.length);
}

static inline void RGSS_Batch_AddItem(RGSS_Batch *batch, VALUE item)
{
    if (rb_obj_is_kind_of(item, rb_cRenderable) != Qtrue)
        rb_raise(rb_eTypeError, "%s is not a Renderable", CLASS_NAME(item));

    int i;
    vec_find(&batch->items, item, i);
    if (i != -1)
        rb_raise(rb_eArgError, "duplicate item added to batch");

    vec_push(&batch->items, item);
    batch->invalid = true;
}

VALUE RGSS_Batch_Add(VALUE self, VALUE obj)
{
    RGSS_Batch *batch = DATA_PTR(self);
    if (RB_TYPE_P(obj, T_ARRAY))
    {
        long len = rb_array_len(obj);
        for (long i = 0; i < len; i++)
            RGSS_Batch_AddItem(batch, rb_ary_entry(obj, i));
    }
    else
    {   
        RGSS_Batch_AddItem(batch, obj);
    }
    return obj;
}

VALUE RGSS_Batch_Remove(VALUE self, VALUE obj)
{
    RGSS_Batch *batch = DATA_PTR(self);
    vec_remove(&batch->items, obj);
    return obj;
}

static VALUE RGSS_Batch_GetItem(VALUE self, VALUE index)
{
    RGSS_Batch *batch = DATA_PTR(self);
    int i = NUM2INT(index);

    if (i < 0 || i >= batch->items.length)
        return Qnil;

    if (batch->invalid)
    {
        vec_sort(&batch->items, RGSS_Batch_Sort);
        batch->invalid = false;
    }

    return batch->items.data[i];
}

VALUE RGSS_Batch_Invalidate(VALUE self)
{
    RGSS_Batch *batch = DATA_PTR(self);
    batch->invalid = true;
    return Qnil;
}

static VALUE RGSS_Batch_IsInvalid(VALUE self)
{
    RGSS_Batch *batch = DATA_PTR(self);
    return RB_BOOL(batch->invalid);
}

static VALUE RGSS_Batch_Contains(VALUE self, VALUE obj)
{
    if (rb_obj_is_kind_of(obj, rb_cRenderable) == Qfalse)
        return Qfalse;

    RGSS_Batch *batch = DATA_PTR(self);
    int i;
    vec_find(&batch->items, obj, i);
    return RB_BOOL(i != -1);
}

void RGSS_Init_Batch(VALUE parent)
{
    rb_cBatch = rb_define_class_under(parent, "Batch", rb_cObject);
    rb_define_alloc_func(rb_cBatch, RGSS_Batch_Alloc);
    rb_include_module(rb_cBatch, rb_mEnumerable);

    rb_define_method0(rb_cBatch, "each", RGSS_Batch_Each, 0);
    rb_define_method0(rb_cBatch, "size", RGSS_Batch_GetSize, 0);
    rb_define_method1(rb_cBatch, "[]", RGSS_Batch_GetItem, 1);
    rb_define_method0(rb_cBatch, "invalidate", RGSS_Batch_Invalidate, 0);
    rb_define_method0(rb_cBatch, "invalid?", RGSS_Batch_IsInvalid, 0);
    rb_define_method1(rb_cBatch, "add", RGSS_Batch_Add, 1);
    rb_define_method1(rb_cBatch, "remove", RGSS_Batch_Remove, 1);
    rb_define_method1(rb_cBatch, "include?", RGSS_Batch_Contains, 1);

    rb_define_alias(rb_cBatch, "item", "[]");
    rb_define_alias(rb_cBatch, "length", "size");
    rb_define_alias(rb_cBatch, "push", "add");
    rb_define_alias(rb_cBatch, "<<", "add");
    rb_define_alias(rb_cBatch, "delete", "remove");
    rb_define_alias(rb_cBatch, "contains?", "include?");
}   