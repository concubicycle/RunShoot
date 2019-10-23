//
// Created by sava on 10/23/19.
//

#ifndef RS__ASSIMP_MESH_LOADER_HPP_
#define RS__ASSIMP_MESH_LOADER_HPP_


#include <vertex_pntx2d.hpp>

#include <asset/model.hpp>

#include <tl/expected.hpp>
#include "mesh_read_error.hpp"


namespace asset
{
    typedef models::model<ogllib::vertex_pntx2d> assimp_model;

    class assimp_loader
    {
        using mesh_result = tl::expected<assimp_model, mesh_read_error>;


    public:
        mesh_result load_model(std::string path);
    private:
    };


}
#endif //__ASSIMP_LOADER_HPP_
