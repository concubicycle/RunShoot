//
// Created by sava on 12/15/19.
//

#include <renderer/render_config.hpp>

rendering::render_config::render_config(
    std::uint32_t screen_width,
    std::uint32_t screen_height,
    std::uint32_t window_width,
    std::uint32_t window_height,
    bool backface_culling,
    bool fullscreen) :
    _screen_width(screen_width),
    _screen_height(screen_height),
    _window_width(window_width),
    _window_height(window_height),
    _backface_culling(backface_culling),
    _fullscreen(fullscreen),
    _width(fullscreen ? _screen_width : window_width),
    _height(fullscreen ? _screen_height : window_height) {}

void rendering::render_config::set_window_width(std::uint32_t w)
{
    _window_width = w;
    if (!_fullscreen) _width = w;
}

void rendering::render_config::set_window_height(std::uint32_t h)
{
    _window_height = h;
    if (!_fullscreen) _height = h;
}

void rendering::render_config::set_fullscreen(bool value)
{
    _fullscreen = value;
    _width = _fullscreen ? _screen_width : _window_width;
    _height = _fullscreen ? _screen_height : _window_height;
}