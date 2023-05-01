#pragma once
#include "Core/Math.hpp"
#include "DrawableBase.hpp"
#include "Core/Geometry.hpp"
#include "Bindable/Bindables.hpp"

class Cube : public DrawableBase<Cube> {
public:
    Cube(Pipeline& pipeline, std::mt19937& rng,
        std::uniform_real_distribution<float>& adist,
        std::uniform_real_distribution<float>& ddist,
        std::uniform_real_distribution<float>& odist,
        std::uniform_real_distribution<float>& rdist,
        std::uniform_real_distribution<float>& bdist );
    Cube(Pipeline& pipeline, float r, const char* textureFP = "res/cube.png");
    void update(float ts) override;
    DirectX::XMMATRIX getTransformMatrix() const override;
private:
    // Positional
    float roll   {deg2rad(180.f)};
    float pitch  {0.f};
    float yaw    {0.f};
    
    float r      {0.f};
    float theta  {0.f};
    float phi    {0.f};
    float chi    {0.f};

    float y      {0.f};
    float z      {0.f};

    // Speed (delta/s)
    float droll  {0.f};
    float dpitch {0.f};
    float dyaw   {0.f};
    float dtheta {0.f};
    float dphi   {0.f};
    float dchi   {0.f};

    struct Vertex {
        DirectX::XMFLOAT3 pos;
        struct {
            float u, v;
        } texture;
    };

    const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout {
        { "Position",          0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TextureCoordinate", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };
};