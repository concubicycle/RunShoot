//
// Created by sava on 10/13/19.
//

#include <unordered_map>
#include <asset/scene_loader.hpp>
#include <set>
#include <fstream>
#include <nlohmann/json.hpp>


using nlohmann::json;


asset::scene_loader::scene_loader(
    events::event_exchange& events,
    component_loader& component_loader,
    asset::prototype_loader& prototypes) :
    _events(events),
    _component_loader(component_loader),
    _prototypes(prototypes)
    {}

asset::scene asset::scene_loader::load_scene(const std::string& file_path, ecs::entity_world &world)
{
    std::ifstream i(file_path);
    json scene_json;
    i >> scene_json;

    for (auto &entity_json : scene_json["entities"])
    {
        entity_id id = entity_json["entity_id"].get<entity_id>();
        auto components = merge_prototype_and_scene_entity(entity_json);

        component_bitset archetype_id = calc_archetype_id(components);
        ecs::entity &e = world.add_entity(archetype_id, id);
        load_entity_components(e, components);
        _events.invoke<ecs::entity&>(events::event_type::entity_created, e);
    }

    return asset::scene(world);
}

component_bitset asset::scene_loader::calc_archetype_id(const json &components) const
{
    component_bitset archetype_id = 0;

    for (auto &component_json : components)
    {
        auto shift = component_json["component_bitshift"].get<std::uint8_t>();
        archetype_id |= component_bitset(1) << shift;
    }

    return archetype_id;
}

void asset::scene_loader::load_entity_components(ecs::entity &e, const json &component_array) const
{
    for (auto &component : component_array)
    {
        std::uint8_t bit_shift = component["component_bitshift"].get<std::uint8_t>();

       _component_loader.load_component(e, component, bit_shift);
    }
}

json asset::scene_loader::merge_prototype_and_scene_entity(json &entity_json)
{
    std::set<std::uint8_t > prototype_component_shifts;
    std::unordered_map<std::uint8_t, json> shift_to_entity_component;

    auto entity_components = entity_json["components"];

    auto prototype_path_it = entity_json.find("prototype");
    if (prototype_path_it == entity_json.end())
        return entity_components;

    auto prototype_path = prototype_path_it.value().get<std::string>();
    json prototype_json = _prototypes.load(prototype_path);

    auto prototype_components = prototype_json["components"];

    // keep track of scene entity components.
    for (auto& c : entity_components)
    {
        auto shift = c["component_bitshift"].get<std::uint8_t>();
        shift_to_entity_component[shift] = c;
    }

    // override prototype component props with scene entity component props
    for (auto& prototype_component : prototype_components)
    {
        auto shift = prototype_component["component_bitshift"].get<std::uint8_t>();
        prototype_component_shifts.insert(shift);

        auto it = shift_to_entity_component.find(shift);
        if (it == shift_to_entity_component.end()) continue;

        auto& entity_component = it->second;
        for (auto& el : entity_component.items())
        {
            prototype_component[el.key()] = el.value();
        }
    }

    // insert components that are in scene, but not prototype
    for (auto& c : entity_components)
    {
        auto shift = c["component_bitshift"].get<std::uint8_t>();
        if (prototype_component_shifts.find(shift) == prototype_component_shifts.end())
        {
            prototype_json["components"].push_back(c);
        }
    }

    return prototype_components;
}



