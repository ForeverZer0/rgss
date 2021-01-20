#include "rgss.h"

VALUE rb_cTable;

typedef struct
{
    int dimensions;
    int width;
    int height;
    int depth;
    int count;
    short *data;
} RGSS_Table;

#define DATA_OFFSET               20
#define INDEX_XY(table, x, y)     ((x) + ((y)*table->width))
#define INDEX_XYZ(table, x, y, z) (INDEX_XY(table, x, y) + ((z)*table->width * table->height))

static void RGSS_Table_Free(void *data)
{
    if (data == NULL)
        return;
    RGSS_Table *table = data;
    if (table->data)
        xfree(table->data);
    xfree(data);
}

static VALUE RGSS_Table_Alloc(VALUE klass)
{
    RGSS_Table *table = ALLOC(RGSS_Table);
    memset(table, 0, sizeof(RGSS_Table));
    return Data_Wrap_Struct(klass, NULL, RGSS_Table_Free, table);
}

static VALUE RGSS_Table_Initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE x, y, z;
    rb_scan_args(argc, argv, "12", &x, &y, &z);

    RGSS_Table *table = DATA_PTR(self);
    switch (argc)
    {
        case 3: {
            table->depth = NUM2INT(z);
            table->height = NUM2INT(y);
            table->width = NUM2INT(x);
            table->count = table->width * table->height * table->depth;
            break;
        }
        case 2: {
            table->height = NUM2INT(y);
            table->width = NUM2INT(x);
            table->count = table->width * table->height;
            break;
        }
        case 1: {
            table->width = NUM2INT(x);
            table->count = table->width;
            break;
        }
    }

    if (table->count < 1)
        rb_raise(rb_eArgError, "dimensions must be greater than 0");

    table->dimensions = argc;
    table->data = xcalloc(sizeof(short), table->count);
    return self;
}

static VALUE RGSS_Table_Get(int argc, VALUE *argv, VALUE self)
{
    RGSS_Table *t = DATA_PTR(self);
    if (argc != t->dimensions)
        rb_raise(rb_eArgError, "wrong number of arguments (given %d, expected %d)", argc, t->dimensions);

    long i;
    switch (argc)
    {
        case 3: {
            int z = NUM2INT(argv[2]);
            if (z < 0 || z >= t->depth)
                rb_raise(rb_eArgError, "z value of of range (given %d, expected 0...%d)", z, t->depth);
            i += (z * t->width * t->height);
            // No break, fall-through to next argument
        }
        case 2: {
            int y = NUM2INT(argv[1]);
            if (y < 0 || y >= t->height)
                rb_raise(rb_eArgError, "y value of of range (given %d, expected 0...%d)", y, t->height);
            i += (y * t->width);
            // No break, fall-through to next argument
        }
        case 1: {
            int x = NUM2INT(argv[0]);
            if (x < 0 || x >= t->width)
                rb_raise(rb_eArgError, "x value of of range (given %d, expected 0...%d)", x, t->width);
            i += x;
            break;
        }
    }

    return INT2NUM(t->data[i]);
}

static VALUE RGSS_Table_Set(int argc, VALUE *argv, VALUE self)
{
    RGSS_Table *t = DATA_PTR(self);
    if (argc != t->dimensions + 1)
        rb_raise(rb_eArgError, "wrong number of arguments (given %d, expected %d)", argc, t->dimensions + 1);

    long i;
    switch (argc)
    {
        case 4: {
            int z = NUM2INT(argv[2]);
            if (z < 0 || z >= t->depth)
                rb_raise(rb_eArgError, "z value of of range (given %d, expected 0...%d)", z, t->depth);
            i += (z * t->width * t->height);
            // No break, fall-through to next argument
        }
        case 3: {
            int y = NUM2INT(argv[1]);
            if (y < 0 || y >= t->height)
                rb_raise(rb_eArgError, "y value of of range (given %d, expected 0...%d)", y, t->height);
            i += (y * t->width);
            // No break, fall-through to next argument
        }
        case 2: {
            int x = NUM2INT(argv[0]);
            if (x < 0 || x >= t->width)
                rb_raise(rb_eArgError, "x value of of range (given %d, expected 0...%d)", x, t->width);
            i += x;
            break;
        }
    }

    t->data[i] = NUM2SHORT(argv[argc - 1]);
    return argv[argc - 1];
}

static VALUE RGSS_Table_GetSizeX(VALUE self)
{
    return INT2NUM(((RGSS_Table *)DATA_PTR(self))->width);
}

static VALUE RGSS_Table_GetSizeY(VALUE self)
{
    return INT2NUM(((RGSS_Table *)DATA_PTR(self))->height);
}

static VALUE RGSS_Table_GetSizeZ(VALUE self)
{
    return INT2NUM(((RGSS_Table *)DATA_PTR(self))->depth);
}

static VALUE RGSS_Table_GetDimensions(VALUE self)
{
    return INT2NUM(((RGSS_Table *)DATA_PTR(self))->dimensions);
}

static VALUE RGSS_Table_GetCount(VALUE self)
{
    return INT2NUM(((RGSS_Table *)DATA_PTR(self))->count);
}

