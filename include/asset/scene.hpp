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

        scene(prototype_spawner& spawner, ecs::entity_world& world) : _spawner(spawner), _entity_world(world)
        {}

        ecs::entity_world& entity_world() { return _entity_world; }

        scene_graph_t& scene_graph() { return _scene_graph; }

    private:
        prototype_spawner& _spawner;
        ecs::entity_world& _entity_world;
        scene_graph_t _scene_graph;
    };
}




#endif //__SCENE_H_
