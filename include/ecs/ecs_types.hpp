//
// Created by sava on 9/21/19.
//

#ifndef ECS_DEV_ECS_TYPES_H
#define ECS_DEV_ECS_TYPES_H

#include <cstdint>
#include <bitset>

#define ECS_MAX_COMPONENT_TYPES 64

namespace ecs
{
typedef std::bitset<ECS_MAX_COMPONENT_TYPES> component_bitset;

typedef std::uint32_t entity_id;
} // namespace ecs

#endif //ECS_DEV_ECS_TYPES_H
