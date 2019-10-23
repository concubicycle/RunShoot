//
// Created by sava on 10/13/19.
//

#ifndef __SCENE_LOADER_HPP_
#define __SCENE_LOADER_HPP_

#include <string>
#include <unordered_map>
#include <functional>
#include <nlohmann/json.hpp>
#include <ecs/entity.hpp>
#include <asset/scene.hpp>


using nlohmann::json;

namespace asset
{
    using bitshift_to_component_loader = std::unordered_map<std::uint8_t , std::function<void(const json&, ecs::entity&)>>;

    class scene_loader
    {
    public:
        static bitshift_to_component_loader component_loaders;

        asset::scene load_scene(std::string file_path, ecs::entity_world& world);

    private:
        component_bitset calc_archetype_id(const json &components) const;
        void load_entity_components(ecs::entity &e, const json& component_array) const;
    };
}

#endif //__SCENE_LOADER_HPP_
