//
// Created by sava on 10/9/19.
//

#ifndef __SCENE_H_
#define __SCENE_H_


#include <ecs/ecs_types.hpp>
#include <ecs/entity_world.hpp>
#include <scene/scene_graph_root.hpp>
#include "prototype_spawner.hpp"

#include <asset/prototype_spawner.hpp>


namespace asset
{
    class scene
    {
    public:
        using scene_graph_t = scene_graph::scene_graph_root<ecs::entity, entity_id>;

        scene(prototype_spawner &spawner, ecs::entity_world &world) :
            _spawner(spawner),
            _entity_world(world),
            _scene_graph(extract_transform) {}

        scene(scene &&other) :
            _spawner(other._spawner),
            _entity_world(other._entity_world),
            _scene_graph(std::move(other._scene_graph))
        {
            int i = 0;
            i++;
        }

        ecs::entity_world &entity_world() { return _entity_world; }

        scene_graph_t &scene_graph() { return _scene_graph; }

        ecs::entity &load_prototype(const std::string &prototype_path)
        {
            return _spawner.load_prototype(prototype_path, _entity_world, _scene_graph);
        }

    private:
        prototype_spawner &_spawner;
        ecs::entity_world &_entity_world;
        scene_graph_t _scene_graph;

        static glm::mat4 extract_transform(ecs::entity &e)
        {
            return e.has<ecs::transform_component>()
                   ? e.get_component<ecs::transform_component>().to_mat4()
                   : glm::mat4(1.f);
        }
    };
}


#endif //__SCENE_H_
