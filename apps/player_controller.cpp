//
// Created by sava on 10/30/19.
//

#include <ctime>
#include <cstdlib>

#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/random.hpp>
#include <sound_wrapper/sound_emitter_component.hpp>

#include "player_controller.hpp"
#include "components/turn_trigger_component.hpp"
#include "components/segment_component.hpp"
#include "runshoot_event.hpp"
#include "components/drone_controller_component.hpp"
#include "components/sound_emitter_component.hpp"
#include "components/death_trigger_component.hpp"
#include "components/shooter_controller_component.hpp"
#include <ecs/behavior.hpp>


player_controller::player_controller(events::event_exchange &events) :
    behavior(events),
    _jump_debounce(std::chrono::duration<float>(0.4f), jump),
    _turn_debounce(std::chrono::duration<float>(1.25f), adjust_turn_counter),
    _footstep_debounce(std::chrono::duration<float>(0.1f), [](ecs::entity& e) {
        auto& sound_emitter = e.get_component<sound::sound_emitter_component>();
        auto& player = e.get_component<player_controller_component>();
        sound_emitter.set_sound_state(player.footstep_sound_index, sound::playing);
        player.footstep_sound_index = (player.footstep_sound_index + 1) % 5 + 1;
    }),
    _right_turn(glm::rotate(-glm::half_pi<float>(), glm::vec3(0, 1, 0))),
    _left_turn(glm::rotate(glm::half_pi<float>(), glm::vec3(0, 1, 0)))
{
    _collision_listener_id = _events.subscribe<const physics::entity_contact &, float>(
        events::collision,
        std::function<void(const physics::entity_contact &, float)>(
            [this](const physics::entity_contact &collision, float dt) {
                on_collision(collision, dt);
            }));
}

player_controller::~player_controller()
{
    _events.unsubscribe(events::collision, _collision_listener_id);
}


void player_controller::update_single(ecs::entity &e, core::behavior_context &ctx)
{
    auto &player = e.get_component<player_controller_component>();
    auto &t = e.get_component<ecs::transform_component>();
    auto &c = e.get_component<ecs::camera_component>();
    auto &input = ctx.input;

    apply_acceleration(e, ctx);

    if (player.state != dying && player.state != dead)
    {
        if (ctx.input.was_mouse_clicked(GLFW_MOUSE_BUTTON_1))
            shoot(e, ctx);

        float color_multiplier = c.get_float("color_multiplier");
        if (color_multiplier != 1)
        {
            float d = 1 - color_multiplier;
            color_multiplier += d * ctx.time.smoothed_delta_secs();
            c.set_float("color_multiplier", color_multiplier);
        }

        player.time_to_flash_out -= ctx.time.smoothed_delta_secs();
        if (player.time_to_flash_out < 0)
        {
            e.graph_node->traverse([&e](ecs::entity &child_e, glm::mat4 &transform) {
                if (e.id() == child_e.id()) return;
                child_e.set_active(false);
            });
        }
    }

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
        case sliding:
            update_sliding(e, player, ctx);
            break;
        case dying:
            update_dying(e, player, ctx);
            break;
        case dead:
            _events.invoke<const std::string&>(events::scene_change, "./assets/scenes/runshoot_gameplay.json");
            break;
    }
}

void player_controller::on_entity_created(ecs::entity &e)
{
    auto &player = e.get_component<player_controller_component>();
    auto &c = e.get_component<ecs::camera_component>();
    auto &t = e.get_component<ecs::transform_component>();
    auto &rb = e.get_component<ecs::rigid_body_component>();

    player.to_camera = c.position - t.pos;
    rb.velocity = player.direction * player.run_speed;

    c.pitch = 0;//glm::pi<float>()/4.f;
    c.roll = 0;
    c.yaw = 0;

    auto hit = std::function([&e](ecs::entity &shooter_e) {
        auto &player = e.get_component<player_controller_component>();
        player.state = dying;
    });

    auto hit_listener_id = _events.subscribe<runshoot_event, ecs::entity &>(runshoot_event::shooter_hit, hit);

    auto forget = std::function([this, &e, hit_listener_id](ecs::entity &destroyed_e) {
        if (destroyed_e.id() == e.id())
        {
            _events.unsubscribe(runshoot_event::shooter_hit, hit_listener_id);
        }
    });

    _events.subscribe<ecs::entity &>(events::entity_destroyed, forget);
}

