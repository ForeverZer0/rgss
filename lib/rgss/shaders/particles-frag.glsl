#version 330 core

// Interpolated values from the vertex shaders
in vec2 uv;
in vec4 vertex_color;

// Ouput data
out vec4 color;

uniform sampler2D myTextureSampler;

void main(){
	// Output color = color of the texture at the specified UV
	color = texture( myTextureSampler, uv ) * vertex_color;

}