static VALUE RGSS_Table_Resize(int argc, VALUE *argv, VALUE self)
{
    VALUE temp = RGSS_Table_Alloc(rb_cTable);
    RGSS_Table_Initialize(argc, argv, temp);

    RGSS_Table *t1 = DATA_PTR(self), *t2 = DATA_PTR(temp);

    int w = RGSS_MIN(t1->width, t2->width);
    int h = RGSS_MIN(t1->height, t2->height);
    int d = RGSS_MIN(t1->depth, t2->depth);
    int dims = RGSS_MIN(t1->dimensions, t2->dimensions);

    switch (dims)
    {
        case 1: {
            for (int x = 0; x < w; x++)
            {
                t2->data[x] = t1->data[x];
            }
            break;
        }
        case 2: {
            for (int x = 0; x < w; x++)
                for (int y = 0; y < h; y++)
                {
                    t2->data[INDEX_XY(t2, x, y)] = t1->data[INDEX_XY(t1, x, y)];
                }
            break;
        }
        case 3: {
            for (int x = 0; x < w; x++)
                for (int y = 0; y < h; y++)
                    for (int z = 0; z < d; z++)
                    {
                        t2->data[INDEX_XYZ(t2, x, y, z)] = t1->data[INDEX_XYZ(t1, x, y, z)];
                    }
            break;
        }
    }

    // Just swap out the underlying pointer for the new table
    struct RData *rdata = RDATA(self);
    rdata->data = t2;

    // Free the original structure
    xfree(t1->data);
    xfree(t1);
    RDATA(temp)->data = NULL;

    return self;
}

static VALUE RGSS_Table_Each(VALUE self)
{
    RETURN_ENUMERATOR(self, 0, NULL);
    RGSS_Table *t = DATA_PTR(self);
    short value;

    switch (t->dimensions)
    {
        case 1: {
            for (long x = 0; x < t->width; x++)
            {
                value = t->data[x];
                rb_yield(rb_ary_new_from_args(2, INT2NUM(x), INT2NUM(value)));
            }
            break;
        }
        case 2: {
            for (long y = 0; y < t->height; y++)
                for (long x = 0; x < t->width; x++)
                {
                    value = t->data[INDEX_XY(t, x, y)];
                    rb_yield(rb_ary_new_from_args(3, INT2NUM(x), INT2NUM(y), INT2NUM(value)));
                }
            break;
        }
        case 3: {
            for (long z = 0; z < t->depth; z++)
                for (long y = 0; y < t->height; y++)
                    for (long x = 0; x < t->width; x++)
                    {
                        value = t->data[INDEX_XYZ(t, x, y, z)];
                        rb_yield(rb_ary_new_from_args(4, INT2NUM(x), INT2NUM(y), INT2NUM(z), INT2NUM(value)));
                    }
            break;
        }
    }

    return self;
}

static VALUE RGSS_Table_Dump(int argc, VALUE *argv, VALUE self)
{
    RGSS_Table *t = DATA_PTR(self);
    size_t size = 20 + (t->count * sizeof(short));

    char buffer[size];
    memcpy(&buffer[0], t, DATA_OFFSET);
    memcpy(&buffer[DATA_OFFSET], t->data, t->count * sizeof(short));

    return rb_str_new(buffer, size);
}

static VALUE RGSS_Table_Load(VALUE klass, VALUE binary)
{
    char *buffer = StringValuePtr(binary);
    RGSS_Table *t = ALLOC(RGSS_Table);
    memcpy(t, buffer, DATA_OFFSET);

    size_t size = t->count * sizeof(short);
    t->data = xmalloc(size);
    memcpy(t->data, &buffer[DATA_OFFSET], size);

    return Data_Wrap_Struct(klass, NULL, RGSS_Table_Free, t);
}

void RGSS_Init_Table(VALUE parent)
{
    rb_cTable = rb_define_class_under(parent, "Table", rb_cObject);
    rb_define_alloc_func(rb_cTable, RGSS_Table_Alloc);

    rb_define_methodm1(rb_cTable, "initialize", RGSS_Table_Initialize, -1);
    rb_define_methodm1(rb_cTable, "[]", RGSS_Table_Get, -1);
    rb_define_methodm1(rb_cTable, "[]=", RGSS_Table_Set, -1);

    rb_define_method0(rb_cTable, "each", RGSS_Table_Each, 0);
    rb_define_method0(rb_cTable, "x_size", RGSS_Table_GetSizeX, 0);
    rb_define_method0(rb_cTable, "y_size", RGSS_Table_GetSizeY, 0);
    rb_define_method0(rb_cTable, "z_size", RGSS_Table_GetSizeZ, 0);
    rb_define_method0(rb_cTable, "dimensions", RGSS_Table_GetDimensions, 0);
    rb_define_method0(rb_cTable, "count", RGSS_Table_GetCount, 0);

    rb_define_methodm1(rb_cTable, "_dump", RGSS_Table_Dump, -1);
    rb_define_singleton_method1(rb_cTable, "_load", RGSS_Table_Load, 1);

    rb_define_alias(rb_cTable, "width", "x_size");
    rb_define_alias(rb_cTable, "height", "y_size");
    rb_define_alias(rb_cTable, "depth", "z_size");
    rb_define_alias(rb_cTable, "size", "count");
    rb_define_alias(rb_cTable, "length", "count");
}