//
// Created by sava on 10/16/19.
//

#ifndef __TEXTURE_MANAGER_HPP_
#define __TEXTURE_MANAGER_HPP_

#include <memory>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <ogl_2d_tex.hpp>

namespace asset
{
    class texture_manager
    {
    public:
        ogllib::ogl_2d_tex &load_texture(std::string filename);

    private:
        std::unordered_map<std::string, ogllib::ogl_2d_tex> _texture_lookup;
    };

}


#endif //__TEXTURE_MANAGER_HPP_
