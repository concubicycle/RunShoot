//
// Created by sava on 11/30/19.
//

#ifndef __SOUND_LISTENER_COMPONENT_HPP_
#define __SOUND_LISTENER_COMPONENT_HPP_



#include <ecs/ecs_types.hpp>
#include <ecs/component.hpp>

#include <nlohmann/json.hpp>
#include <util/string_table.hpp>
#include <ecs/entity.hpp>


using nlohmann::json;


namespace sound
{
    struct sound_listener_component : ecs::component<sound_listener_component>
    {
        glm::vec3 listener_position;
        glm::vec3 listener_velocity;
        glm::vec3 listener_forward;
        glm::vec3 listener_up;
    };

    void load_sound_listener(const json &j, ecs::entity &e, string_table &hashes);

}


#endif //__SOUND_LISTENER_COMPONENT_HPP_
