//
// Created by sava on 10/30/19.
//

#include "player_controller.hpp"
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/matrix_interpolation.hpp>



void player_controller::update_single(ecs::entity &e, core::behavior_context &ctx)
{
    auto& player = e.get_component<player_controller_component>();
    auto& t = e.get_component<ecs::transform_component>();
    auto& c = e.get_component<ecs::camera_component>();

    auto& input = ctx.input;

    switch (player.state)
    {
        case running:
            update_running(e, player, ctx);
            break;
        case airborne:
            update_airborne(e, player, ctx);
            break;
        case turning:
            update_turning(e, player, ctx);
            break;
    }
}

void player_controller::on_entity_created(ecs::entity &e)
{
    auto& player = e.get_component<player_controller_component>();
    auto& c = e.get_component<ecs::camera_component>();
    auto& t = e.get_component<ecs::transform_component>();
    auto& rb = e.get_component<ecs::rigid_body_component>();

    player.to_camera = c.position - t.pos;
    rb.velocity = player.direction * player.run_speed;
}

void player_controller::update_running(ecs::entity &e, player_controller_component &player, core::behavior_context &ctx)
{
    auto frame_time = ctx.time.smoothed_delta_secs();
    auto& rb = e.get_component<ecs::rigid_body_component>();
    auto& t = e.get_component<ecs::transform_component>();
    auto& cam = e.get_component<ecs::camera_component>();

    if (ctx.input.was_key_pressed(GLFW_KEY_SPACE))
    {
        _jump_debounce(e);
        player.state = airborne;
    }
    else if (ctx.input.was_key_pressed(GLFW_KEY_D) && player.turn_counter < 1)
    {
        player.current_turn_duration = 0;
        player.target_direction = _right_turn * glm::vec4(player.direction, 1.f);
        player.previous_direction = player.direction;
        player.state = turning;
        player.previous_yaw = cam.yaw;
        player.target_yaw = cam.yaw += glm::half_pi<float>();
        player.turn_dir = right;
        player.turn_counter += player.turn_dir;
    }
    else if (ctx.input.was_key_pressed(GLFW_KEY_A) && player.turn_counter > -1)
    {
        player.current_turn_duration = 0;
        player.target_direction = _left_turn * glm::vec4(player.direction, 1.f);
        player.previous_direction = player.direction;
        player.state = turning;
        player.previous_yaw = cam.yaw;
        player.target_yaw = cam.yaw -= glm::half_pi<float>();
        player.turn_dir = left;
        player.turn_counter += player.turn_dir;
    }

    integrate(e, rb, frame_time);
    update_player_look(e, ctx.input, frame_time);

    player.time_since_collision += frame_time;
    player.time_since_grounded += frame_time;

    if (player.time_since_grounded > 0.5f)
    {
        player.state = player_state::airborne;
    }
}

void player_controller::update_airborne(ecs::entity &e, player_controller_component &comp, core::behavior_context &ctx)
{
    auto frame_time = ctx.time.smoothed_delta_secs();
    auto& rb = e.get_component<ecs::rigid_body_component>();
    auto& t = e.get_component<ecs::transform_component>();

    integrate(e, rb, frame_time);
    update_player_look(e, ctx.input, ctx.time.smoothed_delta_secs());

    comp.time_since_collision += frame_time;
    comp.time_since_grounded += frame_time;

    if (comp.time_since_grounded < 0.2f)
    {
        comp.state = player_state::running;
    }
}

void player_controller::update_turning(ecs::entity &e, player_controller_component &player, core::behavior_context &ctx)
{
    auto& rb = e.get_component<ecs::rigid_body_component>();
    float frame_time = ctx.time.smoothed_delta_secs();

    if (glm::all(glm::epsilonEqual(player.direction, player.target_direction, glm::epsilon<float>())))
    {
        player.direction = glm::round(player.direction);
        player.target_direction = glm::round(player.target_direction);
        player.state = running;
    } else
    {
        player.current_turn_duration += frame_time;

        auto t_turn = player.turn_t();
        if (t_turn > 1)
        {
            player.direction = player.target_direction;
        } else
        {
            auto interp = player.target_direction - player.previous_direction;
            player.direction = glm::normalize(player.previous_direction + t_turn * interp);
            rb.velocity = player.direction * glm::length(rb.velocity);
        }

        player.current_turn_duration += ctx.time.smoothed_delta_secs();
    }

    integrate(e, rb, frame_time);
    update_turn_look(e);
}

