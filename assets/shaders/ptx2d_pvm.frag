#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D model_texture;


uniform vec2 tex_offset = vec2(0, 0);

void main()
{
    vec4 color = texture(model_texture, TexCoord+tex_offset);

    if (color.w == 0) discard;

    FragColor = color;
}
