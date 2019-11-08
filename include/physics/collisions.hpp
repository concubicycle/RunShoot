//
// Created by sava on 11/1/19.
//

#ifndef __COLLISIONS_HPP_
#define __COLLISIONS_HPP_

#include <set>
#include <functional>
#include <unordered_map>

#include <ecs/ecs_types.hpp>
#include <ecs/entity.hpp>

#include <physics_models/contact.hpp>
#include "entity_contact.hpp"


namespace physics
{

    class collisions
    {
    public:
        physics_models::contact check_collision_and_generate_contact(
            ecs::entity &one,
            ecs::entity &two,
            float frame_time);

    private:
        std::set<physics_models::contact, physics_models::contact_compare> _contact_buffer;
    };

}

#endif //__COLLISIONS_HPP_
