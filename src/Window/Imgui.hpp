#pragma once

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_dx11.h>
#include <imgui/backends/imgui_impl_win32.h>

class Imgui {
public:
    Imgui();
    ~Imgui();

    static ImGuiIO getIO() { return ImGui::GetIO(); };
};