#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D model_texture;
uniform vec3 hue;

void main()
{
    vec4 Kdif = texture(model_texture, TexCoord);


    FragColor = (Kdif + vec4(hue, Kdif.a)) / 2.f;
}