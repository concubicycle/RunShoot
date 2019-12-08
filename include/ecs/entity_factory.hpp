//
// Created by sava on 10/8/19.
//

#ifndef __ENTITY_FACTORY_H_
#define __ENTITY_FACTORY_H_

#include <unordered_map>
#include <unordered_set>
#include <cstdint>

#include "ecs_types.hpp"
#include "archetype_pool.hpp"
#include "archetype_id.hpp"



namespace ecs
{
    class entity_factory
    {
        static const std::uint32_t MinArchetypeChunks;

    public:
        entity_factory(std::uint32_t next_id) : _next_id(next_id) {}
        entity_factory(const entity_factory& other) = delete;
        entity_factory &operator=(const entity_factory &other) = delete;


        template<class... TComponents>
        entity make_entity()
        {
            auto arch_id = archetype_id<TComponents...>();
            return make_entity(arch_id, next_id());
        }

        entity make_entity(component_bitset id)
        {
            return make_entity(id, next_id());
        }

        entity make_entity(component_bitset archetype_id, entity_id id)
        {
            auto& pool = find_pool(archetype_id);

            if (_ids.find(id) != _ids.end())
                throw "Entity id already exists"; // TODO: Handle this better.

            _ids.insert(id);

            assert(_archetype_pools.find(archetype_id) != _archetype_pools.end());
            return _archetype_pools.find(archetype_id)->second.allocate_entity(id);
        }

        void free_entity(entity& e)
        {
            auto id = e.archetype_id();
            auto it = _archetype_pools.find(id);

            if (it == _archetype_pools.end())
            {
                return;
            }

            _ids.erase(e.id());

            return _archetype_pools.find(id)->second.free_entity(e);
        }

        template <class TComponent>
        entity add_component(entity& e)
        {
            auto new_archetype = component<TComponent>::archetype_bit | e.archetype_id();
            auto& new_pool = find_pool(new_archetype);
            auto new_entity = new_pool.allocate_entity(e.id());
            new_entity.copy_components_from(e);

            auto& old_pool = find_pool(e.archetype_id());
            old_pool.free_entity(e);

            return new_entity;
        }


    private:
        std::unordered_map<component_bitset, archetype_pool> _archetype_pools;
        std::unordered_set<entity_id> _ids;
        entity_id _next_id;

        inline entity_id next_id()
        {
            entity_id e_id = ++_next_id;
            while (_ids.find(e_id) != _ids.end()) e_id++;
            return e_id;
        }

        archetype_pool& find_pool(component_bitset archetype_id)
        {
            auto it = _archetype_pools.find(archetype_id);

            if (it == _archetype_pools.end())
            {
                _archetype_pools.emplace(std::piecewise_construct,
                    std::forward_as_tuple(archetype_id),
                    std::forward_as_tuple(archetype_id, MinArchetypeChunks));
            }

            return _archetype_pools.find(archetype_id)->second;
        }
    };
}

#endif //__ENTITY_FACTORY_H_