void player_controller::update_turn_look(ecs::entity& e)
{
    auto& t = e.get_component<ecs::transform_component>();
    auto& c = e.get_component<ecs::camera_component>();
    auto& player = e.get_component<player_controller_component>();

    auto t_turn = player.turn_t();

    t.yaw = player.previous_yaw + t_turn * (player.target_yaw - player.previous_yaw);
    t.roll = 0;

    auto half_pi = glm::pi<float>() / 2.f;
    if (t.pitch > half_pi)
        t.pitch = half_pi;
    if (t.pitch < -half_pi)
        t.pitch = -half_pi;

    c.pitch = t.pitch;
    c.yaw = t.yaw;
    c.roll = t.roll;
}

void player_controller::update_player_look(ecs::entity& e, core::input_manager& input, float frame_time)
{
    auto& t = e.get_component<ecs::transform_component>();
    auto& c = e.get_component<ecs::camera_component>();
    auto mouse_delta = input.mouse_delta();

    t.pitch -= mouse_delta.y * 0.1f * frame_time;
    t.yaw += mouse_delta.x * 0.1f * frame_time;
    t.roll = 0;

    auto half_pi = glm::pi<float>() / 2.f;
    if (t.pitch > half_pi)
        t.pitch = half_pi;
    if (t.pitch < -half_pi)
        t.pitch = -half_pi;

    c.pitch = t.pitch;
    c.yaw = t.yaw;
    c.roll = t.roll;
}




void player_controller::resolve_collision(
    const physics::entity_contact &collision,
    ecs::entity &player_entity,
    player_controller_component &player,
    float dt)
{
    auto& rb = player_entity.get_component<ecs::rigid_body_component>();
    auto n = collision.contact().collision_axis();



    auto intersecting = collision.contact().time() == physics_models::contact::Intersecting;

    if (!intersecting)
    {
        auto res_t = std::max(dt - 0.001f, 0.f);
        auto move = rb.velocity * (res_t);

        auto n_norm = glm::normalize(collision.collision_axis());
        rb.velocity -= n_norm * glm::dot(n, rb.velocity);
        rb.force.x = rb.force.y = rb.force.z = 0.f;
    }
    else if (player.time_since_collision < 0.05f && n.y == 0)
    {
        // resolving xz plane collisions can jerk the player around.
        // probably a problem with collision system - not sure if this
        // would happen with a non-kinematic object. either way, we can
        // cheat here.
        rb.force += glm::vec3(0, 20, 0);
    }
    else
    {
        auto move = n * -1.01f;
        rb.velocity -= n * glm::dot(n, rb.velocity);
        move_component_positions(player_entity, move);
    }

    player.time_since_collision = 0;

    if (n.y < 0)
    {
        if (player.time_since_grounded > 0.01f)
        {
            rb.force += glm::vec3(0, player.footstep_force, 0);
        }

        player.time_since_grounded = 0;
    }
}

void player_controller::on_collision(const physics::entity_contact &collision, float dt)
{
    auto one_comp = collision.one().get_component_opt<player_controller_component>();
    auto two_comp = collision.two().get_component_opt<player_controller_component>();
    if (one_comp) resolve_collision(collision, collision.one(), *one_comp, dt);
    if (two_comp) resolve_collision(collision, collision.two(), *two_comp, dt);
}

void player_controller::move_component_positions(ecs::entity &e, glm::vec3 displacement)
{
    auto& player = e.get_component<player_controller_component>();
    auto& rb = e.get_component<ecs::rigid_body_component>();
    auto& t = e.get_component<ecs::transform_component>();
    auto& c = e.get_component<ecs::camera_component>();
    t.pos += displacement;
    rb.position = t.pos;
    c.position = t.pos + player.to_camera;

    physics::collider_iterator it(e);
    physics_models::collider *cursor;
    while (it.end() != (cursor = it.get_next()))
        cursor->set_position(rb.position);
}

void player_controller::integrate(ecs::entity &e, ecs::rigid_body_component &rb, float frame_time)
{
    auto fa = rb.force * rb.mass_inverse();
    rb.force += rb.gravity;
    rb.acceleration = rb.force;
    rb.velocity += rb.acceleration * frame_time;
    rb.force = glm::vec3(0);
    move_component_positions(e, rb.velocity * frame_time);
}

