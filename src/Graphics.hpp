#pragma once

#include <cstdlib>

class Color {
public:
    Color(float r, float g, float b, float a) 
        : r(r), g(g), b(b), a(a) { 
            data[0] = r; data[1] = g; data[2] = b; data[3] = a;
        }

    float r, g, b, a;
    float data[4];

    static Color random(float value) {
        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;
        return {r / 255.f, g / 255.f, b / 255.f, value};
    }
};