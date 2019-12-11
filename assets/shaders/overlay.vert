#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 rgba;

out VS_OUT {
    vec4 rgba;
} vs_out;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    vs_out.rgba = rgba;
    vec4 p = projection * model * vec4(position, 1);
    p.z = 0.75;

    gl_Position = p;
}

