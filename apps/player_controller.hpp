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
    explicit player_controller(events::event_exchange &events);

    ~player_controller() override;

    [[nodiscard]] component_bitset required_components() const override;

protected:
    void update_single(ecs::entity &e, core::behavior_context &ctx) override;

    void on_entity_created(ecs::entity &e) override;


private:
    listener_id _collision_listener_id;
    listener_id _shooter_hit_listener_id{};
    listener_id _shooter_miss_listener_id{};

    debounce<ecs::entity &, core::behavior_context&> _jump_debounce;
    debounce<ecs::entity &, turn_direction> _turn_debounce;
    glm::mat4 _right_turn;
    glm::mat4 _left_turn;

    void update_running(ecs::entity &e, player_controller_component &player, core::behavior_context &ctx);

    void on_collision(const physics::entity_contact &collision, float dt);

    void resolve_collision(
        const physics::entity_contact &collision,
        ecs::entity &player_entity,
        player_controller_component &player,
        float dt);

    void resolve_trigger_collision(
        const physics::entity_contact &collision,
        ecs::entity &player_entity,
        player_controller_component &player);

    static void update_airborne(ecs::entity &e, player_controller_component &comp, core::behavior_context &ctx);

    static void update_turning(ecs::entity &e, player_controller_component &player, core::behavior_context &ctx);

    static void update_sliding(ecs::entity &e, player_controller_component &player, core::behavior_context &ctx);

    static void update_dying(ecs::entity &e, player_controller_component &player, core::behavior_context &ctx);


    static void update_player_look(ecs::entity &e, core::input_manager &input, float frame_time);

    static void update_turn_look(ecs::entity &e);

    static void integrate(ecs::entity &e, ecs::rigid_body_component &rb, float frame_time);

    static void move_component_positions(ecs::entity &e, glm::vec3 displacement);

    static void jump(ecs::entity &e, core::behavior_context& ctx);

    static void adjust_turn_counter(ecs::entity &e, turn_direction direction);

    static void apply_acceleration(ecs::entity &e, core::behavior_context ctx);

    static void shoot(ecs::entity &e, core::behavior_context ctx);
};


#endif //__PLAYER_CONTROLLER_HPP_
