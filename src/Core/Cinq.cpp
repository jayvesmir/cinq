#include "Cinq.hpp"

Cinq::Cinq()
    : window(1280, 720, "Cinq"), width(1280), height(720), title("Cinq"), aspectRatio((float)width / height), light(window.getGraphicsPipeline()) {}

Cinq::Cinq(int width, int height, const char* title)
    : window(width, height, title), width(width), height(height), title(title), aspectRatio((float)width / height), light(window.getGraphicsPipeline()) {
    drawables.push_back(
        std::make_unique<CubeSolid>(window.getGraphicsPipeline(), DirectX::XMFLOAT3{0.f, 0.f, 0.f})
    );

    drawables.push_back(
        std::make_unique<CubeSolid>(window.getGraphicsPipeline(), DirectX::XMFLOAT3{1.5f, 0.f, 0.f})
    );

    drawables.push_back(
        std::make_unique<CubeSolid>(window.getGraphicsPipeline(), DirectX::XMFLOAT3{-1.5f, 0.f, 0.f})
    );

    Factory factory(window.getGraphicsPipeline());
    drawables.reserve(drawableCount);
    std::generate_n(std::back_inserter(drawables), drawableCount, factory);
}

int Cinq::run() {
    srand((unsigned)time(0));
    int error;
    Logger::info("[Renderer] Start render loop");
    while (!window.processMessages(&error))
        update();

    return error;
}

static constexpr float bgColor[4] {0.1f, 0.1f, 0.12f, 1.f};

void Cinq::update() {
    float ts = timer.markLap();
    ImGuiIO io = gui.getIO();
    auto mouse = window.mouse.read();

    window.getGraphicsPipeline().clearBuffer(bgColor);
    window.getGraphicsPipeline().setCamera(camera.getTransformMatrix());
    window.getGraphicsPipeline().setProjection(DirectX::XMMatrixPerspectiveFovLH(camera.fov, aspectRatio, camera.nearClip, camera.farClip));

    // Draw scene
    light.draw(window.getGraphicsPipeline());
    for (auto& drawable : drawables) {
        drawable->update(ts);
        drawable->draw(window.getGraphicsPipeline());
    }

    float t1 = timer.sinceLastLap();

    // Draw imgui
    gui.newFrame();

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

        ImGuiWindowFlags cameraWindowFlags = 
            ImGuiWindowFlags_AlwaysAutoResize   |
            ImGuiWindowFlags_NoFocusOnAppearing;

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

        ImGui::SetNextWindowBgAlpha(0.75f);
        if (ImGui::Begin("Scene", &showControls, windowFlags)) {
            ImGui::Text("Scene");
            ImGui::Separator();
            ImGui::Text("FPS: %d", (int)(1000/(t1*1000)));
            ImGui::Text("Frametime: %.4fms", t1*1000);
            ImGui::Text("Since last update: %.2fms", ts*1000);
            ImGui::Text("Objects: %d", drawables.size());
            ImGui::Separator();
            ImGui::Checkbox("V-Sync", &window.getGraphicsPipeline().vsync);
        }

        ImGui::SetNextWindowBgAlpha(0.75f);
        if (ImGui::Begin("Camera", &showControls, cameraWindowFlags))
            camera.createInterface();
            ImGui::SetNextWindowBgAlpha(0.75f);
        if (ImGui::Begin("Light", &showControls, cameraWindowFlags))
            light.createInterface();

        ImGui::End();
    }

    /* Key actions */

    if (window.keyboard.keyIsPressed(VK_SPACE)) {
        camera.setFPS(true);
        char buf[256];
        snprintf(buf, 256, "Cinq <FPS>");
        window.setTitle(buf);
        window.hideCursor();
    }

    if (window.keyboard.keyIsPressed(VK_ESCAPE)) {
        camera.reset();
        char buf[256];
        snprintf(buf, 256, "Cinq");
        window.setTitle(buf);
        window.showCursor();
    }

    if (camera.isFPS()) {
        // Forward
        if (window.keyboard.keyIsPressed('W'))
            camera.translate({0.f, 0.f, ts});

        // Backward
        if (window.keyboard.keyIsPressed('S'))
            camera.translate({0.f, 0.f, -ts});

        // Left
        if (window.keyboard.keyIsPressed('A'))
            camera.translate({-ts, 0.f, 0.f});

        // Right
        if (window.keyboard.keyIsPressed('D'))
            camera.translate({ts, 0.f, 0.f});

        // Up
        if (window.keyboard.keyIsPressed('Q'))
            camera.translate({0.0f, ts, 0.0f});

        // Down
        if (window.keyboard.keyIsPressed('E'))
            camera.translate({0.0f, -ts, 0.0f});

        // Zoom In
        if (window.keyboard.keyIsPressed(VK_ADD))
            camera.fov -= ts;

        // Zoom Out
        if (window.keyboard.keyIsPressed(VK_SUBTRACT))
            camera.fov += ts;

        // Using this because I've failed numerous times at trying to implement raw input ⊂(￣(ｴ)￣)⊃
        camera.rotate(io.MouseDelta.x, io.MouseDelta.y);
    }

    ImGui::Render();
    window.getGraphicsPipeline().presentBuffer();
}