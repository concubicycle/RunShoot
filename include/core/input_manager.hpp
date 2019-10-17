//
// Created by sava on 10/16/19.
//

#ifndef __INPUT_MANAGER_HPP_
#define __INPUT_MANAGER_HPP_

#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstring>


namespace core
{
/**
 * GLFW doesn't really have a key state buffer you can get at unfortunately.
 * Creating a key map would be possible with setting a key event callback,
 * via glfwSetKeyCallback(window, key_callback);. This would require some
 * slightly ugly code for passing a function that isn't global.
 *
 * The best approach to match the behavior described in class is a simple
 * wrapper around glfwGetKey(), which already differentiates between
 * pressed/held/released.
 *
 * I can implement my own buffers if needed.
 */
    class input_manager
    {
    public:
        input_manager(GLFWwindow *window) : _window(window)
        {
        }

        bool is_key_down(std::uint16_t key);

        bool was_key_pressed(std::uint16_t key);

        bool was_key_released(std::uint16_t key);

    private:
        GLFWwindow *_window;
    };


}

#endif //__INPUT_MANAGER_HPP_
