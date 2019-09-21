#ifndef __GAME_OBJECT_H_
#define __GAME_OBJECT_H_

#include <ecs/component_store.hpp>
#include <ecs/archetype_store.hpp>
#include <ecs/ecs_types.hpp>
#include <atomic>

namespace ecs
{
class game_object
{
public:
    game_object() : _id(++NextId)
    {
    }

    game_object_id id() const;

private:
    game_object_id _id;

    static std::atomic<game_object_id> NextId;
};
} // namespace ecs

#endif
