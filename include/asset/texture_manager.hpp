//
// Created by sava on 10/16/19.
//

#ifndef __TEXTURE_MANAGER_HPP_
#define __TEXTURE_MANAGER_HPP_

#include <memory>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <ogl_2d_tex.h>
#include <util/string_table.hpp>

namespace asset
{
    class texture_manager
    {
    public:
        explicit texture_manager(string_table& str_table);

        ogllib::ogl_2d_tex &load_texture(std::string filename, size_t hash);
        ogllib::ogl_2d_tex &load_texture(size_t hash);
        ogllib::ogl_2d_tex &load_texture(std::string filename);

		void unload_texture(std::string filename);
		void unload_all();

    private:
        std::unordered_map<size_t, ogllib::ogl_2d_tex> _texture_lookup;
        string_table& _filename_lookup;
    };
}


#endif //__TEXTURE_MANAGER_HPP_
