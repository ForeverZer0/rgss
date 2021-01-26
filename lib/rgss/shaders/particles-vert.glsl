#version 330 core

layout(location = 0) in vec4 vertex;
layout(location = 1) in vec4 quad;
layout(location = 2) in vec4 rgba;
layout(location = 3) in float angle;

layout (std140) uniform RGSS
{
    mat4 projection;
};

out vec4 vertex_color;
out vec2 uv;

void main()
{    
    float c = cos(angle);
    float s = sin(angle);
    vec2 p = quad.xy + (quad.zw * 0.5);

    // mat4 scale = mat4
    // (
    //     quad.z, 0, 0, 0,
    //     0, quad.w, 0, 0,
    //     0, 0, 1, 0,
    //     quad.x, quad.y, 0, 1
    // );
    // mat4 rotation = mat4
    // (   
    //     c, s, 0, 0,
    //     -s, c, 0, 0,
    //     0, 0, 1, 0,
    //     p.x * (1.0 - c) + p.y * s,  p.y * (1.0 - c) - p.x * s, 0, 1
    // );

    // This is the same as creating a rotation (on a pivot) and model matrix and multiplying them.
    // It has been reduced into a single operation to reduce the number of operations that need
    // performed at the cost of readability.
    mat4 model = mat4
    (
        quad.z * c, quad.z * s, 0, 0,
        quad.w * -s, quad.w * c, 0, 0,
        0, 0, 1, 0,

        (quad.x * c) + (quad.y * -s) + (1 * (p.x * (1.0 - c) + p.y * s)),
        (quad.x * s) + (quad.y *  c) + (1 * (p.y * (1.0 - c) - p.x * s)),
        0,
        1
    );

    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
    uv = vertex.zw;    
    vertex_color = rgba;
}