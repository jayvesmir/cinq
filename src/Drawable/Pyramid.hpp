#pragma once
#include "Geometry.hpp"
#include "DrawableBase.hpp"
#include "Bindable/Bindables.hpp"

class Pyramid : public DrawableBase<Pyramid> {
public:
    Pyramid(Pipeline& pipeline, std::mt19937& rng,
            std::uniform_real_distribution<float>& adist,
            std::uniform_real_distribution<float>& ddist,
            std::uniform_real_distribution<float>& odist,
            std::uniform_real_distribution<float>& rdist);
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

        // Point color
        struct {
            float r, g, b, a;
        } color;
    };

    const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout {
        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "Color"   , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
};