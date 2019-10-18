//
// Created by sava on 10/17/19.
//

#ifndef __DEBOUNCE_HPP_
#define __DEBOUNCE_HPP_

#include <functional>
#include <chrono>

using float_second = std::chrono::duration<float>;
using debounce_clock = std::chrono::steady_clock;


class debounce
{
public:
    debounce(
        float_second interval,
        std::function<void()> action) :
        _interval(interval),
        _action(action) {}

    void operator()()
    {
        auto now = debounce_clock::now();
        if (now - _last > _interval)
        {
            _action();
            _last = now;
        }
    }

private:
    float_second _interval;
    debounce_clock::time_point _last;
    std::function<void()> _action;

};

#endif //__DEBOUNCE_HPP_
