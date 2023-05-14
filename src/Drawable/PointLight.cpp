#include "PointLight.hpp"
#include "Window/Imgui.hpp"

PointLight::PointLight(Pipeline& pipeline, float radius) 
    : mesh(pipeline, radius), cbuf(pipeline) {}

void PointLight::reset() {
    pos = {0.f, 0.f, 0.f};
}

void PointLight::createInterface() {
    ImGui::Text("Position");
    ImGui::SliderFloat("X", &pos.x, -20.0f, 20.0f);
    ImGui::SliderFloat("Y", &pos.y, -20.0f, 20.0f);
    ImGui::SliderFloat("Z", &pos.z, -20.0f, 20.0f);
    if (ImGui::Button("Reset")) {
        reset();
    }
    ImGui::End();
}

void PointLight::draw(Pipeline& pipeline) const {
    mesh.setPosition(pos);
    mesh.draw(pipeline);
    bind(pipeline);
}

void PointLight::bind(Pipeline& pipeline) const {
    cbuf.update(pipeline, PointLightCBuf{pos});
    cbuf.bind(pipeline);
}
