//
// Created by sava on 10/24/19.
//

#ifndef __ASSET_TYPES_HPP_
#define __ASSET_TYPES_HPP_

#include <vertex_pntx2d.h>
#include "model.hpp"

namespace asset
{
    typedef models::model<ogllib::vertex_pntx2d> assimp_model;
    typedef models::multi_texture_mesh<ogllib::vertex_pntx2d> assimp_mesh;

    typedef models::model<ogllib::vertex_pntx2d> gltf_model;
    typedef models::multi_texture_mesh<ogllib::vertex_pntx2d> gltf_mesh;
}

#endif //__ASSET_TYPES_HPP_
