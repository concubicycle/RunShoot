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

#include <util/string_table.hpp>
#include "component_loader.hpp"
#include "prototype_loader.hpp"

using nlohmann::json;

namespace asset
{
    class scene_loader
    {
    public:
        explicit scene_loader(
            component_loader& component_loader,
            prototype_loader& prototypes);

        asset::scene load_scene(const std::string& file_path, ecs::entity_world& world);

    private:
        component_loader& _component_loader;
        prototype_loader& _prototypes;

        component_bitset calc_archetype_id(const json &components) const;
        void load_entity_components(ecs::entity &e, const json& component_array) const;

        json merge_prototype_and_scene_entity(json& scene_entity);
    };
}

#endif //__SCENE_LOADER_HPP_
