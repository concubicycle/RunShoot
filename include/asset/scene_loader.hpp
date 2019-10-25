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
#include <asset/texture_manager.hpp>
#include <ecs/components/basic_components.hpp>

#include <util/string_table.hpp>

using nlohmann::json;

namespace asset
{
    using bitshift_to_component_loader =
        std::unordered_map<std::uint8_t , std::function<void(const json&, ecs::entity&, string_table&)>>;

    class scene_loader
    {
    public:
        explicit scene_loader(string_table& str_table);

        static bitshift_to_component_loader component_loaders;

        asset::scene load_scene(std::string file_path, ecs::entity_world& world);

    private:
        string_table& _string_table;

        component_bitset calc_archetype_id(const json &components) const;
        void load_entity_components(ecs::entity &e, const json& component_array) const;
    };
}

#endif //__SCENE_LOADER_HPP_
