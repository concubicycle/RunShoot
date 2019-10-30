//
// Created by sava on 10/16/19.
//

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "asset/texture_manager.hpp"

#include <functional>
#include <string>


asset::texture_manager::texture_manager(string_table &str_table) : _filename_lookup(str_table)
{

}


//pass this to the texture shared_ptr, and see where it gets hit if debugging
//texture de-allocation is needed.
//void wrap_stbi_image_free(void *retval_from_stbi_load)
//{
//	stbi_image_free(retval_from_stbi_load);
//}

ogllib::ogl_2d_tex &asset::texture_manager::load_texture(std::string filename, size_t hash)
{
    auto it = _texture_lookup.find(hash);
    if (it != _texture_lookup.end()) return it->second;

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

    auto ptr =  std::shared_ptr<std::uint8_t>(data, stbi_image_free);

    _texture_lookup.emplace(std::piecewise_construct,
        std::forward_as_tuple(hash),
        std::forward_as_tuple(ptr, width, height, bytes_per_texel));

    return _texture_lookup.find(hash)->second;
}

ogllib::ogl_2d_tex &asset::texture_manager::load_texture(size_t hash)
{
    return load_texture(_filename_lookup[hash], hash);
}

ogllib::ogl_2d_tex &asset::texture_manager::load_texture(std::string filename)
{
    auto hash = _filename_lookup.hash_and_store(filename);
    return load_texture(filename, hash);
}


void asset::texture_manager::unload_texture(std::string filename)
{
    auto hash = _filename_lookup.hash_and_store(filename);

	auto it = _texture_lookup.find(hash);
	if (it == _texture_lookup.end()) return;

	it->second.unload_tex();	
}

void asset::texture_manager::unload_all()
{
	for (auto& pair : _texture_lookup)
	{
		pair.second.unload_tex();
	}
}
