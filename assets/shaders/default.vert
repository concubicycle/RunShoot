#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords_2d;

out VS_OUT {
    vec3 normal;
    vec2 texcoords_2d;
    vec3 light_vec;
    vec3 view_vec;
} vs_out;

uniform mat4 model, model_inverse, projection, view;
uniform vec3 light_pos;
uniform vec3 view_pos;

void main()
{
    vec4 world_pos = model * vec4(position, 1);

    vs_out.light_vec = light_pos - world_pos.xyz;
    vs_out.view_vec = view_pos - world_pos.xyz;

    vs_out.normal = mat3(model_inverse) * normal;
    vs_out.texcoords_2d = texcoords_2d;

    gl_Position = projection * view * world_pos;
}

