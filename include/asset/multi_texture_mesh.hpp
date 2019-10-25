//
// Created by sava on 10/23/19.
//

#ifndef __MULTI_TEXTURE_MESH_HPP_
#define __MULTI_TEXTURE_MESH_HPP_

#include <string>
#include "mesh.hpp"





namespace models
{
    struct texture
    {
        std::uint32_t _id;
        std::string type;
        std::string path;
    };

    template<class TVertex>
    class multi_texture_mesh
    {
    public:
        mesh<TVertex> mesh_data;
        std::vector<texture> textures;
    };


}
#endif //__MULTI_TEXTURE_MESH_HPP_
