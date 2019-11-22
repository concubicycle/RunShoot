//
// Created by sava on 11/21/19.
//

#include <asset/prototype_spawner.hpp>
#include "segment_spawner.hpp"
#include "runshoot_event.hpp"
#include "components/segment_component.hpp"
#include <time.h>       /* time */


segment_spawner::segment_spawner(events::event_exchange &events) :
    behavior(events),
    _right_turn(glm::rotate(-glm::half_pi<float>(), glm::vec3(0, 1, 0))),
    _left_turn(glm::rotate(glm::half_pi<float>(), glm::vec3(0, 1, 0)))
{
    _segment_cleared_listener = events.subscribe(segment_cleared,
        std::function<void(ecs::entity&)>([this](ecs::entity& e) {
            auto& seg_to_remove = _segments.front().get();
            _segments.pop();

            // TODO: free seg_to_remove
        }));

    /* initialize random seed: */
    srand (time(NULL));
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

}

void segment_spawner::spawn_segment(ecs::entity &e, core::behavior_context &ctx)
{
    auto& segment_spawn = e.get_component<segment_spawner_component>();

    int seg_index = rand() % _segment_prototype_paths.size();
    auto& seg_e = ctx.current_scene.load_prototype(_segment_prototype_paths[seg_index]);

    auto& t = seg_e.get_component<ecs::transform_component>();
    auto& seg_component = seg_e.get_component<segment_component>();

    auto pos = segment_spawn.current_position + segment_spawn.jump_distance * segment_spawn.current_direction;
    auto rot = segment_spawn.rotation;

    if (seg_component.turn == left)
        segment_spawn.rotation = _left_turn * segment_spawn.rotation;
    else if (seg_component.turn == right)
        segment_spawn.rotation = _right_turn * segment_spawn.rotation;





    _segments.push(seg_e);
}
