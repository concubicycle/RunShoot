//
// Created by sava on 10/16/19.
//

#ifndef __RENDER_CONFIG_HPP_
#define __RENDER_CONFIG_HPP_

#include <cstdint>

namespace rendering
{

    class render_config
    {
    public:
        render_config(
            std::uint32_t screen_width,
            std::uint32_t screen_height,
            std::uint32_t window_width,
            std::uint32_t window_height,
            bool backface_culling,
            bool fullscreen);

        [[nodiscard]] bool backface_culling() const { return _backface_culling; }

        [[nodiscard]] bool fullscreen() const { return _fullscreen; }

        [[nodiscard]] float width() const { return _width; }

        [[nodiscard]] float height() const { return _height; }

        void set_window_width(std::uint32_t w);

        void set_window_height(std::uint32_t h);

        void set_fullscreen(bool value);

    private:
        std::uint32_t _screen_width;
        std::uint32_t _screen_height;
        std::uint32_t _window_width;
        std::uint32_t _window_height;
        bool _backface_culling;
        bool _fullscreen;
        float _width;
        float _height;
    };
}

#endif //__RENDER_CONFIG_HPP_
