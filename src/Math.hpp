#pragma once
#include <cmath>

#define PI 3.141592653589793238462643383279502884L

template<typename T>
static constexpr T lerp(const T& start, const T& end, float t) {
    return start + (end - start) * t;
}

template<typename T>
static constexpr T deg2rad(T deg) {
    return deg * PI / (T)180.0;
}