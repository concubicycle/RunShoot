#include "ecs/game_object.hpp"

ecs::game_object_id ecs::game_object::id() const
{
    return _id;
}

std::atomic<ecs::game_object_id> ecs::game_object::NextId(0);