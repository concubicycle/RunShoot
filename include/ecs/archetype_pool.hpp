//
// Created by sava on 10/7/19.
//

#ifndef ECS_ATTEMPT2_ECS_H
#define ECS_ATTEMPT2_ECS_H

#include <map>

#include "ecs_types.hpp"
#include <memory/pool_allocator.hpp>

#include "component_meta.hpp"
#include "chunk_component_accessor.hpp"
#include "entity.hpp"
#include "archetype_chunk_component.hpp"

namespace ecs
{
    /**
     * This wraps a raw memory pool, and hands out entities. Entities contain accessor objects,
     * which know how to get the components from the raw chunk of entity memory.
     */
    class archetype_pool
    {
    public:
        archetype_pool(component_bitset arch_id, std::uint32_t count) :
            _arch_id(arch_id),
            _chunk_size(calc_chunk_size(arch_id)),
            _allocator(
                (std::uint32_t)_chunk_size,
                count,
                chunk_component_accessor::chunk_align())
        {
        }

        entity allocate_entity(entity_id id)
        {
            std::uint8_t *ptr = (std::uint8_t *) _allocator.allocate();
            return entity(id, _arch_id, ptr);
        }

        void free_entity(entity &e)
        {
            e.destroy();
            _allocator.free_element(e.ptr());
        }


    private:
        component_bitset _arch_id;
        size_t _chunk_size;
        allocators::pool_allocator _allocator;

        static size_t calc_chunk_size(component_bitset archetype_id)
        {
            return chunk_component_accessor::chunk_size_for(archetype_id).chunk_size;
        }
    };

} // namespace ecs

#endif //ECS_ATTEMPT2_ECS_H
