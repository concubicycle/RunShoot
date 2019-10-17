//
// Created by sava on 10/16/19.
//

#include "core/input_manager.hpp"


bool core::input_manager::is_key_down(std::uint16_t key)
{
    auto state = glfwGetKey(_window, key);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool core::input_manager::was_key_pressed(std::uint16_t key)
{
    auto state = glfwGetKey(_window, key);
    return state == GLFW_PRESS;
}

bool core::input_manager::was_key_released(std::uint16_t key)
{
    auto state = glfwGetKey(_window, key);
    return state == GLFW_RELEASE;
}
