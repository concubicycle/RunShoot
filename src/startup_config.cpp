#include "core/startup_config.hpp"

#include "nlohmann/json.hpp"

#include <sys/stat.h>
#include <fstream>

using json = nlohmann::json;

const std::string core::startup_config::FileName = "startup_config.json";

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
        stream << config_json;
    }
}

const bool core::startup_config::fullscreen() const
{
    return config_json["fullscreen"].get<bool>();
}

json core::startup_config::make_default() const
{
    return {
        {"fullscreen", false}};
}