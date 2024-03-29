#include "Camera.hpp"

Camera::Camera() {
    reset();
}

void Camera::rotate(float x, float y) {
    yaw = wrapAngle(yaw + x * rotationSpeed);
    pitch = std::clamp(pitch + y * rotationSpeed, .995f * -PI_F / 2.f, .995f * PI_F / 2.f);
}

void Camera::translate(DirectX::XMFLOAT3 offset) {
    DirectX::XMStoreFloat3(&offset,
    DirectX::XMVector3Transform(
            DirectX::XMLoadFloat3(&offset),
            DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
            DirectX::XMMatrixScaling(movementSpeed, movementSpeed, movementSpeed)
    ));

    pos = {
        pos.x + offset.x,
        pos.y + offset.y,
        pos.z + offset.z
    };
}

DirectX::XMMATRIX Camera::getTransformMatrix() const {
    if (fpsControls) {
        DirectX::XMVECTOR forward = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
        DirectX::XMVECTOR direction = DirectX::XMVector3Transform(
            forward, DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll)
        );

        DirectX::XMVECTOR camPosition = DirectX::XMLoadFloat3(&pos);
        DirectX::XMVECTOR camTarget = DirectX::XMVectorAdd(camPosition, direction);
        return DirectX::XMMatrixLookAtLH(camPosition, camTarget, DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));
    }

    const DirectX::XMVECTOR pos = DirectX::XMVector3Transform(
        DirectX::XMVectorSet(0.f, 0.f, -r, 0.f),
        DirectX::XMMatrixRotationRollPitchYaw(phi, -theta, roll)
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
    ImGui::SliderAngle("FOV", &fov, 0.f, 180.f);
    ImGui::SliderFloat("Far Clip", &farClip, 0.f, 2500.f);
    ImGui::SliderFloat("Near Clip", &nearClip, 0.f, 5.f);
    ImGui::SliderFloat("Movement Speed", &movementSpeed, 0.f, 80.f);
    ImGui::SliderFloat("Mouse Sensitivity", &rotationSpeed, 0.f, .01f);
    ImGui::Separator();
    if (fpsControls) {
        ImGui::SliderFloat3("XYZ", (float*)&pos, -10.f, 10.f);
    } else {
        ImGui::SliderFloat("R", &r, -10.f, 20.f);
        ImGui::SliderAngle("Phi", &phi, -180.f, 180.f);
        ImGui::SliderAngle("Theta", &theta, -180.f, 180.f);
    }

    ImGui::Separator();
    ImGui::SliderAngle("Yaw", &yaw, -180.f, 180.f);
    ImGui::SliderAngle("Roll", &roll, -180.f, 180.f);
    ImGui::SliderAngle("Pitch", &pitch, -180.f, 180.f);
    
    if (ImGui::Button("Reset"))
        reset();
}

void Camera::reset() {
    r        = 5.f;
    fov      = deg2rad(100 / 3.f);
    phi      = 0.f;
    yaw      = 0.f;
    roll     = 0.f;
    pitch    = 0.f;
    theta    = 0.f;
    farClip  = 1000.f;
    nearClip = 1 / 50.f;

    // Prep for using cartesian coordinates later on
    pos      = {0.f, 0.f, -5.f};

    fpsControls = false;
    movementSpeed = 5.f;
    rotationSpeed = .002f;
}