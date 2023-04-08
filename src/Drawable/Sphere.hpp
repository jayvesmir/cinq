#pragma once
#include "Geometry.hpp"
#include "DrawableBase.hpp"
#include "Bindable/Bindables.hpp"

class Sphere : public DrawableBase<Sphere> {
public:
    Sphere(Pipeline& pipeline, std::mt19937& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist,
        std::uniform_int_distribution<int>& resXdist,
        std::uniform_int_distribution<int>& resYdist);
    void update(float ts) override;
    DirectX::XMMATRIX getTransformMatrix() const override;
private:
    // Positional
    float roll = 0.f;
    float pitch = 0.f;
    float yaw = 0.f;
    float r, theta, phi, chi;

    // Speed (delta/s)
    float droll, dpitch, dyaw, dtheta, dphi, dchi;
    
    struct Vertex {
        DirectX::XMFLOAT3 pos;
    };

    struct ColorCBuffer {
        struct {
            float r, g, b, a;
        } faceColors[8];
    };

    const ColorCBuffer colorCBuffer { {
            { 1.f,  1.f,  1.f },
            { 1.f,  .2f,  .8f },
            { .5f,  .2f,  .8f },
            { .8f,  .2f,  1.f },
            { 2.f,  1.f,  .8f },
            { 1.f,  .8f,  .2f },
            { 1.f,  .5f,  .2f },
            { 1.f,  .5f,  .5f }
        }
    };

    const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout {
        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
};