#version 330 core

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec4 quad; // Position of the center of the particule and size of the square
layout(location = 2) in vec4 rgba; // Position of the center of the particule and size of the square

layout (std140) uniform ortho
{
    mat4 projection;
};

out vec4 vertex_color;
out vec2 uv;

void main()
{
    
    mat4 model = mat4
    (
        quad.z, 0, 0, 0,
        0, quad.w, 0, 0,
        0, 0, 1, 0,
        quad.x, quad.y, 0, 1
    );

    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);

    uv = vertex.zw;
    vertex_color = rgba;
}