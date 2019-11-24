#version 330 core
out vec4 FragColor;


in VS_OUT {
    vec4 rgba;
} fs_in;


void main()
{
    FragColor = fs_in.rgba;
}