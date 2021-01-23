#ifndef RGSS_GRAPHICS_H
#define RGSS_GRAPHICS_H 1

#include "glad.h"
#include "rgss.h"

typedef struct RGSS_Entity RGSS_Entity;
typedef struct RGSS_Renderable RGSS_Renderable;

#define RGSS_GRAPHICS RGSS_GAME.graphics

/**
 * @brief A unit vector for rotation around Z axis.
 */
static vec3 RGSS_AXIS_Z = {0.0f, 0.0f, 1.0f};

/**
 * @brief A set of vertices for drawing a textured quad with two triangles using an element buffer.
 * @see RGSS_QUAD_INDICES
 */
static const GLfloat RGSS_QUAD_VERTICES[] = 
{
    0.0f, 1.0f, 0.0f, 1.0f, // Bottom-Left
    1.0f, 0.0f, 1.0f, 0.0f, // Top-Right
    0.0f, 0.0f, 0.0f, 0.0f, // Top-Left
    1.0f, 1.0f, 1.0f, 1.0f, // Bottom-Right
};

/**
 * @brief A set of indices for drawing a textured quad.
 * @see RGSS_QUAD_VERTICES
 */
static const GLubyte RGSS_QUAD_INDICES[] =
{
    0, 1, 2, // First Triangle 
    0, 3, 1  // Second Triangle
};

/**
 * @brief Asserts the specified program ID is valid, raising a Ruby exception if not.
 * @param[in] id The name of an OpenGL shader program.
 */
#define RGSS_ASSERT_SHADER(id)                                                                                         \
    if (id == GL_NONE)                                                                                                 \
    rb_raise(rb_eRGSSError, "disposed shader program")

/**
 * @brief Stores an OpenGL program ID into a pointer.
 * @param[in] id The name of an OpenGL shader program.
 * @return A pointer containing the ID value.
 */
#define RGSS_SHADER_PTR(id) ((GLuint *)(size_t)id)

/**
 * @brief Wraps an OpenGL program ID into a Ruby object of the specifed class.
 * @param[in] klass The Ruby class to wrap it as.
 * @param[in] id The name of an OpenGL shader program.
 * @return A Ruby VALUE representing the object.
 */
#define RGSS_SHADER_WRAP_CLASS(klass, id) Data_Wrap_Struct(klass, NULL, RUBY_NEVER_FREE, RGSS_SHADER_PTR(id))

/**
 * @brief Wraps an OpenGL program ID into a Ruby object, using base class.
 * @param[in] id The name of an OpenGL shader program.
 * @return A Ruby VALUE representing the object.
 */
#define RGSS_SHADER_WRAP(id) RGSS_SAHDER_WRAP_CLASS(rb_cShader, id)

/**
 * @brief Unwraps a Ruby VALUE object back into an OpenGL shader program.
 * @param[in] v A Ruby VALUE representing the shader.
 * @return The shader ID.
 */
#define RGSS_SHADER_UNWRAP(v) ((GLuint)(size_t)DATA_PTR(v))

/**
 * @brief
 * Creates a shader unit with specified GLSL source code, raising a Ruby exception
 * with detailed log on failure.
 * @param[in] source The GLSL source code of the shader.
 * @param[in] type Either @c GL_VERTEX_SHADER, @c GL_FRAGMENT_SHADER, @c GL_GEOMETRY_SHADER.
 * @return The OpenGL shader name.
 */
GLuint RGSS_CreateShader(const char *source, GLenum type);

/**
 * @brief
 * Creates a shader unit with the GLSL source code in the specified file,
 * raising a Ruby exception with detailed log on failure.
 * @param[in] filename A path to the file to read from.
 * @param[in] type Either @c GL_VERTEX_SHADER, @c GL_FRAGMENT_SHADER, @c GL_GEOMETRY_SHADER.
 * @return The OpenGL shader name.
 */
GLuint RGSS_CreateShaderFromFile(const char *filename, GLenum type);

/**
 * @brief Creates a complete OpenGL shader program from given shader units, raising a Ruby exception
 * with detailed log on failure.
 * @param[in] vertex A valid name of an OpenGL vertex shader.
 * @param[in] fragment A valid name of an OpenGL fragment shader.
 * @param[in] geometry A valid name of an OpenGL geometry shader, or @c GL_NONE to not use this unit.
 * @return The OpenGL program name.
 */
