#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 model, projection, view;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1);
}
