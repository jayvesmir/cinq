#pragma once

#include "Windows.hpp"
#include <chrono>

class Timer {
public:
    Timer();

    static void wait(int seconds);
    float markLap();
    float sinceStart();
    float sinceLastLap();
private:
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point lastLap;
};