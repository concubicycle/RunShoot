
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

uniform float color_multiplier;


void main()
{
    vec4 color = color_multiplier > 10 ? vec4(1) : texture(skybox, TexCoords);
    color = vec4(color.xyz * color_multiplier, 1);
    FragColor = color;
}