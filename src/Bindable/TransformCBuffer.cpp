#include "TransformCBuffer.hpp"

TransformCBuffer::TransformCBuffer(Pipeline& pipeline, const Drawable& parent) 
    : parent(parent) {
    if (!constantBuffer) {
        constantBuffer = std::make_unique<VertexCBuffer<Transforms>>(pipeline);
    }
}

void TransformCBuffer::bind(Pipeline& pipeline) {
    auto model = parent.getTransformMatrix();
    const Transforms tf = {
        DirectX::XMMatrixTranspose(model),
        DirectX::XMMatrixTranspose(
            model                       *
            pipeline.getCamera()        *
            pipeline.getProjection()
        )
    };

    constantBuffer->update(pipeline, tf);
    constantBuffer->bind(pipeline);
}

std::unique_ptr<VertexCBuffer<TransformCBuffer::Transforms>> TransformCBuffer::constantBuffer;