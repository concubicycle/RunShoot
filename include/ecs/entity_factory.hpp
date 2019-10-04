#ifndef __ENTITY_FACTORY_H_
#define __ENTITY_FACTORY_H_

#include <ecs/archetype_store.hpp>

#include <unordered_set>
#include <cstdint>
#include <algorithm>

#include <ecs/ecs_types.hpp>
#include <ecs/entity.hpp>

#define MIN_ENTITY_IDS 512

namespace ecs
{

class entity_factory
{
public:
    entity_factory(
        component_tracked_store &archetype_store,
        entity_base *existing_entities,
        std::uint32_t existing_entities_count) : entity_factory(archetype_store)
    {
        for (std::uint32_t i = 0; i < existing_entities_count; ++i)
        {
            auto id = existing_entities[i].id();
            _ids.insert(id);
            _max_id = std::max(_max_id, id);
            _ids.insert(id);
        }
        _max_id++;
    }

    entity_factory(
        component_tracked_store &archetype_store) : _archetype_store(archetype_store)
    {
        _ids.reserve(MIN_ENTITY_IDS);
    }

    template <class... TComponents>
    entity<TComponents...> add_entity()
    {
        auto components = _archetype_store.add_entity<TComponents...>();
        auto id = _max_id++;

        _ids.insert(id);

        return entity<TComponents...>(id, components);
    }

    template <class TNew, class... TComponents>
    entity<TNew, TComponents...> add_component(entity<TComponents...> &e)
    {
        auto new_components = _archetype_store.add_component<TNew, TComponents...>(e.components());
        return entity<TNew, TComponents...>(e.id(), new_components);
    }

private:
    component_tracked_store &_archetype_store;

    std::unordered_set<entity_id> _ids;
    entity_id _max_id = 1;
};
} // namespace ecs

#endif