void player_controller::update_running(ecs::entity &e, player_controller_component &player, core::behavior_context &ctx)
{
    auto frame_time = ctx.time.smoothed_delta_secs();
    auto &rb = e.get_component<ecs::rigid_body_component>();
    auto &t = e.get_component<ecs::transform_component>();
    auto &cam = e.get_component<ecs::camera_component>();
    auto &sound = e.get_component<sound::sound_emitter_component>();

    if (ctx.input.was_key_pressed(GLFW_KEY_SPACE))
    {
        _jump_debounce(e, ctx);
        player.state = airborne;
    } else if (ctx.input.was_key_pressed(GLFW_KEY_D) && player.turn_counter < 1)
    {
        player.current_turn_duration = 0;
        player.target_direction = glm::round(_right_turn * glm::vec4(player.direction, 1.f));
        player.previous_direction = player.direction;
        player.state = turning;
        player.previous_yaw = cam.yaw;
        player.target_yaw = cam.yaw + glm::half_pi<float>();
        player.turn_dir = right;
        player.turn_counter += player.turn_dir;
    } else if (ctx.input.was_key_pressed(GLFW_KEY_A) && player.turn_counter > -1)
    {
        player.current_turn_duration = 0;
        player.target_direction = glm::round(_left_turn * glm::vec4(player.direction, 1.f));
        player.previous_direction = player.direction;
        player.state = turning;
        player.previous_yaw = cam.yaw;
        player.target_yaw = cam.yaw - glm::half_pi<float>();
        player.turn_dir = left;
        player.turn_counter += player.turn_dir;
    } else if (ctx.input.was_key_pressed(GLFW_KEY_S) || ctx.input.was_key_pressed(GLFW_KEY_LEFT_CONTROL))
    {
        player.state = sliding;
        player.original_to_camera = player.to_camera;
        player.target_to_camera = player.to_camera + player.slide_to_camera_offset;
        t.scale_y = 0.5f;
        sound.emitter_sounds[6].state = sound::playing;

        if (rb.velocity.y > 0)
            rb.velocity.y = 0;
    }

    integrate(e, rb, frame_time);
    update_player_look(e, ctx.input, frame_time);

    player.time_since_collision += frame_time;
    player.time_since_grounded += frame_time;

    if (player.time_since_grounded > 0.48f)
    {
        player.state = player_state::airborne;
    }
}

void player_controller::update_airborne(ecs::entity &e, player_controller_component &comp, core::behavior_context &ctx)
{
    auto frame_time = ctx.time.smoothed_delta_secs();
    auto &rb = e.get_component<ecs::rigid_body_component>();
    auto &t = e.get_component<ecs::transform_component>();

    integrate(e, rb, frame_time);
    update_player_look(e, ctx.input, ctx.time.smoothed_delta_secs());

    comp.time_since_collision += frame_time;
    comp.time_since_grounded += frame_time;

    if (comp.time_since_grounded < 0.2f)
    {
        comp.state = player_state::running;
    }

    if (t.pos.y < comp.kill_y)
    {
        comp.state = dying;
    }
}

