#pragma once

#include "DrawableBase.hpp"

class Sphere : public DrawableBase<Sphere> {
public:
    Sphere(Pipeline& pipeline, float radius);
    void update(float ts) override;
    void setPosition(DirectX::XMFLOAT3 pos);
    DirectX::XMMATRIX getTransformMatrix() const override;
private:
    struct Vertex {
        DirectX::XMFLOAT3 pos;
    };

    const std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout {
        { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    DirectX::XMFLOAT3 pos = {1.f, 1.f, 1.f};
};