#include "TransformCBuffer.hpp"

TransformCBuffer::TransformCBuffer(Pipeline& pipeline, const Drawable& parent) 
    : parent(parent) {
    if (!constantBuffer) {
        constantBuffer = std::make_unique<VertexCBuffer<DirectX::XMMATRIX>>(pipeline);
    }
}

void TransformCBuffer::bind(Pipeline& pipeline) {
    constantBuffer->update(pipeline, 
        DirectX::XMMatrixTranspose(
            parent.getTransformMatrix() *
            pipeline.getProjection()
        )
    );

    constantBuffer->bind(pipeline);
}

std::unique_ptr<VertexCBuffer<DirectX::XMMATRIX>> TransformCBuffer::constantBuffer;