//
// Created by sava on 10/9/19.
//

#ifndef __SCENE_H_
#define __SCENE_H_


#include <ecs/ecs_types.hpp>
#include <ecs/entity_world.hpp>

namespace ecs
{


    class scene
    {
    public:
        scene(const entity_world &entity_world) : _entity_world(entity_world)
        {
        }

    private:
        const entity_world& _entity_world;
    };


}

#endif //__SCENE_H_
