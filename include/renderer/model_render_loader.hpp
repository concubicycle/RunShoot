//
// Created by sava on 10/24/19.
//

#ifndef __MODEL_RENDER_LOADER_HPP_
#define __MODEL_RENDER_LOADER_HPP_

#include <vertex_ptx2d.h>
#include <asset/textured_mesh.hpp>
#include <asset/asset_types.hpp>
#include <ecs/components/basic_components.hpp>
#include <util/string_table.hpp>
#include <asset/texture_manager.hpp>
#include <ecs/entity_world.hpp>
#include <asset/assimp_loader.hpp>
#include <asset/basic_mesh_reader.hpp>
#include "shader_set.hpp"

namespace rendering
{
    class model_render_loader
    {
    public:
        model_render_loader(
            const shader_set& shaders,
            const asset::basic_mesh_reader& reader,
            const asset::assimp_loader& assimp_loader,
            string_table& hashes,
            asset::texture_manager& textures);


        void init_render_component(ecs::render_component_ogl &r, asset::assimp_model &mesh);

        void init_render_component(ecs::render_component_ogl &r, models::textured_mesh<ogllib::vertex_ptx2d> &mesh);

        void init_entity_world_render_components(ecs::entity_world world);

    private:
        const shader_set& _shaders;
        const asset::basic_mesh_reader& _reader;
        const asset::assimp_loader& _assimp_loader;
        string_table& _string_table;
        asset::texture_manager &_textures;


        void init_mesh(
            ecs::render_component_ogl &r,
            models::textured_mesh<ogllib::vertex_ptx2d> &mesh,
            std::uint32_t mesh_index);

        void init_mesh(
            ecs::render_component_ogl &r,
            asset::assimp_mesh &mesh,
            std::uint32_t mesh_index);
    };
}

#endif //__MODEL_RENDER_LOADER_HPP_
