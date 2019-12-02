//
// Created by sava on 10/13/19.
//

#include <unordered_map>
#include <asset/scene_loader.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <asset/scene.hpp>


using nlohmann::json;
using scene_node_t = scene_graph::scene_graph_root<ecs::entity, entity_id>;
using scene_node_base_t = scene_graph::scene_graph_node_base<ecs::entity, entity_id>;

asset::scene_loader::scene_loader(prototype_spawner& spawner) :
    _spawner(spawner)
    {}

void asset::scene_loader::load_scene(const std::string& file_path, ecs::entity_world &world, asset::scene& scene)
{
    //asset::scene scene(_spawner, world);
    std::ifstream i(file_path);
    json scene_json;
    i >> scene_json;

    for (auto &entity_json : scene_json["entities"])
    {
        auto id = entity_json["entity_id"].get<entity_id>();
        json prototype = _spawner.inflate_prototype(entity_json);
        _spawner.load_prototype(prototype, world, scene.scene_graph(), id);
    }
}
