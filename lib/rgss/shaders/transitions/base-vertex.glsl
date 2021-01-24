#version 330 core

in vec2 _p;
out vec2 _uv;

layout (std140) uniform RGSS
{
    mat4 projection;
};

void main() {
    gl_Position = projection * vec4(_p.xy, 0.0, 1.0);
    _uv = vec2(0.5, 0.5) * (_p + vec2(1.0, 1.0));
}