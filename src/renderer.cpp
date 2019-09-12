#include "renderer/renderer.hpp"

#include <iostream>

bool rendering::renderer::init()
{

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, _config.width(), _config.height());

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    return true;
}
