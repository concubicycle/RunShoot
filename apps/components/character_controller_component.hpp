//
// Created by sava on 10/20/19.
//

#ifndef __CHARACTER_CONTROLLER_COMPONENT_HPP_
#define __CHARACTER_CONTROLLER_COMPONENT_HPP_

#include <ecs/component.hpp>
#include <ecs/entity.hpp>

class character_controller_component : public ecs::component<character_controller_component>
{
public:
    float run_speed = 5.f;
    float walk_speed = 3.f;
};

void load_character_controller(const json& j, ecs::entity& e);


#endif //__CHARACTER_CONTROLLER_COMPONENT_HPP_
