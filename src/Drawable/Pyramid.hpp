#pragma once
#include "DrawableBase.hpp"
#include "Core/Geometry.hpp"
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
    float roll   {0.f};
    float pitch  {0.f};
    float yaw    {0.f};
    float r      {0.f};
    float theta  {0.f};
    float phi    {0.f};
    float chi    {0.f};

    // Speed (delta/s)
    float droll  {0.f};
    float dpitch {0.f};
    float dyaw   {0.f};
    float dtheta {0.f};
    float dphi   {0.f};
    float dchi   {0.f};

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