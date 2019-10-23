//
// Created by sava on 10/16/19.
//

#ifndef __INPUT_MANAGER_HPP_
#define __INPUT_MANAGER_HPP_

#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstring>

#include <util/running_average.hpp>
#include <glm/vec2.hpp>


namespace core
{
    class input_manager
    {
        std::uint16_t _glfw_key_codes[128];

    public:
        input_manager(GLFWwindow *window);

        bool is_key_down(std::uint16_t key);

        bool was_key_pressed(std::uint16_t key);

        bool was_key_released(std::uint16_t key);

        void update();

        const glm::vec2 mouse_delta() { return _mouse_delta.average(); }

    private:
        GLFWwindow *_window;
        double _last_x = 0, _last_y = 0;
        util::running_average<glm::vec2, float> _mouse_delta;

        std::uint8_t* _current_key_states;
        std::uint8_t* _last_key_states;

        std::uint8_t _key_state_buffer_a[GLFW_KEY_LAST];
        std::uint8_t _key_state_buffer_b[GLFW_KEY_LAST];
    };


}

#endif //__INPUT_MANAGER_HPP_
