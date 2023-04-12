#include "Cinq.hpp"

Cinq::Cinq()
    : window(1280, 720, "Cinq"), width(1280), height(720), title("Cinq") {}

Cinq::Cinq(int width, int height, const char* title)
    : window(width, height, title), width(width), height(height), title(title) {

    Factory factory(window.getGraphicsPipeline());
    drawables.reserve(drawableCount);
    std::generate_n(std::back_inserter(drawables), drawableCount, factory);

    window.getGraphicsPipeline().setProjection(DirectX::XMMatrixPerspectiveLH(1.f, (float)height / width, .5f, 40.f));
}

int Cinq::run() {
    srand((unsigned)time(0));
    int error;
    while (!window.processMessages(&error))
        update();

    return error;
}

void Cinq::update() {
    float ts = timer.markLap();

    float bgColor[4] {.1f, .1f, .1f, .1f};
    window.getGraphicsPipeline().clearBuffer(bgColor);

    for (auto& drawable : drawables) {
        drawable->update(ts);
        drawable->draw(window.getGraphicsPipeline());
    }

    float t1 = timer.sinceLastLap();
    char buf[128];
    sprintf(buf, "%s %.2fms frametime (%.2fms since last update / %d FPS)", title, t1*1000, ts*1000, (int)(1000/(ts*1000))); 
    window.setTitle(buf);

    window.getGraphicsPipeline().presentBuffer();
}