#include "PixelShader.hpp"

PixelShader::PixelShader(Pipeline& pipeline, const wchar_t* path) {
    D3DReadFileToBlob(path, &psBlob);
    getDevice(pipeline)->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &pixelShader
    );
}

void PixelShader::bind(Pipeline& pipeline) {
    getDeviceContext(pipeline)->PSSetShader(pixelShader.Get(), nullptr, 0);
}