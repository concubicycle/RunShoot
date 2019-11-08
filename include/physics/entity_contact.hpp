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

        ecs::entity &one() const { return _one.get(); }

        ecs::entity &two() const { return _two.get(); }

        const physics_models::contact &contact() const { return _contact; }

        static bool compare(const entity_contact& a, const entity_contact& b)
        {
            return a.contact().time() < b.contact().time();
        }

        static bool compare_descending(const entity_contact& a, const entity_contact& b)
        {
            return a.contact().time() > b.contact().time();
        }

    private:
        std::reference_wrapper<ecs::entity> _one;
        std::reference_wrapper<ecs::entity> _two;
        physics_models::contact _contact;
    };

}

#endif //__ENTITY_CONTACT_HPP_
