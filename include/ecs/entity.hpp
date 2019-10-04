#ifndef __ECS_TYPES_H_
#define __ECS_TYPES_H_

#include <ecs/ecs_types.hpp>
#include <ecs/archetype_id.hpp>

namespace ecs
{

class entity_base
{
public:
    entity_id id() { return _id; }

protected:
    const entity_id _id;
    entity_base(entity_id id) : _id(id) {}
};

template <class... TComponents>
class entity : public entity_base
{
public:
    entity(
        entity_id id,
        std::shared_ptr<std::tuple<TComponents...>> components)
        : entity_base(id),
          _components(components)
    {
    }

    std::shared_ptr<std::tuple<TComponents...>> components() { return _components; }

private:
    const std::shared_ptr<std::tuple<TComponents...>> _components;
};

} // namespace ecs

#endif