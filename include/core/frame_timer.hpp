#ifndef __FRAME_TIMER_H_
#define __FRAME_TIMER_H_

#include <cstdint>
#include <chrono>
#include <string>

#include "util/running_average.hpp"

namespace core
{
class frame_timer
{
private:
    util::running_average<std::chrono::nanoseconds> _frame_average;
    std::chrono::high_resolution_clock _high_res_timer;
    std::chrono::time_point<std::chrono::high_resolution_clock> _start, _end;

    std::chrono::nanoseconds _delta;
    std::chrono::nanoseconds _smoothed_delta;

public:
    frame_timer() : _frame_average(10) {}

    void start();
    void end();

    std::chrono::nanoseconds delta() const;
    std::chrono::nanoseconds smoothed_delta() const;
    std::chrono::nanoseconds current_frame_time() const;

    std::string frame_info() const;
};
} // namespace core

#endif
