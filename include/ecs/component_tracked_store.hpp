//
// Created by sava on 9/27/19.
//

#ifndef ECS_DEV_COMPONENT_TRACKED_STORE_H
#define ECS_DEV_COMPONENT_TRACKED_STORE_H

#include <ecs/ecs_types.hpp>
#include <ecs/archetype_store.hpp>
#include <ecs/components/basic_components.hpp>

#include <vector>
#include <unordered_map>

namespace ecs
{

/**
 * Game scripts will prefer entities as tuples of components, but systems like rendering just want a
 * list of some component, like render components. Here, we maintain vectors of just specific components.
 *
 * Not sure if this defeats the whole purpose of trying to pool archetypes.
 *
 * Find a way to generalize this? Burn it all down? Hmm...
 */
class component_tracked_store : public archetype_store
{
public:
    component_tracked_store()
    {
    }

    template <typename... TComponents>
    std::shared_ptr<std::tuple<TComponents...>> add_renderable_entity()
    {
        auto tuple_ptr = add_entity<TComponents...>();
        render_component &r = std::get<render_component>(*tuple_ptr);

        std::tuple<transform_component &, render_component &> ren_tuple = std::forward_as_tuple(
            std::get<transform_component>(*tuple_ptr),
            std::get<render_component>(*tuple_ptr));

        _renderables.push_back(ren_tuple);

        return tuple_ptr;
    }

    // ...

    std::vector<std::tuple<transform_component &, render_component &>> &renderables() { return _renderables; }

private:
    std::vector<std::tuple<transform_component &, render_component &>> _renderables;
};

} // namespace ecs

#endif //ECS_DEV_COMPONENT_TRACKED_STORE_H
