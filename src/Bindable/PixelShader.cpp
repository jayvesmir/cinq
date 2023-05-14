#include "PixelShader.hpp"

PixelShader::PixelShader(Pipeline& pipeline, const wchar_t* path) {
    Logger::traceW(std::wstring(L"[Pipeline] Loading pixel shader: ") + path);
    LOG_ERROR_W(
        D3DReadFileToBlob(path, &psBlob
    ), std::wstring(L"[Pipeline] Invalid file path: {}") + path);
    LOG_ERROR(
        getDevice(pipeline)->CreatePixelShader(
            psBlob->GetBufferPointer(),
            psBlob->GetBufferSize(),
            nullptr,
            &pixelShader
    ), "[Pipeline] Failed to load pixel shader");
}

void PixelShader::bind(Pipeline& pipeline) {
    getDeviceContext(pipeline)->PSSetShader(pixelShader.Get(), nullptr, 0);
}