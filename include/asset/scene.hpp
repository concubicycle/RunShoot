//
// Created by sava on 10/9/19.
//

#ifndef __SCENE_H_
#define __SCENE_H_


#include <ecs/ecs_types.hpp>
#include <ecs/entity_world.hpp>
#include "scene_graph_root.hpp"

namespace asset
{
    class scene
    {
    public:
        using scene_graph_t = scene_graph_root<ecs::entity, entity_id>;


        scene(
            ecs::entity_world &entity_world) : _entity_world(entity_world)
        {
        }

        ecs::entity_world& entity_world() { return _entity_world; }

        scene_graph_t& scene_graph() { return _scene_graph; }

    private:
        ecs::entity_world& _entity_world;
        scene_graph_t _scene_graph;
    };


}

#endif //__SCENE_H_
