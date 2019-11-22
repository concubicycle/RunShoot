//
// Created by sava on 11/21/19.
//

#ifndef __PROTOTYPE_SPAWNER_HPP_
#define __PROTOTYPE_SPAWNER_HPP_

#include <string>

#include <events/event_exchange.hpp>
#include <scene/scene_graph_root.hpp>
#include <ecs/entity_world.hpp>
#include "component_loader.hpp"
#include "prototype_loader.hpp"

namespace asset
{
    class prototype_spawner
    {
        using scene_graph_t = scene_graph::scene_graph_root<ecs::entity, entity_id>;

    public:
        explicit prototype_spawner(
            events::event_exchange &events,
            component_loader &component_loader,
            prototype_loader &prototypes);

        ecs::entity &load_prototype(
            const std::string& prototype_path,
            ecs::entity_world &world,
            scene_graph_t &scene_graph,
            entity_id id);

        ecs::entity &load_prototype(
            const std::string& prototype_path,
            ecs::entity_world &world,
            scene_graph_t &scene_graph);

        ecs::entity &load_prototype(
            json &prototype,
            ecs::entity_world &world,
            scene_graph_t &scene_graph,
            entity_id id);



        json inflate_prototype(json &entity_json);

    private:
        events::event_exchange &_events;
        component_loader &_component_loader;
        prototype_loader &_prototypes;

        [[nodiscard]] component_bitset calc_archetype_id(const json &components) const;

        void load_entity_components(ecs::entity &e, const json &component_array) const;

        ecs::entity &load_prototype(
            json &prototype,
            ecs::entity_world &world,
            scene_graph_t &scene_graph);

        ecs::entity &load_prototype(
            json &prototype,
            ecs::entity_world &world,
            scene_graph_t &scene_graph,
            ecs::entity &e_root);

        static void merge_component_props(
            json &target,
            json &source);

        static void scene_graph_insert(
            scene_graph_t &scene_graph,
            ecs::entity &e);

        static void scene_graph_insert(
            scene_graph_t &scene_graph,
            ecs::entity &e,
            json &e_json);

        static void scene_graph_insert(
            scene_graph_t &scene_graph,
            ecs::entity &e,
            entity_id parent_id);
    };
}

#endif //__PROTOTYPE_SPAWNER_HPP_
