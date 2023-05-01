#pragma once

#include "Math.hpp"
#include "Pipeline.hpp"

#include <algorithm>

class Camera {
public:
    Camera();

    void reset();
    void createInterface();
    void rotate(float x, float y);
    void translate(DirectX::XMFLOAT3 offset);
    DirectX::XMMATRIX getTransformMatrix() const;

    bool isMoving() const { return moving; }
    void setMoving(bool val) { moving = val; }
    float getFarClip() const { return farClip; }
    float getNearClip() const { return nearClip; }

private:
    float r;
    float phi;
    float yaw;
    float roll;
    float theta;
    float pitch;
    float farClip;
    float nearClip;

    // Prep for using cartesian coordinates later on
    DirectX::XMFLOAT3 pos;

    bool moving;
    float movementSpeed;
    float rotationSpeed;
};