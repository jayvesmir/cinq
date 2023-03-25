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
    char buf[128];
    sprintf(buf, "%s (%.2fms since last frame)", title, t*1000); 
    window.setTitle(buf);

    window.getGraphicsPipeline().clearBuffer({.1f, .1f, .1f, .1f});
    window.getGraphicsPipeline().draw();
    window.getGraphicsPipeline().presentBuffer();

    // Can now be removed because presentBuffer() handles the blocking.
    // I'm still keeping it just in case something else makes windows cry again
    // and I won't remember how to calm it down.
    /* Sleep for 1 millisecond to prevent Windows from shitting itself from too many window renames
    timer.wait(1); */
}