#include "Timer.hpp"

Timer::Timer() {
    using namespace std::chrono;

    start = steady_clock::now();
    lastLap = start;
}

void Timer::wait(int milliseconds) {
    Sleep(milliseconds);
}

float Timer::sinceStart() {
    using namespace std::chrono;

    steady_clock::time_point t = steady_clock::now();
    duration<float> d = t - start;
    return d.count();
}

float Timer::sinceLastLap() {
    using namespace std::chrono;

    steady_clock::time_point t = steady_clock::now();
    duration<float> d = t - lastLap;
    return d.count();
}

float Timer::markLap() {
    using namespace std::chrono;

    steady_clock::time_point t = steady_clock::now();
    duration<float> d = t - lastLap;
    lastLap = t;
    return d.count();
}