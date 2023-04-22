#pragma once

#include "Math.hpp"
#include "Pipeline.hpp"

#include <algorithm>

class Camera {
public:
    Camera();

    void reset();
    void createInterface();
    DirectX::XMMATRIX getTransformMatrix() const;

    float getFarClip() const { return farClip; }
    float getNearClip() const { return nearClip; }

private:
    float r;
    float phi;
    float yaw;
    float roll;
    float pitch;
    float theta;
    float farClip;
    float nearClip;

    // Prep for using cartesian coordinates later on
    DirectX::XMFLOAT3 pos;
};