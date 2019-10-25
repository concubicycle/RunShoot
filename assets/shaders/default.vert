#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords_2d;

// declare an interface block; see 'Advanced GLSL' for what these are.
out VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 texcoords_2d;
} vs_out;

uniform mat4 model, model_inverse, projection, view;

void main()
{
    vs_out.position = (projection * view * model * vec4(position,1)).xyz;
    vs_out.normal = mat3(model_inverse) * normal;
    vs_out.texcoords_2d = texcoords_2d;

    gl_Position = projection * view * vec4(position, 1.0);
}

