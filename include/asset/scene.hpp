//
// Created by sava on 10/9/19.
//

#ifndef __SCENE_H_
#define __SCENE_H_


#include <ecs/ecs_types.hpp>
#include <ecs/entity_world.hpp>

namespace asset
{
    class scene
    {
    public:
        scene(const ecs::entity_world &entity_world) : _entity_world(entity_world)
        {
        }

        const ecs::entity_world& entity_world() { return _entity_world; }

    private:
        const ecs::entity_world& _entity_world;
    };


}

#endif //__SCENE_H_
