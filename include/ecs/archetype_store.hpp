//
// Created by sava on 9/25/19.
//

#ifndef ECS_DEV_ARCHETYPE_STORE_H
#define ECS_DEV_ARCHETYPE_STORE_H

#include <memory>
#include <unordered_map>
#include <algorithm>

#include <ecs/archetype_id.hpp>
#include <ecs/archetype_pool.hpp>

namespace ecs
{

class archetype_store
{
    // the initial capacity for storage of each archetype
    static const std::uint32_t InitialNumArchetypes;

public:
    ~archetype_store()
    {
    }

    template <class... TComponents>
    std::shared_ptr<std::tuple<TComponents...>> add_entity()
    {
        component_bitset id = archetype_id<TComponents...>();
        ensure_archetype_exists<TComponents...>(id);
        auto& arch_pool = _arch_map.find(id)->second;
        return arch_pool.allocate<std::tuple<TComponents...>>();

    }

    template <class TNew, class... TComponents>
    std::shared_ptr<std::tuple<TNew, TComponents...>> add_component(std::shared_ptr<std::tuple<TComponents...>> entity)
    {
        auto id = archetype_id<TComponents...>();
        ensure_archetype_exists<TComponents...>(id);
        entity.reset();
        return add_entity<TNew, TComponents...>();
    }

    template <class... TComponents>
    const std::vector<std::shared_ptr<std::tuple<TComponents...>>> &read_archetype_all()
    {
        auto id = archetype_id<TComponents...>();
        ensure_archetype_exists(id);
        return _arch_map[id].template read_archetype_all<std::tuple<TComponents...>>();
    }

private:
    std::unordered_map<component_bitset, archetype_pool> _arch_map;

    template <class... TComponents>
    void ensure_archetype_exists(component_bitset id)
    {
        std::unordered_map<component_bitset, archetype_pool>::iterator it = _arch_map.find(id);

        if (it == _arch_map.end())
        {
            auto size = (std::uint32_t) sizeof(std::tuple<TComponents...>);
            auto align = (uintptr_t )alignof(std::tuple<TComponents...>);

            _arch_map.emplace(std::piecewise_construct,
                    std::forward_as_tuple(id),
                    std::forward_as_tuple(size, InitialNumArchetypes, align));
        }
    }
};

    const std::uint32_t archetype_store::InitialNumArchetypes = 100;

} // namespace ecs

#endif //ECS_DEV_ARCHETYPE_STORE_H
