//
// Created by sava on 10/30/19.
//

#include "player_controller.hpp"


void player_controller::update_single(ecs::entity &e, core::behavior_context &ctx)
{
    auto& player = e.get_component<player_controller_component>();
    auto& t = e.get_component<ecs::transform_component>();
    auto& c = e.get_component<ecs::camera_component>();

    auto& input = ctx.input;
    auto frame_time = ctx.time.smoothed_delta_secs();

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

    switch (player.state)
    {
        case running:
            update_running(e, player, ctx);
            break;
        case airborne:
            update_airborne(e, player, ctx.time.smoothed_delta_secs());
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

void player_controller::update_running(ecs::entity &e, player_controller_component &comp, core::behavior_context &ctx)
{
    auto frame_time = ctx.time.smoothed_delta_secs();
    auto& rb = e.get_component<ecs::rigid_body_component>();
    auto& t = e.get_component<ecs::transform_component>();

    if (ctx.input.was_key_pressed(GLFW_KEY_SPACE))
        _jump_debounce(e);

    auto fa = rb.force * rb.mass_inverse();
    rb.force += rb.gravity;
    rb.acceleration = rb.force;
    rb.velocity += rb.acceleration * frame_time;
    rb.force = glm::vec3(0);
    move_component_positions(e, rb.velocity * frame_time);

    comp.time_since_collision += frame_time;
    comp.time_since_grounded += frame_time;

    if (comp.time_since_grounded > 0.5f)
    {
        comp.state = player_state::airborne;
    }
}

void player_controller::update_airborne(ecs::entity &e, player_controller_component &comp, float frame_time)
{
    auto& rb = e.get_component<ecs::rigid_body_component>();
    auto& t = e.get_component<ecs::transform_component>();

    auto fa = rb.force * rb.mass_inverse();
    rb.force += rb.gravity;
    rb.acceleration = rb.force;
    rb.velocity += rb.acceleration * frame_time;
    rb.force = glm::vec3(0);
    move_component_positions(e, rb.velocity * frame_time);

    comp.time_since_collision += frame_time;
    comp.time_since_grounded += frame_time;

    if (comp.time_since_grounded < 0.2f)
    {
        comp.state = player_state::running;
    }
}



void player_controller::resolve_collision(const physics::entity_contact &collision, ecs::entity &e,
                                          player_controller_component &player, float dt)
{
    auto& rb = e.get_component<ecs::rigid_body_component>();
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
    else if (player.time_since_collision < 0.01f && n.y == 0)
    {
        // resolving xz plane collisions can jerk the player around.
        // probably a problem with collision system - not sure if this
        // would happen with a non-kinematic object. either way, we can
        // cheat here.
        rb.force += glm::vec3(0, 2, 0);
    }
    else
    {
        auto move = n * -1.01f;
        rb.velocity -= n * glm::dot(n, rb.velocity);
        move_component_positions(e, move);
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