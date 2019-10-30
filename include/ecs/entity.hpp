//
// Created by sava on 10/8/19.
//

#ifndef __ENTITY_H_
#define __ENTITY_H_

#include "chunk_component_accessor.hpp"
#include "ecs_types.hpp"

namespace ecs
{

/**
 * Need better .. semantics? around constructors/destructors. Don't want to call _accessor.destruct() in
 * entity's destructor, since that will actually destroy the components in memory, when we might just be
 * moving the entity. We can delete copy constructor, and only pass around references, with a master copy
 * being kept in a map in archetype_store. hmm...
 */
    class entity
    {
    public:
        entity(
            entity_id id,
            component_bitset archetype_id,
            std::uint8_t *chunk_ptr) :
            _id(id),
            _accessor(chunk_ptr, archetype_id),
            _active(true),
            _archetype_id(archetype_id)
        {
            _accessor.construct();
        }

        entity(const entity& other) :
            _id(other._id),
            _accessor(other._accessor),
            _active(other._active),
            _archetype_id(other._archetype_id)
        {
        }

        entity_id id() { return _id; }

        void *ptr() const { return _accessor.ptr(); }
        component_bitset archetype_id() const { return _archetype_id; }

        void destroy()
        {
            _accessor.destruct();
            _active = false;
        }

        template<class T>
        T &get_component() { return *(_accessor.get_component<T>()); }

        template<class T>
        bool has()
        {
            return _archetype_id & component<T>::archetype_bit;
        }

        bool has(component_bitset component_bit) const
        {
            return _archetype_id & component_bit;
        }

        void copy_components_from(entity& other)
        {
            for(std::uint8_t i = 0; i < ECS_MAX_COMPONENT_TYPES; ++i)
            {
                component_bitset bit = component_bitset(1) << i;

                if (!has(bit) || !other.has(bit)) continue;

                auto& meta = component_meta::bit_metas.find(i)->second;

                void* destination = _accessor.get_component(i);
                void* source = other._accessor.get_component(i);

                meta.copy(destination, source);
            }
        }


    private:
        entity_id _id;
        component_bitset  _archetype_id;
        chunk_component_accessor _accessor;
        bool _active;
    };
}


#endif //__ENTITY_H_
