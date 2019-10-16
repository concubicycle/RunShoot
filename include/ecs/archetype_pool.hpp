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
            _shift_to_chunk_component_descriptor(calc_shift_to_chunk_component_descriptor(_arch_id)),
            _chunk_size(calc_chunk_size(_shift_to_chunk_component_descriptor)),
            _allocator(
                _chunk_size,
                count,
                _shift_to_chunk_component_descriptor.begin()->second.meta.align()) {
            int i = 0;
        }

        entity allocate_entity(entity_id id)
        {
            std::uint8_t *ptr = (std::uint8_t *) _allocator.allocate();
            return entity(id, _shift_to_chunk_component_descriptor, ptr);
        }

        void free_entity(entity &e)
        {
            e.destroy();
            _allocator.free_element(e.ptr());
        }


    private:
        component_bitset _arch_id;
        std::map<std::uint8_t, archetype_chunk_component> _shift_to_chunk_component_descriptor;
        std::uint32_t _chunk_size;
        allocators::pool_allocator _allocator;


        /**
         * Creates a lookup of component type metadata for component bitshifts.
         * @param arch_id archetype id bitset
         * @return a lookup of component type metadata for component bitshifts.
         */
        static std::map<std::uint8_t, archetype_chunk_component> calc_shift_to_chunk_component_descriptor(
            component_bitset arch_id)
        {
            std::map<std::uint8_t, archetype_chunk_component> ret;
            std::uint32_t ptr_offset = 0;

            for (auto &x : component_meta::bit_metas)
            {
                auto shift = x.first;
                component_bitset component_bit = (component_bitset)(1 << shift);

                if (!(arch_id & component_bit)) continue;

                auto &meta = x.second;

                auto adjustment = calc_align_adjustment(ptr_offset, meta.align());
                ptr_offset += adjustment;

                ret.emplace(std::piecewise_construct,
                    std::forward_as_tuple(shift),
                    std::forward_as_tuple(ptr_offset, meta));

                ptr_offset += meta.size();

            }

            return ret;
        }

        /**
         * Calculate the chunk size for an archetype chunk.
         * The whole chunk has to be aligned like the first component.
         * After the first component, add alignment padding for each subsequent component.
         *
         * @param shift_to_chunk_desc a map of the bit set bits that identify this pool's
         * archetype to the metadata for that component type
         *
         * @return _chunk_size of the raw pool
         */
        static std::uint32_t calc_chunk_size(
            const std::map<std::uint8_t, archetype_chunk_component> &chunk_components)
        {
            std::uint32_t ptr_offset = 0;

            for (auto const &x : chunk_components)
            {
                ptr_offset += x.second.meta.size();
            }

            auto first_align = chunk_components.begin()->second.meta.align();
            ptr_offset += calc_align_adjustment(ptr_offset, first_align);

            return ptr_offset;
        }

        /**
         * Calculate adjustment by masking off the lower bits of the address,
         * to determine how 'misaligned' it is.
         * */
        static uintptr_t calc_align_adjustment(uintptr_t raw, uintptr_t alignment)
        {
            if (alignment == 0) return 0;

            uintptr_t mask = (alignment - 1);
            uintptr_t misalignment = raw & mask;

            std::uint32_t mask_32 = ((std::uint32_t) alignment) - 1;
            std::uint32_t raw_32 = (std::uint32_t) raw;
            std::uint32_t misalignment_32 = mask_32 & raw_32;

            return misalignment > 0
                   ? alignment - misalignment
                   : 0;
        }
    };

}

#endif //ECS_ATTEMPT2_ECS_H
