//
// Created by sava on 11/1/19.
//

#ifndef __COLLISIONS_HPP_
#define __COLLISIONS_HPP_

#include <set>
#include <vector>
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
        const std::uint16_t ContactBufferSize = 64;

    public:
        collisions();

        physics_models::contact check_collision_and_generate_contact(ecs::entity &one, ecs::entity &two);

    private:
        std::vector<physics_models::contact> _contact_buffer;
    };

}

#endif //__COLLISIONS_HPP_
