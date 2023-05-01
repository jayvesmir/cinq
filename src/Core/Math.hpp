#pragma once
#include <cmath>

#define PI 3.141592653589793238462643383279502884L

// Chili
template<typename T>
T wrapAngle(T theta) {
    const T modded = std::fmod(theta, (T)2.0 * (T)PI);
    return (modded > (T)PI) ? (modded - (T)2.0 * (T)PI) : modded;
}

template<typename T>
static inline constexpr T lerp(const T& start, const T& end, float t) {
    return start + (end - start) * t;
}

template<typename T>
static inline constexpr T deg2rad(T deg) {
    return deg * PI / (T)180.0;
}