
#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

uniform float color_multiplier;


void main()
{
    vec4 color = texture(skybox, TexCoords);

    if (color_multiplier > 3)
    {
        color = vec4(1.f);
    }
    vec4 color_multiplied = vec4((color * color_multiplier).xyz, color.w);
    FragColor = color_multiplier > 3 ? vec4(1, 1, 1, 1) : color_multiplied;
}