//
// Created by sava on 10/25/19.
//

#include <asset/prototype_loader.hpp>
#include <fstream>

using nlohmann::json;

json asset::prototype_loader::load(std::string path)
{
    auto it = _path_to_component.find(path);

    if (it == _path_to_component.end())
    {
        std::ifstream i(path);
        json j;
        i >> j;

        _path_to_component.insert(std::make_pair(path, j));
    }

    return _path_to_component.find(path)->second;
}
