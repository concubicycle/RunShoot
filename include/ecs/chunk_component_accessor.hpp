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
            _chunk_ptr(chunk_ptr) {}

        chunk_component_accessor(const chunk_component_accessor& other) :
            _chunk_ptr(other._chunk_ptr),
            _shift_to_chunk_component(other._shift_to_chunk_component)
        {
        }

        void *ptr() const { return _chunk_ptr; }

        template<class T>
        T *get_component()
        {
            auto it = _shift_to_chunk_component.find(component<T>::component_bitshift);
            if (it == _shift_to_chunk_component.end()) return nullptr;

            auto offset = it->second.chunk_component_offset;
            std::uint8_t *addr = _chunk_ptr + offset;
            return (T *) addr;
        }

        /**
         * The component metadata objects stored in archetype_chunk_component.meta contain wrappers for
         * in-place constructor and destructor of that particular type. We can go through all the
         * archetype_chunk_component instances, grab pointer offset, and construct/destruct where it says.
         */
        void construct()
        {
            for (auto const &x : _shift_to_chunk_component)
            {
                void *ptr = (void *) (_chunk_ptr + x.second.chunk_component_offset);
                x.second.meta.construct(ptr);
            }
        }

        void destruct()
        {
            for (auto const &x : _shift_to_chunk_component)
            {
                void *ptr = (void *) (_chunk_ptr + x.second.chunk_component_offset);
                x.second.meta.destruct(ptr);
            }
        }


    private:
        const std::map<std::uint8_t, archetype_chunk_component> &_shift_to_chunk_component;
        std::uint8_t *_chunk_ptr;
    };


}

#endif //__CHUNK_COMPONENT_ACCESSOR_H_
