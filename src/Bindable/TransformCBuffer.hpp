#pragma once
#include "ConstantBuffers.hpp"
#include "../Drawable/Drawable.hpp"

class TransformCBuffer : public Bindable {
public:
    TransformCBuffer(Pipeline& pipeline, const Drawable& parent);
    
    void bind(Pipeline& pipeline) override;
protected:
    struct Transforms {
        DirectX::XMMATRIX model;
        DirectX::XMMATRIX modelViewProjection;
    };

    const Drawable& parent;
    static std::unique_ptr<VertexCBuffer<Transforms>> constantBuffer;
};