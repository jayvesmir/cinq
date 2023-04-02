#include "VertexShader.hpp"

VertexShader::VertexShader(Pipeline& pipeline, const wchar_t* path) {
    D3DReadFileToBlob(path, &vsBlob);
    getDevice(pipeline)->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &vertexShader
    );
}

void VertexShader::bind(Pipeline& pipeline) {
    getDeviceContext(pipeline)->VSSetShader(vertexShader.Get(), nullptr, 0);
}