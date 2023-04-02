#include "IndexBuffer.hpp"

IndexBuffer::IndexBuffer(Pipeline& pipeline, const std::vector<uint16_t>& indices) 
    : count((uint)indices.size()){

    // Create the buffer
    D3D11_BUFFER_DESC indexBufferDesc{};
    indexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.CPUAccessFlags = NULL;
    indexBufferDesc.MiscFlags = NULL;
    indexBufferDesc.ByteWidth = count * sizeof(uint16_t);
    indexBufferDesc.StructureByteStride = sizeof(uint16_t);

    D3D11_SUBRESOURCE_DATA indexSubresourceData{};
    indexSubresourceData.pSysMem = indices.data();
    getDevice(pipeline)->CreateBuffer(&indexBufferDesc, &indexSubresourceData, &indexBuffer);
}

void IndexBuffer::bind(Pipeline& pipeline) {
    getDeviceContext(pipeline)->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
}

uint IndexBuffer::getCount() const {
    return count;
}