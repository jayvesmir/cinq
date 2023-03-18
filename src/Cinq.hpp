#pragma once

#include "Window.hpp"
#include "Timer.hpp"

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

    void update();
};