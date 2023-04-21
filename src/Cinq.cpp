#include "Cinq.hpp"

Cinq::Cinq()
    : window(1280, 720, "Cinq"), width(1280), height(720), title("Cinq") {}

Cinq::Cinq(int width, int height, const char* title)
    : window(width, height, title), width(width), height(height), title(title) {

    Factory factory(window.getGraphicsPipeline());
    drawables.reserve(drawableCount);
    std::generate_n(std::back_inserter(drawables), drawableCount, factory);

    window.getGraphicsPipeline().setCamera(camera.getTransformMatrix());
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

    static bool showControls = true;
    static bool showDemo  = false;

    if (showDemo) {
        ImGui::ShowDemoWindow(&showDemo);
    }

    // Scene controls overlay - Mostly copied from the demo
    {
        static int location = 0;
        ImGuiWindowFlags windowFlags = 
            ImGuiWindowFlags_NoDecoration       |
            ImGuiWindowFlags_AlwaysAutoResize   |
            ImGuiWindowFlags_NoSavedSettings    |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoNav;

        const float PAD = 10.0f;
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
            ImGui::Text("Frametime: %.2fms", t1*1000);
            ImGui::Text("Since last update: %.2fms", ts*1000);
        }
        ImGui::End();
    }


    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    window.getGraphicsPipeline().presentBuffer();
}