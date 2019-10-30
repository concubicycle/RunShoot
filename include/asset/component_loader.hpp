//
// Created by sava on 10/25/19.
//

#ifndef __COMPONENT_LOADER_HPP_
#define __COMPONENT_LOADER_HPP_

#include <cstdint>
#include <unordered_map>
#include <util/string_table.hpp>
#include <functional>

#include <nlohmann/json.hpp>

using nlohmann::json;

#include <ecs/entity.hpp>

namespace asset
{
    /**
     * This class handles initializing an actual entity from its json data. The cpp file contains a
     * bunch of default loader functions for the basic components. The component_loaders map is public,
     * which allows client to add their own loaders during game startup.
     */
    class component_loader
    {
        using bitshift_to_component_loader =
            std::unordered_map<component_shift, std::function<void(const json &, ecs::entity &, string_table &)>>;

    public:
        explicit component_loader(string_table &string_table);

        void load_component(ecs::entity& e, const json& j, std::uint8_t component_bitshift) const;

        static bitshift_to_component_loader loader_functions;

    private:

        string_table &_string_table;
    };
}

#endif //__COMPONENT_LOADER_HPP_
