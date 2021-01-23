#version 330 core

in layout(location = 0) vec4 vertex;

out vec2 uv;

layout (std140) uniform RGSS
{
    mat4 projection;
};

uniform mat4 model;

void main() {
    uv = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}