//
// Created by sava on 10/8/19.
//

#ifndef ECS_ATTEMPT2_COMPONENT_META_H
#define ECS_ATTEMPT2_COMPONENT_META_H

#include <map>
#include <cstdint>
#include <functional>

#include <nlohmann/json.hpp>
using nlohmann::json;

#include "ecs_types.hpp"
#include "components/basic_components.hpp"
#include "component.hpp"

namespace ecs
{

    class component_meta
    {
    public:
        component_meta &operator=(const component_meta &other)
        {
            _size = other._size;
            _align = other._align;
            _shift = other._shift;
            return *this;
        }

        /**
         * Function for constructing a type metadata object of type T. Only default ctor supported for now.
         * @tparam T A component type. must have a static component_bitshift member.
         * @return A metadata object for type T
         */
        template<class T>
        static component_meta of()
        {
            return component_meta(
                sizeof(T),
                alignof(T),
                component<T>::component_bitshift,
                [](void *addr)
                { new(addr) T(); },
                [](void *addr)
                { ((T *) addr)->~T(); });
        }

        static std::map<std::uint8_t, component_meta> bit_metas;

        size_t size() const { return _size; }

        size_t align() const { return _align; }

        std::uint8_t shift() const { return _shift; }

        void construct(void *addr) { _ctor(addr); }

        void destruct(void *addr) { _dtor(addr); }


    private:
        component_meta(
            size_t size,
            size_t align,
            std::uint8_t shift,
            std::function<void(void *)> ctor,
            std::function<void(void *)> dtor) :
            _size(size),
            _align(align),
            _shift(shift),
            _ctor(ctor),
            _dtor(dtor)
        {
        }

        // const as far as client goes, but need to assign-construct.
        mutable size_t _size;
        mutable size_t _align;
        mutable std::uint8_t _shift;

        mutable std::function<void(void *)> _ctor;
        mutable std::function<void(json &, void *)> _deserialize_ctor;
        mutable std::function<void(void *)> _dtor;
    };
}

#endif //ECS_ATTEMPT2_COMPONENT_META_H
