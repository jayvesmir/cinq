#include "TransformCBuffer.hpp"

TransformCBuffer::TransformCBuffer(Pipeline& pipeline, const Drawable& parent) 
    : constantBuffer(pipeline), parent(parent) {}

void TransformCBuffer::bind(Pipeline& pipeline) {
    constantBuffer.update(pipeline, 
        DirectX::XMMatrixTranspose(
            parent.getTransformMatrix() *
            pipeline.getProjection()
        )
    );

    constantBuffer.bind(pipeline);
}