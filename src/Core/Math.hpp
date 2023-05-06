#pragma once
#include <cmath>

#define PI 3.141592653589793238462643383279502884L
#define PI_F 3.141592653589793238462643383279502884f

// Chili
template<typename T>
T wrapAngle(T theta) {
    constexpr T twoPI = (T)2 * (T)PI;
    const T mod = std::fmod(theta, twoPI);
    if (mod > (T)PI)
        return mod - twoPI;
    else if (mod < -(T)PI)
        return mod + twoPI;
    return mod;
}

template<typename T>
static inline constexpr T lerp(const T& start, const T& end, float t) {
    return start + (end - start) * t;
}

template<typename T>
static inline constexpr T deg2rad(T deg) {
    return deg * PI / (T)180.0;
}