#include "VertexShader.hpp"

VertexShader::VertexShader(Pipeline& pipeline, const wchar_t* path) {
    Logger::traceW(std::wstring(L"[Pipeline] Loading vertex shader: ") + path);
    LOG_ERROR_W(
        D3DReadFileToBlob(path, &vsBlob
    ), std::wstring(L"[Pipeline] Invalid file path: {}") + path);
    LOG_ERROR(
        getDevice(pipeline)->CreateVertexShader(
            vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            nullptr,
            &vertexShader
    ), "[Pipeline] Failed to load vertex shader");
}

void VertexShader::bind(Pipeline& pipeline) {
    getDeviceContext(pipeline)->VSSetShader(vertexShader.Get(), nullptr, 0);
}