#pragma once

#include "Pipeline.hpp"

class Camera {
public:
    DirectX::XMMATRIX getTransformMatrix() const;
    void reset();

    float roll  = 0.f;
    float pitch = 0.f;
    float yaw   = 0.f;
    float r     = 5.f;
    float theta = 0.f;
    float phi   = 0.f;
    float chi   = 0.f;
};