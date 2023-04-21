#include "Cinq.hpp"

Cinq::Cinq()
    : window(1280, 720, "Cinq"), width(1280), height(720), title("Cinq") {}

Cinq::Cinq(int width, int height, const char* title)
    : window(width, height, title), width(width), height(height), title(title) {

    drawables.push_back(
        std::make_unique<Cube>(window.getGraphicsPipeline(), 0.f)
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

void Cinq::update() {
    float ts = timer.markLap();

    static float farClip = 1000.f;
    static float nearClip = 2 / 3.f;
    float bgColor[4] {.1f, .1f, .1f, .1f};
    window.getGraphicsPipeline().clearBuffer(bgColor);
    window.getGraphicsPipeline().setCamera(camera.getTransformMatrix());
    window.getGraphicsPipeline().setProjection(DirectX::XMMatrixPerspectiveLH(1.f, (float)height / width, nearClip, farClip));

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
            ImGui::SliderFloat("Far Clip", &farClip, 0.f, 2500.f);
            ImGui::SliderFloat("Near Clip", &nearClip, 0.f, 5.f);
            ImGui::Separator();
            ImGui::SliderFloat("R", &camera.r, 0.f, 20.f);
            ImGui::SliderAngle("Theta", &camera.theta, -180.f, 180.f);
            ImGui::SliderAngle("Phi", &camera.phi, -180.f, 180.f);
            ImGui::Separator();
            ImGui::SliderAngle("Roll", &camera.roll, -180.f, 180.f);
            ImGui::SliderAngle("Pitch", &camera.pitch, -180.f, 180.f);
            ImGui::SliderAngle("Yaw", &camera.yaw, -180.f, 180.f);
            if (ImGui::Button("Reset")) {
                camera.reset();
                nearClip = 2 / 3.f;
                farClip = 1000.f;
            }
        }
        ImGui::End();
    }


    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    window.getGraphicsPipeline().presentBuffer();
}