#include "VertexBuffer.hpp"

void VertexBuffer::bind(Pipeline& pipeline) {
    getDeviceContext(pipeline)->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
}