#include "InputLayout.hpp"

InputLayout::InputLayout(Pipeline& pipeline, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexShader) {
    LOG_ERROR(
        getDevice(pipeline)->CreateInputLayout(
            layout.data(),
            layout.size(),
            vertexShader->GetBufferPointer(),
            vertexShader->GetBufferSize(),
            &inputLayout
    ), "[Input Layout] Failed to create input layout");
}

void InputLayout::bind(Pipeline& pipeline) {
    getDeviceContext(pipeline)->IASetInputLayout(inputLayout.Get());
}