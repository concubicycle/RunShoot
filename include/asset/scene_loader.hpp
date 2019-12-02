//
// Created by sava on 10/13/19.
//

#ifndef __SCENE_LOADER_HPP_
#define __SCENE_LOADER_HPP_

#include <string>
#include <unordered_map>
#include <functional>
#include <nlohmann/json.hpp>
#include <ecs/entity.hpp>
#include <asset/scene.hpp>
#include <asset/texture_manager.hpp>
#include <ecs/components/basic_components.hpp>

#include <scene/scene_graph_node_base.hpp>

#include <util/string_table.hpp>
#include "component_loader.hpp"
#include "prototype_loader.hpp"
#include "prototype_spawner.hpp"
#include "scene.hpp"

using nlohmann::json;

namespace asset
{
    class scene_loader
    {

    public:
        explicit scene_loader(prototype_spawner& spawner);

        void load_scene(const std::string& file_path, ecs::entity_world& world, asset::scene& scene);

    private:
        prototype_spawner& _spawner;
    };
}

#endif //__SCENE_LOADER_HPP_
