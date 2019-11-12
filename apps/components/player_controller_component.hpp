//
// Created by sava on 11/10/19.
//

#ifndef __PLAYER_CONTROLLER_COMPONENT_HPP_
#define __PLAYER_CONTROLLER_COMPONENT_HPP_

#include <ecs/component.hpp>
#include <ecs/entity.hpp>
#include <util/string_table.hpp>

enum player_state
{
    running,
    airborne,
    dying,
    dead
};

class player_controller_component : public ecs::component<player_controller_component>
{
public:
    glm::vec3 direction { 0.f, 0.f, -1.f};
    glm::vec3 to_camera {0};
    float run_speed {5.f};
    float time_since_collision {0};
    float time_since_grounded {0};
    player_state state {running};
};

void load_player_controller(const json& j, ecs::entity& e, string_table& hashes);


#endif //__PLAYER_CONTROLLER_COMPONENT_HPP_
