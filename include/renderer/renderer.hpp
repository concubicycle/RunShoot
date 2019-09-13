#ifndef __RENDERER_H_
#define __RENDERER_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "core/startup_config.hpp"

#include <vertex_buffer.h>
#include <vertex_p.h>

namespace rendering
{
class renderer
{
public:
    renderer(const core::startup_config &config) : _config(config) {}
    bool init();

private:
    const core::startup_config &_config;
};
} // namespace rendering

#endif
