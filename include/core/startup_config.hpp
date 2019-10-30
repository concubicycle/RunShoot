#ifndef __STARTUP_CONFIG_H_
#define __STARTUP_CONFIG_H_

#include <string>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace core
{
class startup_config
{
private:
    static const std::string FileName;
    json config_json;

public:
    startup_config()
        : config_json(make_default())
    {
    }

    void load();

    bool fullscreen() const;
    std::uint32_t height() const;
	std::uint32_t width() const;
    float fov() const;
    bool backface_culling() const;

private:
    json make_default() const;
};
} // namespace core

#endif
