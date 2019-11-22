//
// Created by sava on 11/21/19.
//

#include <asset/prototype_spawner.hpp>


using nlohmann::json;
using scene_node_t = scene_graph::scene_graph_root<ecs::entity, entity_id>;
using scene_node_base_t = scene_graph::scene_graph_node_base<ecs::entity, entity_id>;
using scene_graph_t = scene_graph::scene_graph_root<ecs::entity, entity_id>;


asset::prototype_spawner::prototype_spawner(
    events::event_exchange &events,
    component_loader &component_loader,
    asset::prototype_loader &prototypes) :
    _events(events),
    _component_loader(component_loader),
    _prototypes(prototypes) {}


ecs::entity &asset::prototype_spawner::load_prototype(
    const std::string &prototype_path,
    ecs::entity_world &world,
    scene_graph_t &scene_graph,
    entity_id parent_id)
{
    auto prototype = _prototypes.load(prototype_path);
    auto &root = prototype["root"];
    auto archetype_id = calc_archetype_id(root["components"]);
    auto &e_root = world.add_entity(archetype_id);
    load_entity_components(e_root, root["components"]);
    scene_graph_insert(scene_graph, e_root, parent_id);
    _events.invoke<ecs::entity &>(events::event_type::entity_created, e_root);
    return load_prototype(prototype, world, scene_graph, e_root);
}

ecs::entity &asset::prototype_spawner::load_prototype(
    const std::string &prototype_path,
    ecs::entity_world &world,
    scene_graph_t &scene_graph)
{
    auto prototype = _prototypes.load(prototype_path);
    auto &root = prototype["root"];
    auto archetype_id = calc_archetype_id(root["components"]);
    auto &e_root = world.add_entity(archetype_id);
    load_entity_components(e_root, root["components"]);
    scene_graph_insert(scene_graph, e_root);
    _events.invoke<ecs::entity &>(events::event_type::entity_created, e_root);
    return load_prototype(prototype, world, scene_graph, e_root);
}


ecs::entity &
asset::prototype_spawner::load_prototype(
    json &prototype,
    ecs::entity_world &world,
    scene_graph_t &scene_graph,
    entity_id id)
{
    auto &root = prototype["root"];
    auto archetype_id = calc_archetype_id(root["components"]);
    auto &e_root = world.add_entity(archetype_id, id);
    load_entity_components(e_root, root["components"]);
    scene_graph_insert(scene_graph, e_root, root);
    _events.invoke<ecs::entity &>(events::event_type::entity_created, e_root);
    return load_prototype(prototype, world, scene_graph, e_root);

}

ecs::entity &asset::prototype_spawner::load_prototype(
    json &prototype,
    ecs::entity_world &world,
    scene_graph_t &scene_graph)
{
    auto &root = prototype["root"];
    auto archetype_id = calc_archetype_id(root["components"]);
    auto &e_root = world.add_entity(archetype_id);
    load_entity_components(e_root, root["components"]);
    // NOTE: We don't insert into scene graph here -
    // this is used for loading children, and a parent
    // (possibly another child) may still not be loaded.
    return load_prototype(prototype, world, scene_graph, e_root);
}

ecs::entity &
asset::prototype_spawner::load_prototype(
    json &prototype,
    ecs::entity_world &world,
    scene_graph_t &scene_graph,
    ecs::entity &e_root)
{
    if (prototype.find("children") == prototype.end()) return e_root;

    std::unordered_map<std::uint32_t, ecs::entity &> index_to_entity;

    std::uint32_t count = 0;
    for (auto &c : prototype["children"])
    {
        auto e_prototype = inflate_prototype(c);
        auto &e_child = load_prototype(e_prototype, world, scene_graph);
        index_to_entity.insert({count, e_child});
        count++;
    }

    count = 0;
    for (auto &c : prototype["children"])
    {
        auto parent_index = c.value("parent_index", -1);
        auto &e_child = index_to_entity.find(count)->second;
        auto parent_id = parent_index == -1
                         ? e_root.id()
                         : index_to_entity.find(parent_index)->second.id();

        scene_graph_insert(scene_graph, e_child, parent_id);
        _events.invoke<ecs::entity &>(events::event_type::entity_created, e_child);
    }

    return e_root;
}

