//
// Created by sava on 11/1/19.
//

#include <ecs/ecs_types.hpp>
#include <physics/collisions.hpp>


using aabb_colliders = ecs::aabb_collider_component;
using aabb_opt = std::optional<std::reference_wrapper<aabb_colliders>>;

using sphere_colliders = ecs::sphere_collider_component;
using sphere_opt = std::optional<std::reference_wrapper<sphere_colliders>>;


physics_models::contact physics::collisions::check_collision_and_generate_contact(ecs::entity &one, ecs::entity &two)
{
    _contact_buffer.clear();

    aabb_opt aabb_one = one.get_component_opt<aabb_colliders>();
    aabb_opt aabb_two = two.get_component_opt<aabb_colliders>();

    sphere_opt sphere_one = one.get_component_opt<sphere_colliders>();
    sphere_opt sphere_two = two.get_component_opt<sphere_colliders>();

    if (aabb_one && aabb_two)
    {
        auto count_one = aabb_one->get().count;
        auto count_two = aabb_two->get().count;

        for (std::uint32_t i = 0; i < count_one; ++i)
        {
            for (std::uint32_t j = 0; j < count_two; ++j)
            {
                auto& c1 = aabb_one->get().colliders[i];
                auto& c2 = aabb_one->get().colliders[j];
                _contact_buffer.insert(c1.accept(c2));
            }
        }
    }

    if (aabb_one && sphere_two)
    {
        auto count_one = aabb_one->get().count;
        auto count_two = aabb_two->get().count;

        for (std::uint32_t i = 0; i < count_one; ++i)
        {
            for (std::uint32_t j = 0; j < count_two; ++j)
            {
                auto& c1 = aabb_one->get().colliders[i];
                auto& c2 = sphere_two->get().colliders[j];
                _contact_buffer.insert(c1.accept(c2));
            }
        }
    }

    if (sphere_one && aabb_two)
    {
        auto count_one = aabb_one->get().count;
        auto count_two = aabb_two->get().count;

        for (std::uint32_t i = 0; i < count_one; ++i)
        {
            for (std::uint32_t j = 0; j < count_two; ++j)
            {
                auto& c1 = sphere_one->get().colliders[i];
                auto& c2 = aabb_two->get().colliders[j];
                _contact_buffer.insert(c1.accept(c2));
            }
        }
    }

    if (sphere_one && sphere_two)
    {
        auto count_one = aabb_one->get().count;
        auto count_two = aabb_two->get().count;

        for (std::uint32_t i = 0; i < count_one; ++i)
        {
            for (std::uint32_t j = 0; j < count_two; ++j)
            {
                auto& c1 = sphere_one->get().colliders[i];
                auto& c2 = sphere_two->get().colliders[j];
                _contact_buffer.insert(c1.accept(c2));
            }
        }
    }

    return _contact_buffer.empty() ? physics_models::contact::None : *_contact_buffer.begin();
}

