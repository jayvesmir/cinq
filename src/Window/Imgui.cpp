#include "Imgui.hpp"

Imgui::Imgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiStyle& style = ImGui::GetStyle();
    style.TabRounding       = 11 / 3.;
    style.GrabRounding      = 11 / 3.;
    style.ChildRounding     = 11 / 3.;
    style.FrameRounding     = 11 / 3.;
    style.PopupRounding     = 11 / 3.;
    style.WindowRounding    = 11 / 3.;
    style.ScrollbarRounding = 11 / 3.;

    style.ScaleAllSizes(1.33f);
}

Imgui::~Imgui() {
    ImGui::DestroyContext();
}