#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D model_texture;

void main()
{
    FragColor = texture(model_texture, TexCoord);
}