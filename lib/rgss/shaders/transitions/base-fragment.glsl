#version 330 core

in vec2 _uv;
out vec4 result;

uniform sampler2D from, to;
uniform float progress, ratio;

vec4 getFromColor(vec2 uv) {
    return texture2D(from, uv);
}

vec4 getToColor(vec2 uv) {
    return texture2D(to, uv);
}