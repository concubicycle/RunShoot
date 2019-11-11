#include <ecs/ecs_types.hpp>
#include <physics/collisions.hpp>
#include <physics/collider_iterator.hpp>


physics_models::contact physics::collisions::check_collision_and_generate_contact(
    ecs::entity &one,
    ecs::entity &two)
{
    _contact_buffer.clear();

    auto v0 = one.has<ecs::rigid_body_component>()
              ? one.get_component<ecs::rigid_body_component>().velocity
              : glm::vec3(0);

    auto v1 = two.has<ecs::rigid_body_component>()
              ? two.get_component<ecs::rigid_body_component>().velocity
              : glm::vec3(0);

    auto combined_v = v1 - v0;

    collider_iterator it1(one);
    collider_iterator it2(two);

    physics_models::collider* c1;
    physics_models::collider* c2;

    while (it1.end() != (c1 = it1.get_next()))
    {
        while (it2.end() != (c2 = it2.get_next()))
            _contact_buffer.insert(c1->accept(*c2, combined_v));

        it2.reset();
    }

    return _contact_buffer.empty()
        ? physics_models::contact::None
        : *(_contact_buffer.begin());
}

