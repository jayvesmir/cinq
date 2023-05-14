#pragma once

#include "Math.hpp"
#include "Pipeline.hpp"

#include <algorithm>

class Camera {
public:
    Camera();

    float fov;
    float farClip;
    float nearClip;

    void reset();
    void createInterface();
    void rotate(float x, float y);
    void translate(DirectX::XMFLOAT3 offset);
    DirectX::XMMATRIX getTransformMatrix() const;

    inline bool isFPS() const { return fpsControls; }
    inline void setFPS(bool val) { fpsControls = val; }
private:
    float r;
    float phi;
    float yaw;
    float roll;
    float theta;
    float pitch;

    // Prep for using cartesian coordinates later on
    DirectX::XMFLOAT3 pos;

    bool fpsControls;
    float movementSpeed;
    float rotationSpeed;
};