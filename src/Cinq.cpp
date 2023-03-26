#include "Cinq.hpp"

Cinq::Cinq()
    : window(1280, 720, "Cinq"), width(1280), height(720), title("Cinq") {}

Cinq::Cinq(int width, int height, const char* title)
    : window(width, height, title), width(width), height(height), title(title) {}

int Cinq::run() {
    srand((unsigned)time(0));
    int error;
    while (!window.processMessages(&error))
        update();

    return error;
}

void Cinq::update() {
    float t = timer.markLap();

    window.getGraphicsPipeline().clearBuffer({.1f, .1f, .1f, .1f});

    float x = window.mouse.X() / ((float)width / 2) - 1.f;  // Squish mouse position to -1..1
    float y = window.mouse.Y() / ((float)height / 2) - 1.f; // Squish mouse position to -1..1

    window.getGraphicsPipeline().draw(timer.sinceStart(), x, y);
    window.getGraphicsPipeline().draw(-timer.sinceStart(), 0.f, 0.f);

    float t1 = timer.sinceLastLap();
    char buf[128];
    sprintf(buf, "%s %.2fms frametime (%.2fms since last update)", title, t1*1000, t*1000); 
    window.setTitle(buf);

    window.getGraphicsPipeline().presentBuffer();
    // Can now be removed because presentBuffer() handles the blocking.
    // I'm still keeping it just in case something else makes windows cry again
    // and I won't remember how to calm it down.
    // Sleep for 1 millisecond to prevent Windows from shitting itself from too many window renames
    // timer.wait(1);
}