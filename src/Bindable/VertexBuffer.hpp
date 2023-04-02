#pragma once
#include "Bindable.hpp"

class VertexBuffer : public Bindable {
public:
    template<class T>
    VertexBuffer(Pipeline& pipeline, const std::vector<T>& vertices) 
        : stride(sizeof(T)) {
            
        // Create the buffer
        D3D11_BUFFER_DESC bufferDesc{};
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.CPUAccessFlags = NULL;
        bufferDesc.MiscFlags = NULL;
        bufferDesc.ByteWidth = sizeof(T) * vertices.size();
        bufferDesc.StructureByteStride = sizeof(T);

        D3D11_SUBRESOURCE_DATA subresourceData{};
        subresourceData.pSysMem = vertices.data();

        getDevice(pipeline)->CreateBuffer(&bufferDesc, &subresourceData, &vertexBuffer);
    }
    void bind(Pipeline& pipeline) override;
protected:
    uint stride;
    const uint offset = 0;
    wrl::ComPtr<ID3D11Buffer> vertexBuffer;
};