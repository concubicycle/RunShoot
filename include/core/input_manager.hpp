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
        static std::uint16_t glfw_key_codes[];

    public:
        input_manager(GLFWwindow *window) :
        _window(window),
        _current_key_states(_key_state_buffer_a),
        _last_key_states(_key_state_buffer_b)
        {
            memset(_key_state_buffer_a, 0, sizeof(_key_state_buffer_a));
            memset(_key_state_buffer_b, 0, sizeof(_key_state_buffer_b));
        }

        bool is_key_down(std::uint16_t key);

        bool was_key_pressed(std::uint16_t key);

        bool was_key_released(std::uint16_t key);

        void update();

    private:
        GLFWwindow *_window;

        std::uint8_t* _current_key_states;
        std::uint8_t* _last_key_states;

        std::uint8_t _key_state_buffer_a[GLFW_KEY_LAST];
        std::uint8_t _key_state_buffer_b[GLFW_KEY_LAST];
    };


}

#endif //__INPUT_MANAGER_HPP_
