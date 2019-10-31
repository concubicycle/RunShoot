//
// Created by sava on 10/23/19.
//

#ifndef RS__ASSIMP_MESH_LOADER_HPP_
#define RS__ASSIMP_MESH_LOADER_HPP_

#include <vector>

#include <vertex_pntx2d.h>
#include <asset/model.hpp>
#include <asset/mesh_type.hpp>
#include <tl/expected.hpp>
#include <assimp/scene.h>
#include "mesh_read_error.hpp"
#include "asset_types.hpp"
#include <ecs/components/basic_components.hpp>

namespace asset
{
    class assimp_loader
    {
        using mesh_result = tl::expected<assimp_model, mesh_read_error>;

    public:
        mesh_result load_model(const std::string& path, asset::mesh_type mesh_type) const;

    private:
        mesh_result load_glf2(const std::string& path) const;

        void process_node(
            aiNode *node,
            const aiScene *scene,
            assimp_model& model,
            const std::string& mesh_path) const;

        assimp_mesh process_mesh(
            aiMesh *mesh,
            const aiScene *scene,
            const std::string& mesh_path) const;

        std::vector<models::texture> load_material_textures(
            aiMaterial *mat,
            aiTextureType type,
            const std::string& typeName,
            const std::string& mesh_path) const;
    };
}
#endif //__ASSIMP_LOADER_HPP_
