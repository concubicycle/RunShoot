//
// Created by sava on 10/20/19.
//

#ifndef __FREEFLY_CONTROLLER_COMPONENT_HPP_
#define __FREEFLY_CONTROLLER_COMPONENT_HPP_

#include <ecs/component.hpp>
#include <ecs/entity.hpp>

#include <util/string_table.hpp>

class freefly_controller_component : public ecs::component<freefly_controller_component>
{
public:
    float run_speed = 5.f;
    float walk_speed = 3.f;
};

void load_character_controller(const json& j, ecs::entity& e, string_table& hashes);


#endif //__FREEFLY_CONTROLLER_COMPONENT_HPP_