void player_controller::update_turning(ecs::entity &e, player_controller_component &player, core::behavior_context &ctx)
{
    auto &rb = e.get_component<ecs::rigid_body_component>();
    float frame_time = ctx.time.smoothed_delta_secs();

    if (glm::all(glm::epsilonEqual(player.direction, player.target_direction, glm::epsilon<float>())))
    {
        player.direction = player.target_direction;
        player.state = running;
        rb.velocity = player.direction * glm::length(rb.velocity);
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

void player_controller::update_sliding(ecs::entity &e, player_controller_component &player, core::behavior_context &ctx)
{
    auto frame_time = ctx.time.smoothed_delta_secs();
    auto &rb = e.get_component<ecs::rigid_body_component>();
    auto &t = e.get_component<ecs::transform_component>();
    auto &cam = e.get_component<ecs::camera_component>();

    player.current_slide_duration += frame_time;

    float slide_t = player.slide_t();

    if (slide_t < player.down_t)
    {
        auto offset = slide_t / player.down_t * player.slide_to_camera_offset;
        player.to_camera = player.original_to_camera + offset;
    }

    if (slide_t > player.up_t)
    {
        auto current_up_t = glm::length(slide_t - player.up_t) / glm::length(1.f - player.up_t);
        auto offset = (1.f - current_up_t) * player.slide_to_camera_offset;
        player.to_camera = player.original_to_camera + offset;

    }

    if (player.current_slide_duration > player.total_slide_duration)
    {
        player.state = running;
        player.to_camera = player.original_to_camera;
        player.current_slide_duration = 0.f;

        t.scale_y = 1.f;
    }

    integrate(e, rb, frame_time);
    update_player_look(e, ctx.input, ctx.time.smoothed_delta_secs());
}

void player_controller::update_dying(ecs::entity &e, player_controller_component &player, core::behavior_context &ctx)
{
    auto frame_time = ctx.time.smoothed_delta_secs();
    auto &rb = e.get_component<ecs::rigid_body_component>();
    auto &c = e.get_component<ecs::camera_component>();

    player.current_fadeout_time -= frame_time;

    auto scale = player.current_fadeout_time / player.fadeout_time;
    rb.velocity *= 5.f * frame_time;
    c.set_float("color_multiplier", scale);

    integrate(e, rb, frame_time);

    if (player.current_fadeout_time < 0)
        player.state = dead;
}


void player_controller::update_turn_look(ecs::entity &e)
{
    if (_starting_mouse_lock > 0)
    {
        _starting_mouse_lock--;
        return;
    }

    auto &c = e.get_component<ecs::camera_component>();
    auto &player = e.get_component<player_controller_component>();

    auto t_turn = player.turn_t();

    c.yaw = player.previous_yaw + t_turn * (player.target_yaw - player.previous_yaw);
    c.roll = 0;

    auto half_pi = glm::pi<float>() / 2.f;
    if (c.pitch > half_pi)
        c.pitch = half_pi;
    if (c.pitch < -half_pi)
        c.pitch = -half_pi;
}

void player_controller::update_player_look(ecs::entity &e, core::input_manager &input, float frame_time)
{
    if (_starting_mouse_lock > 0)
    {
        _starting_mouse_lock--;
        return;
    }

    auto &c = e.get_component<ecs::camera_component>();
    auto mouse_delta = input.mouse_delta();

    c.pitch -= mouse_delta.y * 0.1f * frame_time;
    c.yaw += mouse_delta.x * 0.1f * frame_time;
    c.roll = 0;

    auto half_pi = glm::pi<float>() / 2.f;
    if (c.pitch > half_pi)
        c.pitch = half_pi;
    if (c.pitch < -half_pi)
        c.pitch = -half_pi;
}

void player_controller::resolve_collision(
    const physics::entity_contact &collision,
    ecs::entity &player_entity,
    player_controller_component &player,
    float dt)
{
    auto &rb = player_entity.get_component<ecs::rigid_body_component>();
    auto n = collision.contact().collision_axis();
    auto &sound_emitter = player_entity.get_component<sound::sound_emitter_component>();

    if (collision.contact().is_trigger_contact())
    {
        resolve_trigger_collision(collision, player_entity, player);
        return;
    }

    auto &other_entity = collision.the_other(player_entity.id());

    if (other_entity.has<death_trigger_component>())
    {
        player.state = dying;
    }

    auto drone_opt = other_entity.get_component_opt<drone_controller_component>();
    auto is_dead_drone = drone_opt &&
                         drone_opt->get().state == drone_controller_component::exploding;
    if (is_dead_drone)
    {
        auto &drone_rb = other_entity.get_component<ecs::rigid_body_component>();
        drone_rb.velocity += player.direction * 200.f + glm::vec3(0.f, 1.f, 0.f) * 50.f;
        return;
    }

    auto intersecting = collision.contact().time() == physics_models::contact::Intersecting;

    if (!intersecting)
    {
        auto res_t = std::max(dt - 0.001f, 0.f);
        auto move = rb.velocity * (res_t);

        auto n_norm = glm::normalize(collision.collision_axis());
        rb.velocity -= n_norm * glm::dot(n, rb.velocity);
        rb.force.x = rb.force.y = rb.force.z = 0.f;
    }
        //else if (player.time_since_collision < 0.05f && n.y == 0)
        //{
        // resolving xz plane collisions can jerk the player around.
        // probably a problem with collision system - not sure if this
        // would happen with a non-kinematic object. either way, we can
        // cheat here.
        //rb.force += glm::vec3(0, 20, 0);
        //}
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
            _footstep_debounce(player_entity);
        }

        player.time_since_grounded = 0;
    }

    if (other_entity.has<segment_component>())
    {
        if (player.current_segment_id != other_entity.id())
        {
            player.seg_clear_count++;
            player.current_segment_id = other_entity.id();
        }

        if (player.seg_clear_count > player.segments_to_clear)
        {
            player.seg_clear_count = 0;
            player.segments_to_clear = 0;
            _events.invoke<runshoot_event, ecs::entity &>(segment_cleared, other_entity);
        }
    }
}

void player_controller::resolve_trigger_collision(
    const physics::entity_contact &collision,
    ecs::entity &player_entity,
    player_controller_component &player)
{
    auto &trigger_e = collision.the_other(player_entity.id());
    auto trigger_component = trigger_e.get_component_opt<turn_trigger_component>();

    if (trigger_component)
        _turn_debounce(player_entity, trigger_component->get().direction);
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
    auto &player = e.get_component<player_controller_component>();
    auto &rb = e.get_component<ecs::rigid_body_component>();
    auto &t = e.get_component<ecs::transform_component>();
    auto &c = e.get_component<ecs::camera_component>();
    t.pos += displacement;
    //rb.position = t.pos;
    c.position = t.pos + player.to_camera;
    rb.position = t.pos;
}

