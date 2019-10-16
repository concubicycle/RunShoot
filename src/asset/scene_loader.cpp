//
// Created by sava on 10/13/19.
//

#include <asset/scene_loader.hpp>
#include <ecs/components/basic_components.hpp>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using nlohmann::json;
using bitshift_to_component_loader = std::unordered_map<std::uint8_t , std::function<void(const json&, ecs::entity&)>>;


//////////// LOADER FUNCTIONS FOR ALL COMPONENT TYPES /////////////
void load_transform(const json& j, ecs::entity& e)
{
    auto& transform = e.get_component<ecs::transform_component>();
    transform.x = 10.f; //j["x"].get<double>();
    transform.y = j["y"].get<float>();
    transform.z = j["z"].get<float>();
    transform.pitch = j["pitch"].get<float>();
    transform.yaw = j["yaw"].get<float>();
    transform.roll = j["roll"].get<float>();
}

const bitshift_to_component_loader asset::scene_loader::component_loaders
{
    {ecs::transform_component::component_bitshift, load_transform }
};
///////////////////////////////////////////////////////////////////




ecs::scene asset::scene_loader::load_scene(std::string file_path, ecs::entity_world &world)
{
    std::ifstream i(file_path);
    json scene_json;
    i >> scene_json;

    for (auto& entity_json : scene_json["entities"])
    {
        entity_id id = entity_json["entity_id"].get<entity_id>();
        component_bitset archetype_id = calc_archetype_id(entity_json["components"]);
        ecs::entity& e = world.add_entity(archetype_id, id);
        load_entity_components(e, entity_json["components"]);
    }

    return ecs::scene(world);
}

component_bitset asset::scene_loader::calc_archetype_id(const json &components) const
{
    component_bitset archetype_id = 0;

    for (auto& component_json : components)
    {
        auto shift = component_json["component_bitshift"].get<std::uint8_t>();
        archetype_id |= (1 << shift);
    }

    return archetype_id;
}

void asset::scene_loader::load_entity_components(ecs::entity &e, const json& component_array) const
{
    for (auto& component : component_array)
    {
        std::uint8_t bit_shift = component["component_bitshift"].get<std::uint8_t>();

        assert(component_loaders.find(bit_shift) != component_loaders.end());

        auto it = component_loaders.find(bit_shift);
        it->second(component, e);
    }
}
