#pragma once

#include "Sphere.hpp"
#include "Core/Pipeline.hpp"
#include "Bindable/ConstantBuffers.hpp"

class PointLight {
public:
    PointLight(Pipeline& pipeline, float radius = .25f);

    void reset();
    void createInterface();
    void draw(Pipeline& pipeline) const;
    void bind(Pipeline& pipeline) const;
private:
    struct PointLightCBuf {
        DirectX::XMFLOAT3 pos;
        float padding;
    };

    mutable Sphere mesh;
    DirectX::XMFLOAT3 pos = {0.f, 0.f, 0.f};
    mutable PixelCBuffer<PointLightCBuf> cbuf;
};