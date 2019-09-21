#ifndef __COMPONENT_STORE_H_
#define __COMPONENT_STORE_H_

#include <ecs/ecs_types.hpp>
#include <ecs/game_obj_component_pools.hpp>

#define MIN_COMPONENTS 512

namespace ecs
{
class component_store
{
public:
    component_store(const game_obj_component_pools &pools) : _pools(pools) {}

private:
    const game_obj_component_pools &_pools;
};

} // namespace ecs

#endif
