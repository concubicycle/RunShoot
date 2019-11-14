//
// Created by sava on 11/14/19.
//

#include <cubemap.hpp>
#include <ogl_2d_tex.h>
#include <stb_image.h>
#include <iostream>

const ogllib::vertex_p ogllib::cubemap::vertices[] = {
    // positions
    { -1.0f,  1.0f, -1.0f},
    { -1.0f, -1.0f, -1.0f},
    { 1.0f, -1.0f, -1.0f},
    { 1.0f, -1.0f, -1.0f},
    { 1.0f,  1.0f, -1.0f},
    { -1.0f,  1.0f, -1.0f},

    { -1.0f, -1.0f,  1.0f},
    { -1.0f, -1.0f, -1.0f},
    { -1.0f,  1.0f, -1.0f},
    { -1.0f,  1.0f, -1.0f},
    { -1.0f,  1.0f,  1.0f},
    { -1.0f, -1.0f,  1.0f},

    { 1.0f, -1.0f, -1.0f},
    { 1.0f, -1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f, -1.0f},
    { 1.0f, -1.0f, -1.0f},

    { -1.0f, -1.0f,  1.0f},
    { -1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f, -1.0f,  1.0f},
    { -1.0f, -1.0f,  1.0f},

    { -1.0f,  1.0f, -1.0f},
    { 1.0f,  1.0f, -1.0f},
    { 1.0f,  1.0f,  1.0f},
    { 1.0f,  1.0f,  1.0f},
    { -1.0f,  1.0f,  1.0f},
    { -1.0f,  1.0f, -1.0f},

    { -1.0f, -1.0f, -1.0f},
    { -1.0f, -1.0f,  1.0f},
    { 1.0f, -1.0f, -1.0f},
    { 1.0f, -1.0f, -1.0f},
    { -1.0f, -1.0f,  1.0f},
    { 1.0f, -1.0f,  1.0f}
};

ogllib::cubemap::cubemap(
    std::string left,
    std::string top,
    std::string front,
    std::string bottom,
    std::string right,
    std::string back)
{
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);


    std::vector<std::string> faces
    {
        right,
        left,
        top,
        bottom,
        front,
        back
    };

    _texture_id = load_cubemap(faces);


}

unsigned int ogllib::cubemap::load_cubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            auto format = nrChannels == 3 ? GL_RGB : GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            spdlog::error("Cubemap texture failed to load at path: {0}", faces[i]);
        }

        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void ogllib::cubemap::bind()
{
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _texture_id);
}

void ogllib::cubemap::unbind()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
