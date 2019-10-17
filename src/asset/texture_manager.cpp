//
// Created by sava on 10/16/19.
//

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "asset/texture_manager.hpp"



ogllib::ogl_2d_tex &asset::texture_manager::load_texture(std::string filename)
{
    const char *str = filename.c_str();
    int width;
    int height;
    int bytes_per_texel;

    unsigned char *data = stbi_load(
        str,
        &width,
        &height,
        &bytes_per_texel,
        0);

    unsigned int size = width * height;

    auto ptr =  std::shared_ptr<std::uint8_t>(data,
        [](std::uint8_t *dataPtr) -> void
        {
            stbi_image_free(dataPtr);
        });

    _texture_lookup.emplace(std::piecewise_construct,
        std::forward_as_tuple(filename),
        std::forward_as_tuple(ptr, width, height, bytes_per_texel));

    return _texture_lookup.find(filename)->second;
}
