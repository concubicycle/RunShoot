#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D model_texture;

void main()
{
    vec4 color = texture(model_texture, TexCoord);

    if (color.w == 0) discard;

    FragColor = color;
}