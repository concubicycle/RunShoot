//
// Created by sava on 9/21/19.
//

#ifndef ECS_DEV_BASIC_COMPONENTS_H
#define ECS_DEV_BASIC_COMPONENTS_H

#include <ecs/ecs_types.hpp>
#include <glm/mat4x4.hpp>

namespace ecs
{

struct transform_component
{
    static const component_bitset component_bit;

    glm::mat4 transform;
};

struct render_component
{
    static const component_bitset component_bit;

    float a;
    float b;
};

const component_bitset transform_component::component_bit(1 << 0);
const component_bitset render_component::component_bit(1 << 1);

} // namespace ecs

#endif //ECS_DEV_BASIC_COMPONENTS_H