void player_controller::integrate(ecs::entity &e, ecs::rigid_body_component &rb, float frame_time)
{
    rb.force += rb.gravity;
    rb.acceleration = rb.force;
    rb.velocity += rb.acceleration;


    rb.force = glm::vec3(0);
    move_component_positions(e, rb.velocity * frame_time);
}

void player_controller::jump(ecs::entity &e, core::behavior_context &ctx)
{
    auto &rb = e.get_component<ecs::rigid_body_component>();
    auto &player = e.get_component<player_controller_component>();
    auto &t = e.get_component<ecs::transform_component>();

    bool nothing_under = true;
    ctx.physics.raycast({t.pos - 4.f * player.direction, glm::vec3(0.f, -1.f, 0.f)}, [&nothing_under](ecs::entity &e) {
        nothing_under = false;
    });

    if (!nothing_under)
    {
        rb.force += glm::vec3(0, player.jump_force, 0) - rb.force;
        player.state = player_state::airborne;
    }
}

void player_controller::adjust_turn_counter(ecs::entity &e, turn_direction direction)
{
    auto &player = e.get_component<player_controller_component>();
    player.turn_counter -= direction; // naive implementation
}

component_bitset player_controller::required_components() const
{
    return
        ecs::transform_component::archetype_bit |
        player_controller_component::archetype_bit |
        ecs::transform_component::archetype_bit |
        ecs::camera_component::archetype_bit;
}

void player_controller::apply_acceleration(ecs::entity &e, core::behavior_context ctx)
{
    auto &rb = e.get_component<ecs::rigid_body_component>();
    auto &player = e.get_component<player_controller_component>();
    auto &cam = e.get_component<ecs::camera_component>();

    rb.velocity *= 1.f + player.run_acceleration * ctx.time.smoothed_delta_secs();

    float speed_sq = glm::length2(rb.velocity);

    if (glm::epsilonEqual(speed_sq, 0.f, glm::epsilon<float>()))
        rb.velocity = player.direction;

    if (speed_sq < player.run_speed * player.run_speed)
        rb.velocity *= 1 + (0.4 * ctx.time.smoothed_delta_secs());

    cam.pitch += player.recoil_acceleration.y * ctx.time.smoothed_delta_secs();
    cam.yaw += player.recoil_acceleration.x * ctx.time.smoothed_delta_secs();


    if (player.recoil_acceleration.x != 0 || player.recoil_acceleration.y != 0)
    {
        auto mu = 0.85f;
        auto N = 17.f;
        auto friction_d = -glm::normalize(player.recoil_acceleration);
        auto friction_m = mu * N;
        auto f = friction_d * friction_m;
        auto old_a = player.recoil_acceleration;
        player.recoil_acceleration += f * ctx.time.smoothed_delta_secs();
        if (glm::dot(old_a, player.recoil_acceleration) < 0)
            player.recoil_acceleration = glm::vec2(0.f);
    }
}

void player_controller::shoot(ecs::entity &e, core::behavior_context ctx)
{
    auto &cam = e.get_component<ecs::camera_component>();
    auto &player = e.get_component<player_controller_component>();
    auto &sound = e.get_component<sound::sound_emitter_component>();

    physics_models::ray ray = {cam.position, cam.fwd()};

    ctx.physics.raycast(ray, [](ecs::entity &hit_entity) {
        if (hit_entity.has<drone_controller_component>())
        {
            auto &drone = hit_entity.get_component<drone_controller_component>();
            drone.state = drone_controller_component::exploding;
        }
        if (hit_entity.has<shooter_controller_component>())
        {
            auto &shooter = hit_entity.get_component<shooter_controller_component>();
            shooter.state = shooter_controller_component::dying;
        }
    }, drone_controller_component::archetype_bit | shooter_controller_component::archetype_bit);

    player.recoil_acceleration = {
        glm::linearRand(-0.8f, 0.8f),
        glm::linearRand(1.2f, 1.8f)
    };

    sound.set_sound_state(0, sound::playing);
    sound.emitter_sounds[0].volume = 0.25f;

    e.graph_node->traverse([&e](ecs::entity &child_e, glm::mat4 &transform) {
        if (e.id() == child_e.id()) return;
        child_e.set_active(true);
    });

    player.time_to_flash_out = player.gunshot_flash_time;
}

