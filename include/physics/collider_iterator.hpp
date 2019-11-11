//
// Created by sava on 11/6/19.
//

#ifndef __COLLIDER_ITERATOR_HPP_
#define __COLLIDER_ITERATOR_HPP_

#include <ecs/entity.hpp>


namespace physics
{
    using aabb_colliders = ecs::aabb_collider_component;
    using aabb_opt = std::optional<std::reference_wrapper<aabb_colliders>>;

    using sphere_colliders = ecs::sphere_collider_component;
    using sphere_opt = std::optional<std::reference_wrapper<sphere_colliders>>;

    class collider_iterator
    {
    public:
        collider_iterator(ecs::entity &entity) :
            _entity(entity),
            _current_type(aabb),
            _current_index(0),
            _aabb(_entity.get_component_opt<aabb_colliders>()),
            _sphere(_entity.get_component_opt<sphere_colliders>())
        {}

        physics_models::collider *end() { return nullptr; }

        physics_models::collider *get_next()
        {
            if (_current_type == aabb)
                if (!_aabb || _current_index >= _aabb->get().count)
                    reset_to(sphere);
                else
                    return _aabb->get().colliders + _current_index++;

            if (_current_type == sphere)
                if (!_sphere || _current_index >= _sphere->get().count)
                    reset_to(none);
                else
                    return _sphere->get().colliders + _current_index++;

            return end();
        }

        void reset()
        {
            _current_index = 0;
            _current_type = aabb;
        }

    private:
        enum collider_type
        {
            aabb = 1, sphere = 2, none = 3
        };

        ecs::entity &_entity;
        collider_type _current_type;
        std::uint32_t _current_index;
        aabb_opt _aabb;
        sphere_opt _sphere;

        void reset_to(collider_type ct)
        {
            _current_index = 0;
            _current_type = ct;
        }
    };

}

#endif //__COLLIDER_ITERATOR_HPP_
