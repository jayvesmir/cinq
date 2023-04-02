#pragma once
#include "ConstantBuffers.hpp"
#include "../Drawable/Drawable.hpp"

class TransformCBuffer : public Bindable {
public:
    TransformCBuffer(Pipeline& pipeline, const Drawable& parent);
    
    void bind(Pipeline& pipeline) override;
protected:
    const Drawable& parent;
    VertexCBuffer<DirectX::XMMATRIX> constantBuffer;
};