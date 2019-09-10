#include "core/frame_timer.hpp"

void core::frame_timer::start()
{
    _start = _high_res_timer.now();
}

void core::frame_timer::end()
{
    _end = _high_res_timer.now();
    _delta = _end - _start;
    _smoothed_delta = _frame_average.update(_delta);
}

std::chrono::nanoseconds core::frame_timer::delta()
{
    return _delta;
}

std::chrono::nanoseconds core::frame_timer::smoothed_delta()
{
    return _smoothed_delta;
}