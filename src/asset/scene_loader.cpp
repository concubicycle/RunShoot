//
// Created by sava on 10/13/19.
//

#include <unordered_map>
#include <asset/scene_loader.hpp>
#include <unordered_set>
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

scene_graph::scene asset::scene_loader::load_scene(const std::string& file_path, ecs::entity_world &world)
{
    scene_graph::scene scene(world);
    std::ifstream i(file_path);
    json scene_json;
    i >> scene_json;

    for (auto &entity_json : scene_json["entities"])
    {
        entity_id id = entity_json["entity_id"].get<entity_id>();
        json prototype = inflate_prototype(entity_json);
        auto& e = load_prototype(prototype, world, scene.scene_graph(), id);
    }

    return scene;
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


json asset::scene_loader::inflate_prototype(json &entity_json)
{
    auto entity_components = entity_json["components"];

    auto prototype_path_it = entity_json.find("prototype");
    if (prototype_path_it == entity_json.end())
        return {{"root", {"components", entity_components}}};

    auto prototype_path = prototype_path_it.value().get<std::string>();
    json prototype_json = _prototypes.load(prototype_path);

    merge_component_props(prototype_json["root"], entity_json);
    return prototype_json;
}

void asset::scene_loader::merge_component_props(json &target, json &source)
{
    std::unordered_set<std::uint8_t > target_component_shifts;
    std::unordered_map<std::uint8_t, json> shift_to_source_component;

    auto& source_components = source["components"];
    auto& target_components = target["components"];

    // keep track of scene entity components.
    for (auto& c : source_components)
    {
        auto shift = c["component_bitshift"].get<std::uint8_t>();
        shift_to_source_component[shift] = c;
    }

    // override prototype component props with scene entity component props
    for (auto& target_component : target_components)
    {
        auto shift = target_component["component_bitshift"].get<std::uint8_t>();
        target_component_shifts.insert(shift);

        auto it = shift_to_source_component.find(shift);
        if (it == shift_to_source_component.end()) continue;

        auto& entity_component = it->second;
        for (auto& el : entity_component.items())
        {
            target_component[el.key()] = el.value();
        }
    }

    // insert components that are in scene, but not prototype
    for (auto& c : source_components)
    {
        auto shift = c["component_bitshift"].get<std::uint8_t>();
        if (target_component_shifts.find(shift) == target_component_shifts.end())
        {
            target_components.push_back(c);
        }
    }
}



ecs::entity &
asset::scene_loader::load_prototype(
    json &prototype,
    ecs::entity_world &world,
    scene_graph::scene::scene_graph_t &scene_graph,
    entity_id id)
{
    auto& root = prototype["root"];
    auto archetype_id = calc_archetype_id(root["components"]);
    auto& e_root = world.add_entity(archetype_id, id);
    load_entity_components(e_root, root["components"]);
    scene_graph_insert(scene_graph, e_root, root);
    _events.invoke<ecs::entity&>(events::event_type::entity_created, e_root);

    return load_prototype(prototype, world, scene_graph, e_root);

}

ecs::entity &asset::scene_loader::load_prototype(
    json &prototype,
    ecs::entity_world &world,
    scene_graph::scene::scene_graph_t &scene_graph)
{
    auto& root = prototype["root"];
    auto archetype_id = calc_archetype_id(root["components"]);
    auto& e_root = world.add_entity(archetype_id);
    load_entity_components(e_root, root["components"]);
    //scene_graph_insert(scene_graph, e_root, root); // do not insert here

    return load_prototype(prototype, world, scene_graph, e_root);
}

ecs::entity &
asset::scene_loader::load_prototype(
    json &prototype,
    ecs::entity_world &world,
    scene_graph::scene::scene_graph_t &scene_graph,
    ecs::entity &e_root)
{
    if (prototype.find("children") == prototype.end()) return e_root;

    std::unordered_map<std::uint32_t, ecs::entity&> index_to_entity;

    std::uint32_t count = 0;
    for (auto& c : prototype["children"])
    {
        auto e_prototype = inflate_prototype(c);
        auto& e_child = load_prototype(e_prototype, world, scene_graph);
        index_to_entity.insert({count, e_child});
        count++;
    }

    count = 0;
    for (auto& c : prototype["children"])
    {
        auto parent_index = c.value("parent_index", -1);
        auto& e_child = index_to_entity.find(count)->second;
        auto parent_id = parent_index == -1
            ? e_root.id()
            : index_to_entity.find(parent_index)->second.id();

        scene_graph_insert(scene_graph, e_child, parent_id);
        _events.invoke<ecs::entity&>(events::event_type::entity_created, e_child);
    }

    return e_root;
}

void
asset::scene_loader::scene_graph_insert(scene_graph::scene::scene_graph_t &scene_graph, ecs::entity &e, entity_id parent_id)
{
    auto t_opt = e.get_component_opt<ecs::transform_component>();

    if (t_opt)
        scene_graph.insert(e, e.id(), parent_id, t_opt->get().to_mat4());
    else
        scene_graph.insert(e, e.id(), parent_id);
}

void asset::scene_loader::scene_graph_insert(scene_graph::scene::scene_graph_t& scene_graph, ecs::entity &e,  json& e_json)
{
    auto t_opt = e.get_component_opt<ecs::transform_component>();
    auto parent_id = e_json.value("parent_id", -1);

    if (t_opt && parent_id != -1)
        scene_graph.insert(e, e.id(), parent_id, t_opt->get().to_mat4());
    else if (parent_id != -1)
        scene_graph.insert(e, e.id(), parent_id);
    else if (t_opt)
        scene_graph.add_child(e, e.id(), t_opt->get().to_mat4());
    else
        scene_graph.add_child(e, e.id());
}
