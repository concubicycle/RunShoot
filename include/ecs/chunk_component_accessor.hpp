//
// Created by sava on 10/8/19.
//

#ifndef __CHUNK_COMPONENT_ACCESSOR_H_
#define __CHUNK_COMPONENT_ACCESSOR_H_

#include <map>
#include "archetype_chunk_component.hpp"

namespace ecs
{
    class chunk_component_accessor
    {
    public:
        chunk_component_accessor(
            const std::map<std::uint8_t, archetype_chunk_component> &shift_to_chunk_component,
            std::uint8_t *chunk_ptr) :
            _shift_to_chunk_component(shift_to_chunk_component),
            _chunk_ptr(chunk_ptr),
            _shift_to_component_addr(calc_shift_to_addr(chunk_ptr, shift_to_chunk_component))
        {
        }

        chunk_component_accessor(const chunk_component_accessor &other) :
            _chunk_ptr(other._chunk_ptr),
            _shift_to_chunk_component(other._shift_to_chunk_component),
            _shift_to_component_addr(other._shift_to_component_addr)
        {
        }

        void *ptr() const { return _chunk_ptr; }

        template<class T>
        T *get_component()
        {
            auto it = _shift_to_component_addr.find(component<T>::component_bitshift);
            if (it == _shift_to_component_addr.end())
                return nullptr;
            return (T *) it->second;
        }

        /**
             * The component metadata objects stored in archetype_chunk_component.meta contain wrappers for
             * in-place constructor and destructor of that particular type. We can go through all the
             * archetype_chunk_component instances, grab pointer offset, and construct/destruct where it says.
             */
        void construct()
        {
            for (auto const &x : _shift_to_component_addr)
            {
                void *ptr = x.second;
                auto meta = _shift_to_chunk_component.find(x.first)->second.meta;
                meta.construct(ptr);
            }
        }

        void destruct()
        {
            for (auto const &x : _shift_to_component_addr)
            {
                void *ptr = x.second;
                auto meta = _shift_to_chunk_component.find(x.first)->second.meta;
                meta.destruct(ptr);
            }
        }

    private:
        const std::map<std::uint8_t, archetype_chunk_component> &_shift_to_chunk_component;
        std::map<std::uint8_t, void *> _shift_to_component_addr;
        std::uint8_t *_chunk_ptr;

        static std::map<std::uint8_t, void *> calc_shift_to_addr(
            void *chunk_ptr,
            const std::map<std::uint8_t, archetype_chunk_component> &chunk_info)
        {
            std::map<std::uint8_t, void *> shift_to_addr;

            uintptr_t chunk_start = (uintptr_t) chunk_ptr;
            uintptr_t ptr_offset = 0;

            for (auto &x : chunk_info)
            {
                auto &component_desc = x.second;
                auto &meta = component_desc.meta;
                auto adjustment = calc_align_adjustment(chunk_start + ptr_offset, meta.align());

                ptr_offset += adjustment;
                shift_to_addr[x.first] = (void *) (chunk_start + ptr_offset);
                ptr_offset += meta.size();
            }

            return shift_to_addr;
        }

        /**
            * TODO: Code duplication
            * Calculate adjustment by masking off the lower bits of the address,
            * to determine how 'misaligned' it is.
            * */
        static uintptr_t calc_align_adjustment(uintptr_t raw, uintptr_t alignment)
        {
            if (alignment == 0)
                return 0;

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

} // namespace ecs

#endif //__CHUNK_COMPONENT_ACCESSOR_H_
