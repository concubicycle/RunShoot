//
// Created by sava on 11/21/19.
//

#include <asset/prototype_spawner.hpp>
#include "segment_spawner.hpp"
#include "runshoot_event.hpp"
#include "components/segment_component.hpp"
#include <ctime>


segment_spawner::segment_spawner(events::event_exchange &events) :
    behavior(events),
    _left_turn(glm::rotate(glm::half_pi<float>(), glm::vec3(0, 1, 0))),
    _right_turn(glm::rotate(-glm::half_pi<float>(), glm::vec3(0, 1, 0))),
    _spawn_new_segment(std::chrono::duration<float>(1.75), [this](ecs::entity& e) {
        spawn_new_segment(e);
    })
{
    _segment_cleared_listener =
        events.subscribe(segment_cleared, std::function<void(ecs::entity&)>(_spawn_new_segment));
}

segment_spawner::~segment_spawner()
{
    _events.unsubscribe(segment_cleared, _segment_cleared_listener);
}


void segment_spawner::update_single(ecs::entity &e, core::behavior_context &ctx)
{
    spawn_enough_segments(e, ctx);
}

void segment_spawner::spawn_enough_segments(ecs::entity &e, core::behavior_context &ctx)
{
    auto& segment_spawn = e.get_component<segment_spawner_component>();

    while (_segments.size() < segment_spawn.num_initial_segments)
        spawn_segment(e, ctx);
}

void segment_spawner::spawn_segment(ecs::entity &e, core::behavior_context &ctx)
{
    auto& segment_spawn = e.get_component<segment_spawner_component>();

    srand (time(nullptr));
    int seg_index_var = rand() % 100;
    int freq_int = segment_spawn.turn_frequency * 100.f;
    int seg_index = seg_index_var > freq_int ? 0 : 1;
    auto& seg_e = ctx.current_scene.load_prototype(_segment_prototype_paths[seg_index]);

    auto& t = seg_e.get_component<ecs::transform_component>();
    auto& seg_component = seg_e.get_component<segment_component>();

    float jump_distance = _segments.empty() ? 0 : segment_spawn.jump_distance; // spawn first seg under player
    auto pos = segment_spawn.current_position + jump_distance * glm::vec3(segment_spawn.current_direction);

    auto rotate = segment_spawn.rotation;
    auto translate = glm::translate(pos);
    auto total_t = translate * rotate;
    auto seg_offset = rotate * glm::vec4(seg_component.end_position, 0);
    t.set_from_mat4(total_t);

    segment_spawn.current_position = pos + glm::vec3(seg_offset);

    //std::cout<< "Spawned at " << glm::to_string(translate[3]) << std::endl;

    if (seg_component.turn == left)
    {
        segment_spawn.rotation = _left_turn * segment_spawn.rotation;
        segment_spawn.current_direction = _left_turn * segment_spawn.current_direction;
    }
    else if (seg_component.turn == right)
    {
        segment_spawn.rotation = _right_turn * segment_spawn.rotation;
        segment_spawn.current_direction = _right_turn * segment_spawn.current_direction;
    }

    _segments.push(seg_e);
}

void segment_spawner::spawn_new_segment(ecs::entity &current_segment)
{
    auto& seg_to_remove = _segments.front().get();
    _segments.pop();
    remove_entity(seg_to_remove);
}
