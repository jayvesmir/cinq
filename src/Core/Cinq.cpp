#include "Cinq.hpp"

Cinq::Cinq()
    : window(1280, 720, "Cinq"), width(1280), height(720), title("Cinq") {}

Cinq::Cinq(int width, int height, const char* title)
    : window(width, height, title), width(width), height(height), title(title) {
    drawables.push_back(
        std::make_unique<Cube>(window.getGraphicsPipeline(), 0.f)
    );

    drawables.push_back(
        std::make_unique<Cube>(window.getGraphicsPipeline(), 1.5f)
    );

    drawables.push_back(
        std::make_unique<Cube>(window.getGraphicsPipeline(), -1.5f)
    );

    // Factory factory(window.getGraphicsPipeline());
    // drawables.reserve(drawableCount);
    // std::generate_n(std::back_inserter(drawables), drawableCount, factory);
}

int Cinq::run() {
    srand((unsigned)time(0));
    int error;
    while (!window.processMessages(&error))
        update();

    return error;
}

static constexpr float bgColor[4] {0.1f, 0.1f, 0.12f, 1.f};

void Cinq::update() {
    float ts = timer.markLap();

    window.getGraphicsPipeline().clearBuffer(bgColor);
    window.getGraphicsPipeline().setCamera(camera.getTransformMatrix());
    window.getGraphicsPipeline().setProjection(DirectX::XMMatrixPerspectiveLH(1.f, (float)height / width, camera.getNearClip(), camera.getFarClip()));

    // Draw scene
    for (auto& drawable : drawables) {
        drawable->update(ts);
        drawable->draw(window.getGraphicsPipeline());
    }

    float t1 = timer.sinceLastLap();

    // Draw imgui
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static bool showDemo     = false;
    static bool showControls = true;

    if (showDemo) {
        ImGui::ShowDemoWindow(&showDemo);
    }

    // Scene controls overlay - Mostly copied from the demo
    if (showControls) {
        static int location = 0;
        ImGuiWindowFlags windowFlags = 
            ImGuiWindowFlags_NoDecoration       |
            ImGuiWindowFlags_AlwaysAutoResize   |
            ImGuiWindowFlags_NoSavedSettings    |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoNav;

        const float PAD = 15.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 workspacePos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 workspaceSize = viewport->WorkSize;
        ImVec2 position, posPivot;
        position.x = (location & 1) ? (workspacePos.x + workspaceSize.x - PAD) : (workspacePos.x + PAD);
        position.y = (location & 2) ? (workspacePos.y + workspaceSize.y - PAD) : (workspacePos.y + PAD);
        posPivot.x = (location & 1) ? 1.f : 0.f;
        posPivot.y = (location & 2) ? 1.f : 0.f;
        ImGui::SetNextWindowPos(position, ImGuiCond_Always, posPivot);
        windowFlags |= ImGuiWindowFlags_NoMove;

        if (ImGui::Begin("Scene", &showControls, windowFlags)) {
            ImGui::Text("Scene");
            ImGui::Separator();
            ImGui::Text("FPS: %d", (int)(1000/(t1*1000)));
            ImGui::Text("Frametime: %.4fms", t1*1000);
            ImGui::Text("Since last update: %.2fms", ts*1000);
            ImGui::Separator();

            ImGui::Text("Camera");
            camera.createInterface();
        }
        ImGui::End();
    }

    // Key actions

    if (window.keyboard.keyIsPressed(VK_SPACE)) {
        camera.setMoving(true);
        char buf[256];
        snprintf(buf, 256, "Cinq <Moving>");
        window.setTitle(buf);
    }

    if (window.keyboard.keyIsPressed(VK_ESCAPE)) {
        camera.setMoving(false);
        char buf[256];
        snprintf(buf, 256, "Cinq");
        window.setTitle(buf);
    }

    if (window.keyboard.keyIsPressed('R'))
        camera.reset();
    
    if (camera.isMoving()) {
        // Forward
        if (window.keyboard.keyIsPressed('W')) {
            camera.translate({0.f, 0.f, ts});
        }

        // Backward
        if (window.keyboard.keyIsPressed('S')) {
            camera.translate({0.f, 0.f, -ts});
        }

        // Left
        if (window.keyboard.keyIsPressed('A')) {
            camera.translate({-ts, 0.f, 0.f});
        }

        // Right
        if (window.keyboard.keyIsPressed('D')) {
            camera.translate({ts, 0.f, 0.f});
        }

        // Up
        if (window.keyboard.keyIsPressed('E')) {
            camera.translate({0.0f, ts, 0.0f});
        }

        // Down
        if (window.keyboard.keyIsPressed('Q')) {
            camera.translate({0.0f, -ts, 0.0f});
        }

        // Using this because I've failed numerous times at trying to implement raw input ⊂(￣(ｴ)￣)⊃
        camera.rotate(gui.getIO().MouseDelta.x, -gui.getIO().MouseDelta.y);
    }

    ImGui::Render();
    window.getGraphicsPipeline().presentBuffer();
}