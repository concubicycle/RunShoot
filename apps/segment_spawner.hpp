//
// Created by sava on 11/21/19.
//

#ifndef __SEGMENT_SPAWNER_HPP_
#define __SEGMENT_SPAWNER_HPP_

#include <queue>
#include <vector>
#include <string>

#include <core/behavior.hpp>
#include <core/behavior_context.hpp>

#include "components/segment_spawner_component.hpp"

class segment_spawner : public core::behavior
{
public:
    explicit segment_spawner(events::event_exchange& events);
    ~segment_spawner() override;

    void update_single(ecs::entity& e, core::behavior_context &ctx) override;

    [[nodiscard]] component_bitset required_components()  const override
    {
        return segment_spawner_component::archetype_bit;
    }

private:
    glm::mat4 _right_turn;
    glm::mat4 _left_turn;

    std::string seg_one_path = "./asset/prototypes/seg1.json";
    std::string seg_two_path = "./asset/prototypes/seg2.json";
    std::vector<std::string> _segment_prototype_paths {
        seg_one_path,
        seg_two_path
    };

    listener_id _segment_cleared_listener;
    std::queue<std::reference_wrapper<ecs::entity>> _segments;

    void spawn_enough_segments(ecs::entity& e, core::behavior_context &ctx);
    void spawn_segment(ecs::entity& e, core::behavior_context &ctx);


};

#endif //__SEGMENT_SPAWNER_HPP_
