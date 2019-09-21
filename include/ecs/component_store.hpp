#ifndef __COMPONENT_STORE_H_
#define __COMPONENT_STORE_H_

#include <map>
#include <memory/typed_pool_allocator.hpp>

#include <ecs/ecs_types.hpp>

#define MIN_COMPONENTS 512

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

/**
 * component_store will have pools of components, managed by whatever allocators is best. 
 * it will also have lookups for game object components (game_object_id -> std::vector<c
 * */
class component_store
{
private:
    /*

    component_pool<Transform> _transforms;
    component_pool<Renderable> _renderables;

    */
};

} // namespace ecs

#endif
