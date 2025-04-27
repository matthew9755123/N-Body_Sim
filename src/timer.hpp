#pragma once

#include <chrono>

class timer
{
private:
    using time_t = std::chrono::time_point<std::chrono::system_clock>;
    time_t start_time, stop_time;

public:
    timer() = default;

    time_t start() { return (start_time = std::chrono::system_clock::now()); }
    time_t stop() { return (stop_time = std::chrono::system_clock::now()); }

    double elapsed()
    {
        auto diff = stop_time - start_time;
        return std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
    }
};