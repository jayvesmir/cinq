#pragma once
#include "Bindable.hpp"

template<typename T>
class ConstantBuffer : public Bindable {
public:
    ConstantBuffer(Pipeline& pipeline, const T& values) {
        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = NULL;
        bufferDesc.ByteWidth = sizeof(values);
        bufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA subresourceData = {};
        subresourceData.pSysMem = &values;
        LOG_ERROR(
            getDevice(pipeline)->CreateBuffer(&bufferDesc, &subresourceData, &constantBuffer
        ), "[Bindable] Failed to create constant buffer");
    }

    ConstantBuffer(Pipeline& pipeline) {
        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0;
        bufferDesc.ByteWidth = sizeof(T);
        bufferDesc.StructureByteStride = 0;
        LOG_ERROR(
            getDevice(pipeline)->CreateBuffer(&bufferDesc, nullptr, &constantBuffer
        ), "[Bindable] Failed to create constant buffer");
    }

    void update(Pipeline& pipeline, const T& values) {
        D3D11_MAPPED_SUBRESOURCE subresource;
        LOG_ERROR(
            getDeviceContext(pipeline)->Map(
                constantBuffer.Get(), 0,
                D3D11_MAP_WRITE_DISCARD, 0,
                &subresource
        ), "[Bindable] Failed to map constant buffer");
        memcpy(subresource.pData, &values, sizeof(values));
        getDeviceContext(pipeline)->Unmap(constantBuffer.Get(), 0);
    }
protected:
    wrl::ComPtr<ID3D11Buffer> constantBuffer;
};

template<typename T>
class VertexCBuffer : public ConstantBuffer<T> {
    using ConstantBuffer<T>::constantBuffer;
    using Bindable::getDeviceContext;
public:
    using ConstantBuffer<T>::ConstantBuffer;
    void bind(Pipeline& pipeline) override {
        getDeviceContext(pipeline)->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
    }
};

template<typename T>
class PixelCBuffer : public ConstantBuffer<T>{
    using ConstantBuffer<T>::constantBuffer;
    using Bindable::getDeviceContext;
public:
    using ConstantBuffer<T>::ConstantBuffer;
    void bind(Pipeline& pipeline) override {
        getDeviceContext(pipeline)->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
    }
};