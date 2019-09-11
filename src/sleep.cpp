#include "platform/sleep.hpp"

#if (WIN32)
#include <windows.h>
#include <timeapi.h>

void os::sleep(std::chrono::nanoseconds millis)
{
    // TODO: use media timer
    Sleep(millis.ticks());
}

#elif (__linux__)
#include <time.h>

void os::sleep(std::chrono::nanoseconds nanos)
{
    timespec ts = {0, nanos.count()};
    nanosleep(&ts, NULL);
}

#else
#error Unfamiliar platform
#endif