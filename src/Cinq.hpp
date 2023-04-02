#pragma once

#include "Window.hpp"
#include "Timer.hpp"
#include "Drawable/Cube.hpp"

#include <ctime>
#include <memory>
#include <random>
#include <cstdlib>

#define PI 3.141592653589793238462643383279502884L

class Cinq {
public:
    Cinq();
    Cinq(int width, int height, const char* title);

    int run();
private:
    Timer timer;
    Window window;
    const char* title;
    int width, height;
    std::vector<std::unique_ptr<Cube>> boxes;

    void update();
};