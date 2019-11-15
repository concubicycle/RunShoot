#ifndef __PLAYER_CONTROLLER_HPP_
#define __PLAYER_CONTROLLER_HPP_

#include <iostream>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <core/behavior.hpp>
#include <physics/entity_contact.hpp>
#include <physics/collider_iterator.hpp>

#include <util/debounce.hpp>

#include "components/player_controller_component.hpp"



class player_controller : public core::behavior
{
public:
    player_controller(events::event_exchange& events) :
        behavior(events),
        _jump_debounce(std::chrono::duration<float>(0.7), jump),
        _right_turn(glm::rotate(-glm::half_pi<float>(), glm::vec3(0, 1, 0))),
        _left_turn(glm::rotate(glm::half_pi<float>(), glm::vec3(0, 1, 0)))
    {
        _collision_listener_id = _events.subscribe<const physics::entity_contact&, float>(
            events::collision,
            std::function<void(const physics::entity_contact&, float)>(on_collision));

        // remove this
        events.subscribe<ecs::entity&>(
            events::delay_entity_jump,
            std::function<void(ecs::entity&)>(on_jump_delay));
    }

    // this is temporary, only to show delayed events
    static void on_jump_delay(ecs::entity& e)
    {
        jump(e);
    }


    ~player_controller() override
    {
        _events.unsubscribe(events::collision, _collision_listener_id);
    }

    [[nodiscard]] component_bitset required_components()  const override { return player_components(); }

protected:
    void update_single(ecs::entity& e, core::behavior_context &ctx) override;
    void on_entity_created(ecs::entity& e) override;


private:
    listener_id _collision_listener_id;
    debounce<ecs::entity&> _jump_debounce;
    glm::mat4 _right_turn;
    glm::mat4 _left_turn;



    void update_running(ecs::entity& e, player_controller_component& player, core::behavior_context &ctx);
    void update_airborne(ecs::entity &e, player_controller_component &comp, core::behavior_context &ctx);
    void update_turning(ecs::entity &e, player_controller_component &player, core::behavior_context &ctx);

    static void on_collision(const physics::entity_contact& collision, float dt);
    static void move_component_positions(ecs::entity& e, glm::vec3 displacement);
    void update_player_look(ecs::entity& e, core::input_manager& input, float frame_time);
    void update_turn_look(ecs::entity& e);

    void integrate(ecs::entity& e, ecs::rigid_body_component& rb, float frame_time);


    static void resolve_collision(const physics::entity_contact& collision,
                                  ecs::entity& player_entity,
                                  player_controller_component& player,
                                  float dt);

    static component_bitset player_components()
    {
        return
            ecs::transform_component::archetype_bit |
            player_controller_component::archetype_bit |
            ecs::transform_component::archetype_bit |
            ecs::camera_component::archetype_bit;
    }

    static void jump(ecs::entity& e)
    {
        auto& rb = e.get_component<ecs::rigid_body_component>();
        auto& player = e.get_component<player_controller_component>();

        rb.force += glm::vec3(0, player.jump_force, 0);
        player.state = player_state::airborne;
    }
};


#endif //__PLAYER_CONTROLLER_HPP_
