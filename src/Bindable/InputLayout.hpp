#pragma once
#include "Bindable.hpp"

class InputLayout : public Bindable {
public:
    InputLayout(Pipeline& pipeline, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vertexShader);
    
    void bind(Pipeline& pipeline) override;
protected:
    wrl::ComPtr<ID3D11InputLayout> inputLayout;
};