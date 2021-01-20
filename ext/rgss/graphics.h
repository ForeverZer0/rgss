#ifndef RGSS_GRAPHICS_H
#define RGSS_GRAPHICS_H 1

#include "glad.h"
#include "rgss.h"

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

#endif /* RGSS_GRAPHICS_H */