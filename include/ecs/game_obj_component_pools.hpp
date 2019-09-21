#ifndef __GAME_OBJ_COMPONENT_POOLS_H_
#define __GAME_OBJ_COMPONENT_POOLS_H_

#include <map>
#include <memory/typed_pool_allocator.hpp>

#include <ecs/behavior_component.hpp>

namespace ecs
{
template <typename TComponent>
class component_pool
{
    component_pool() : _allocator(MIN_COMPONENTS)
    {
    }

private:
    allocators::typed_pool_allocator<TComponent> _allocator;
    std::map<game_object_id, TComponent *> _game_obj_to_instance;
};

class game_obj_component_pools
{
private:
    component_pool<behavior_component> behaviors;
};

} // namespace ecs

#endif
