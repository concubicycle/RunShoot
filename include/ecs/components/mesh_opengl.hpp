//
// Created by sava on 10/24/19.
//

#ifndef __MESH_OPENGL_HPP_
#define __MESH_OPENGL_HPP_

#include <cstdint>


namespace ecs
{

    struct mesh_opengl
    {
        std::uint32_t vao;
        std::uint32_t vbo; // probably don't need
        std::uint32_t ebo; // probably don't need
        std::uint32_t diffuse_texture_id;
        std::uint32_t element_count;
    };

}

#endif //__MESH_OPENGL_HPP_
