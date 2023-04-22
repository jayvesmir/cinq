#include "Camera.hpp"

Camera::Camera() {
    reset();
}

DirectX::XMMATRIX Camera::getTransformMatrix() const {
    const DirectX::XMVECTOR pos = DirectX::XMVector3Transform(
        DirectX::XMVectorSet(0.f, 0.f, -r, 0.f),
        DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, 0.f)
    );

    // Flip the camera upside down if going down from over the poles.
    // Hard to explain, remove this code and try going over one of the poles
    // you'll see why it's here
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
    if (std::abs(phi) >= deg2rad(90.f))
        up = DirectX::XMVectorSet(0.f, -1.f, 0.f, 0.f);

    return (
        DirectX::XMMatrixLookAtLH(
            pos, DirectX::XMVectorZero(), up
        ) * 
        DirectX::XMMatrixRotationRollPitchYaw(pitch, -yaw, roll)
    );
}

void Camera::createInterface() {
    ImGui::SliderFloat("Far Clip", &farClip, 0.f, 2500.f);
    ImGui::SliderFloat("Near Clip", &nearClip, 0.f, 5.f);
    ImGui::Separator();
    ImGui::SliderFloat("R", &r, -20.f, 20.f);
    ImGui::SliderAngle("Phi", &phi, -180.f, 180.f);
    ImGui::SliderAngle("Theta", &theta, -180.f, 180.f);
    ImGui::Separator();
    ImGui::SliderAngle("Yaw", &yaw, -180.f, 180.f);
    ImGui::SliderAngle("Roll", &roll, -180.f, 180.f);
    ImGui::SliderAngle("Pitch", &pitch, -180.f, 180.f);
    if (ImGui::Button("Reset"))
        reset();
}

void Camera::reset() {
    r        = 5.f;
    phi      = 0.f;
    yaw      = 0.f;
    roll     = 0.f;
    pitch    = 0.f;
    theta    = 0.f;
    farClip  = 1000.f;
    nearClip = 2 / 3.f;

    // Prep for using cartesian coordinates later on
    pos      = {0.f, 0.f, 0.f};
}