#ifndef __GAME_OBJECT_FACTORY_H_
#define __GAME_OBJECT_FACTORY_H_

#include <ecs/archetype_store.hpp>
#include <ecs/component_store.hpp>

namespace ecs
{
class game_object_factory
{
public:
    game_object_factory(
        const archetype_store &archetype_store,
        const component_store &component_store) : _archetype_store(archetype_store),
                                                  _component_store(component_store)
    {
    }

private:
    const archetype_store &_archetype_store;
    const component_store &_component_store;
};
} // namespace ecs

#endif
