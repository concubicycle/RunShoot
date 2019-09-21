#ifndef __COMPONENT_H_
#define __COMPONENT_H_

#include <cstddef>
#include <cstdint>

#include <fstream>

#include <ecs/game_object.hpp>
#include <ecs/game_obj_component_pools.hpp>

namespace ecs
{

class component
{
public:
    component(const game_obj_component_pools &pools) : _pools(pools)
    {
    }

protected:
    const game_obj_component_pools &_pools;
};

#endif
