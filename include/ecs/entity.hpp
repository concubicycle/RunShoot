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
            const std::map<std::uint8_t, archetype_chunk_component> &shift_to_chunk_component,
            std::uint8_t *chunk_ptr) :
            _accessor(shift_to_chunk_component, chunk_ptr),
            _active(true),
            _archetype_id(0)
        {
            _accessor.construct();

            for (auto& pair : shift_to_chunk_component)
            {
                _archetype_id |= component_bitset(1) << pair.first;
            }
        }

        entity(
            entity_id id,
            component_bitset archetype_id,
            const std::map<std::uint8_t, archetype_chunk_component> &shift_to_chunk_component,
            std::uint8_t *chunk_ptr) :
            _accessor(shift_to_chunk_component, chunk_ptr),
            _active(true),
            _archetype_id(archetype_id)
        {
            _accessor.construct();
        }

        entity(const entity& other) :
        _accessor(other._accessor),
        _active(other._active),
        _archetype_id(other._archetype_id)
        {
        }

        void *ptr() const { return _accessor.ptr(); }
        component_bitset archetype_id() const { return _archetype_id; }

        void destroy()
        {
            _accessor.destruct();
            _active = false;
        }

        template<class T>
        T &get_component() { return *(_accessor.get_component<T>()); }

    private:
        component_bitset  _archetype_id;
        chunk_component_accessor _accessor;
        bool _active;
    };


}


#endif //__ENTITY_H_
