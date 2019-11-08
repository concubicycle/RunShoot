//
// Created by sava on 10/15/19.
//

#include <ecs/entity.hpp>
#include <ecs/entity_world.hpp>

ecs::entity& ecs::entity_world::add_entity(component_bitset archetype_id, entity_id id)
{
    auto it = _entity_lookup.find(id);

    if (it == _entity_lookup.end())
    {
        _entity_lookup.emplace(id, _entity_factory.make_entity(archetype_id, id));
    }

    it = _entity_lookup.find(id);
    return it->second;
}

void ecs::entity_world::for_all_entities(std::function<void(entity & )> callback)
{
    for (auto& pair : _entity_lookup)
    {
        callback(pair.second);
    }
}

ecs::entity &ecs::entity_world::get_entity(entity_id id)
{
    return _entity_lookup.find(id)->second;
}
