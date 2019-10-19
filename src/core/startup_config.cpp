#include "core/startup_config.hpp"

#include "nlohmann/json.hpp"

#include <sys/stat.h>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

const std::string core::startup_config::FileName = "./startup_config.json";

void core::startup_config::load()
{
    std::fstream stream(FileName);

    if (stream)
    {
        config_json.clear();
        stream >> config_json;
    }
    else
    {
        stream.open(FileName, std::fstream::out);
        stream << config_json;
    }

    stream.close();
}

bool core::startup_config::fullscreen() const { return config_json["fullscreen"].get<bool>(); }
int core::startup_config::height() const { return config_json["height"].get<int>(); }
int core::startup_config::width() const { return config_json["width"].get<int>(); }
float core::startup_config::fov() const { return config_json["fov"]; }
bool core::startup_config::backface_culling() const { return config_json["backfaceCulling"].get<bool>(); }

json core::startup_config::make_default() const
{
    return {
        {"fullscreen", false},
        {"height", 800},
        {"width", 1400},
        {"fov", 45},
        {"backfaceCulling", true}};
}




