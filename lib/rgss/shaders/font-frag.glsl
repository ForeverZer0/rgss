#version 330 core

in vec2 uv;
out vec4 result;

uniform sampler2D text;
uniform vec4 color;

void main()
{    
    result = color * vec4(1.0, 1.0, 1.0, texture(text, uv).r);
}