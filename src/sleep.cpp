#include "platform/sleep.hpp"

#if (WIN32)
#include <windows.h>
#include <timeapi.h>

void os::sleep(std::chrono::nanoseconds nanos)
{
    // TODO: use media timer
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(nanos).count();
	auto wait_time = millis < 0 ? 0 : millis;
    Sleep(wait_time);
}

#elif (__linux__)
#include <time.h>

void os::sleep(std::chrono::nanoseconds nanos)
{
    timespec ts = {0, nanos.count()};
    nanosleep(&ts, NULL);
}

#else
// nanosleep should be available on mac in the same way?
#include <time.h>

void os::sleep(std::chrono::nanoseconds nanos)
{
    timespec ts = {0, nanos.count()};
    nanosleep(&ts, NULL);
}
#endif