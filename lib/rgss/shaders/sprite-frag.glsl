#version 330 core

in vec2 uv;
out vec4 result;

uniform sampler2D image;
uniform vec4 color;
uniform vec4 tone;
uniform vec4 flash;
uniform float opacity = 1.0;
uniform float hue;

const vec3 k = vec3(0.57735, 0.57735, 0.57735);

void main() {

    // Get the fragment from bound texture
    result = texture(image, uv);

    // Applu hue shift
    float angle = cos(radians(hue)); // Source is already in radians
    vec3 rgb = vec3(result.rgb * angle + cross(k, result.rgb) * sin(radians(hue)) + k * dot(k, result.rgb) * (1.0 - angle));
    result = vec4(rgb, result.a);

    // Apply color blending
    result = vec4(mix(result.rgb, color.rgb, color.a), result.a);

    // Apply tone blending
    float avg = (result.r + result.g + result.b) / 3.0;
    result.r  = result.r - ((result.r - avg) * tone.a);
    result.g  = result.g - ((result.g - avg) * tone.a);
    result.b  = result.b - ((result.b - avg) * tone.a);
    result = vec4(clamp(result.rgb + tone.rgb, 0.0, 1.0), result.a);

    // Flash effect color blending
    result = vec4(mix(result.rgb, flash.rgb, flash.a), result.a);

    // Apply opacity
    result *= opacity;
}