GLuint RGSS_CreateProgram(GLuint vertex, GLuint fragment, GLuint geometry);

/**
 * @brief Creates a complete OpenGL shader program from given shader source code, raising a Ruby exception
 * with detailed log on failure.
 * @param[in] vert_src The GLSL source code for the vertex shader.
 * @param[in] frag_src The GLSL source code for the fragment shader.
 * @param[in] geom_src The GLSL source code for the geometry shader, or @c NULL to not use this unit.
 * @return The OpenGL program name.
 */
GLuint RGSS_CreateProgramFromSource(const char *vert_src, const char *frag_src, const char *geom_src);

/**
 * @brief Creates a complete OpenGL shader program from given shader source code files, raising a Ruby exception
 * with detailed log on failure.
 * @param[in] vert_path The path to a file containg the GLSL source code for the vertex shader.
 * @param[in] frag_path The path to a file containg the GLSL source code for the fragment shader.
 * @param[in] geom_path The path to a file containg the GLSL source code for the geometry shader, or @c NULL to not use
 * this unit.
 * @return The OpenGL program name.
 */
GLuint RGSS_CreateProgramFromFile(const char *vert_path, const char *frag_path, const char *geom_path);

/**
 * @brief Initiializes the base enitity object for structures that utilize it.
 * 
 * @param[out] entity A pointer to the structure to initialize.
 * @note The RGSS_Entity structure must always be the first field in a structure.
 */
void RGSS_Entity_Init(RGSS_Entity *entity);

/**
 * @brief Cleans up resources of a entity structure.
 * 
 * @param[in] entity A pointer to the structure to finalize.
 */
void RGSS_Entity_Deinit(RGSS_Entity *entity);


void RGSS_Renderable_Init(RGSS_Renderable *obj);


static inline void RGSS_ValueToVec3(VALUE value, vec3 result)
{
    if (!RTEST(value))
    {
        glm_vec3_zero(result);
        return;
    }

    result[2] = 0.0f;
    if (rb_obj_is_kind_of(value, rb_cIVec2) == Qtrue)
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

VALUE RGSS_Entity_GetX(VALUE self);
VALUE RGSS_Entity_SetX(VALUE self, VALUE value);
VALUE RGSS_Entity_GetY(VALUE self);
VALUE RGSS_Entity_SetY(VALUE self, VALUE value);
VALUE RGSS_Entity_GetZ(VALUE self);
VALUE RGSS_Entity_SetZ(VALUE self, VALUE value);
VALUE RGSS_Entity_GetLocation(VALUE self);
VALUE RGSS_Entity_SetLocation(VALUE self, VALUE point);
VALUE RGSS_Entity_Update(VALUE self, VALUE delta);
VALUE RGSS_Entity_GetModel(VALUE self);
VALUE RGSS_Entity_SetModel(VALUE self, VALUE model);
VALUE RGSS_Entity_GetPosition(VALUE self);
VALUE RGSS_Entity_SetPosition(VALUE self, VALUE value);
VALUE RGSS_Entity_GetVelocity(VALUE self);
VALUE RGSS_Entity_SetVelocity(VALUE self, VALUE value);
VALUE RGSS_Entity_GetScale(VALUE self);
VALUE RGSS_Entity_SetScale(VALUE self, VALUE value);
VALUE RGSS_Entity_GetPivot(VALUE self);
VALUE RGSS_Entity_SetPivot(VALUE self, VALUE value);
VALUE RGSS_Entity_GetSize(VALUE self);
VALUE RGSS_Entity_SetSize(VALUE self, VALUE value);
VALUE RGSS_Entity_GetAngle(VALUE self);
VALUE RGSS_Entity_SetAngle(VALUE self, VALUE degrees);
VALUE RGSS_Entity_GetBounds(VALUE self);
VALUE RGSS_Entity_Rotate(int argc, VALUE *argv, VALUE self);
VALUE RGSS_Entity_GetWidth(VALUE self);
VALUE RGSS_Entity_GetHeight(VALUE self);
VALUE RGSS_Entity_SetWidth(VALUE self, VALUE value);
VALUE RGSS_Entity_SetHeight(VALUE self, VALUE value);


#endif /* RGSS_GRAPHICS_H */