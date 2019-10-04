#include "renderer/renderer.hpp"

#include <iostream>

#include <glbinding/gl/gl.h>

bool rendering::renderer::init()
{
    gl::glViewport(0, 0, _config.width(), _config.height());

    gl::glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    return true;
}
