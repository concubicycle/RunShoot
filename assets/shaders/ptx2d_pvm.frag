#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D model_texture;


uniform vec2 tex_offset = vec2(0, 0);

// color multiplier for flash effect
uniform float color_multiplier = 1;

void main()
{
    vec4 color = texture(model_texture, TexCoord+tex_offset);

    if (color.w == 0) discard;

    vec4 color_multiplied = vec4((color * color_multiplier).xyz, color.w);

    FragColor = color_multiplied;
}
