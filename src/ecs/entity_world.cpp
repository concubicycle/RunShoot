//
// Created by sava on 10/15/19.
//

#include <ecs/entity.hpp>
#include <ecs/entity_world.hpp>

ecs::entity &ecs::entity_world::add_entity(component_bitset archetype_id)
{
    return add_entity(archetype_id, next_entity_id++);
}


ecs::entity& ecs::entity_world::add_entity(component_bitset archetype_id, entity_id id)
{
    auto it = _entity_lookup.find(id);

    if (it == _entity_lookup.end())
    {
        _entity_lookup.emplace(id, _entity_factory.make_entity(archetype_id, id));
    }

    it = _entity_lookup.find(id);

    if (id >= next_entity_id) next_entity_id = id + 1;

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

void ecs::entity_world::remove_entity(entity_id entity_id)
{
    auto it = _entity_lookup.find(entity_id);
    if (it == _entity_lookup.end()) return;

    auto& e = it->second;

    _events.invoke<entity&>(events::entity_destroyed, e);

    _entity_factory.free_entity(e);
    _entity_lookup.erase(it);
}


std::atomic_uint ecs::entity_world::next_entity_id = 100000;
