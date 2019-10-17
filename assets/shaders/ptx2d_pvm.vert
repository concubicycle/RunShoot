#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoords_2d;

uniform mat4 projection_view_model;

out vec2 TexCoord;

void main()
{
    gl_Position = projection_view_model * vec4(position.x, position.y, position.z, 1.0);
    TexCoord = texcoords_2d;
}