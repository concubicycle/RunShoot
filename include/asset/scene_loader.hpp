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
#include <scene/scene.hpp>
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
            events::event_exchange& events,
            component_loader& component_loader,
            prototype_loader& prototypes);

        scene_graph::scene load_scene(const std::string& file_path, ecs::entity_world& world);

    private:
        events::event_exchange& _events;
        component_loader& _component_loader;
        prototype_loader& _prototypes;

        component_bitset calc_archetype_id(const json &components) const;
        void load_entity_components(ecs::entity &e, const json& component_array) const;

        json inflate_prototype(json& entity_json);
        static void merge_component_props(json& target, json& source);

        ecs::entity& load_prototype(
            json& prototype,
            ecs::entity_world& world,
            scene_graph::scene::scene_graph_t& scene_graph,
            entity_id id);

        ecs::entity& load_prototype(
            json& prototype,
            ecs::entity_world& world,
            scene_graph::scene::scene_graph_t& scene_graph);

        ecs::entity& load_prototype(
            json& prototype,
            ecs::entity_world& world,
            scene_graph::scene::scene_graph_t& scene_graph,
            ecs::entity& e_root);

        void scene_graph_insert(scene_graph::scene::scene_graph_t& scene_graph, ecs::entity& e, json& e_json);
        void scene_graph_insert(scene_graph::scene::scene_graph_t& scene_graph, ecs::entity& e, entity_id parent_id);

    };
}

#endif //__SCENE_LOADER_HPP_