void
asset::prototype_spawner::scene_graph_insert(
    scene_graph_t &scene_graph,
    ecs::entity &e)
{
    auto &insert_result = scene_graph.add_child(e, e.id());
    scene_node_base_t *node_ptr = &insert_result;
    e.graph_node = node_ptr;
}

void
asset::prototype_spawner::scene_graph_insert(
    scene_graph_t &scene_graph,
    ecs::entity &e,
    entity_id parent_id)
{
    auto insert_result = scene_graph.insert(e, e.id(), parent_id);
    if (insert_result)
    {
        auto &node_ref = insert_result->get();
        scene_node_base_t *node_ptr = &(node_ref);
        e.graph_node = node_ptr;
    }
}

void asset::prototype_spawner::scene_graph_insert(
    scene_graph_t &scene_graph,
    ecs::entity &e,
    json &e_json)
{
    auto parent_id = e_json.value("parent_id", -1);
    scene_node_base_t *node_ptr;

    if (parent_id != -1)
    {
        auto insert_result = scene_graph.insert(e, e.id(), parent_id);
        if (insert_result)
        {
            auto& child = insert_result->get();
            node_ptr = &(child);
        }
    }
    else
    {
        auto& child = scene_graph.add_child(e, e.id());
        node_ptr = &(child);
    }

    e.graph_node = node_ptr;
}


component_bitset asset::prototype_spawner::calc_archetype_id(const json &components) const
{
    component_bitset archetype_id = 0;

    for (auto &component_json : components)
    {
        auto shift = component_json["component_bitshift"].get<std::uint8_t>();
        archetype_id |= component_bitset(1) << shift;
    }

    return archetype_id;
}


json asset::prototype_spawner::inflate_prototype(json &entity_json)
{
    auto entity_components = entity_json["components"];
    auto prototype_path_it = entity_json.find("prototype");

    if (prototype_path_it == entity_json.end())
        return {{"root", entity_json}};

    auto prototype_path = prototype_path_it.value().get<std::string>();
    json prototype_json = _prototypes.load(prototype_path);
    merge_component_props(prototype_json["root"], entity_json);
    return prototype_json;
}


void asset::prototype_spawner::load_entity_components(ecs::entity &e, const json &component_array) const
{
    for (auto &component : component_array)
    {
        auto bit_shift = component["component_bitshift"].get<std::uint8_t>();
        _component_loader.load_component(e, component, bit_shift);
    }
}


void asset::prototype_spawner::merge_component_props(json &target, json &source)
{
    std::unordered_set<std::uint8_t> target_component_shifts;
    std::unordered_map<std::uint8_t, json> shift_to_source_component;

    auto &source_components = source["components"];
    auto &target_components = target["components"];

    // keep track of scene entity components.
    for (auto &c : source_components)
    {
        auto shift = c["component_bitshift"].get<std::uint8_t>();
        shift_to_source_component[shift] = c;
    }

    // override prototype component props with scene entity component props
    for (auto &target_component : target_components)
    {
        auto shift = target_component["component_bitshift"].get<std::uint8_t>();
        target_component_shifts.insert(shift);

        auto it = shift_to_source_component.find(shift);
        if (it == shift_to_source_component.end()) continue;

        auto &entity_component = it->second;
        for (auto &el : entity_component.items())
        {
            target_component[el.key()] = el.value();
        }
    }

    // insert components that are in scene, but not prototype
    for (auto &c : source_components)
    {
        auto shift = c["component_bitshift"].get<std::uint8_t>();
        if (target_component_shifts.find(shift) == target_component_shifts.end())
        {
            target_components.push_back(c);
        }
    }
}


