//
// Created by sava on 11/7/19.
//

#ifndef __ENTITY_CONTACT_HPP_
#define __ENTITY_CONTACT_HPP_

#include <physics_models/contact.hpp>
#include <ecs/entity.hpp>

namespace physics
{
    class entity_contact
    {
    public:
        entity_contact(ecs::entity& one, ecs::entity& two, physics_models::contact contact) :
            _one(one),
            _two(two),
            _contact(contact)
        {
        }

        [[nodiscard]] ecs::entity &one() const { return _one.get(); }
        [[nodiscard]] ecs::entity &two() const { return _two.get(); }
        [[nodiscard]] const physics_models::contact &contact() const { return _contact; }
        [[nodiscard]] const glm::vec3& collision_axis() const { return _contact.collision_axis(); }
        [[nodiscard]] float time() const { return _contact.time(); }

        void decrement_time(float amount) { _contact.decrement_time(amount); }

        static bool compare(const entity_contact& a, const entity_contact& b)
        {
            return a.time() < b.time();
        }

        static bool compare_descending(const entity_contact& a, const entity_contact& b)
        {
            return a.time() > b.time();
        }

    private:
        std::reference_wrapper<ecs::entity> _one;
        std::reference_wrapper<ecs::entity> _two;
        physics_models::contact _contact;
    };

}

#endif //__ENTITY_CONTACT_HPP_
