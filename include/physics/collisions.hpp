//
// Created by sava on 11/1/19.
//

#ifndef __COLLISIONS_HPP_
#define __COLLISIONS_HPP_

#include <functional>
#include <unordered_map>

#include <ecs/ecs_types.hpp>
#include <ecs/entity.hpp>

#include <physics/contact.hpp>


namespace physics
{

    class collisions
    {
    public:
        collisions();

        void reset();

        contact check_collision_and_generate_contact(ecs::entity &one, ecs::entity &two);

    private:
        static const component_shift collider_components[];

        /*
         * We need a map like [collider_a][collider_b] => intersect function.
         *
         * We can use a bitset indicating the collider component types for the key.
         * [component_bitset]=>intersect test.
         *
         * but then how to handle multiple colliders per entity?
         */

        static std::unordered_map<component_shift, std::function<contact(ecs::entity &,
                                                                         ecs::entity &)>> _intersection_tests;


    };

}

#endif //__COLLISIONS_HPP_
