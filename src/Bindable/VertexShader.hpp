#pragma once
#include "Bindable.hpp"

class VertexShader : public Bindable {
public:
    VertexShader(Pipeline& pipeline, const wchar_t* path);
    
    void bind(Pipeline& pipeline) override;
    ID3DBlob* getBytecode() const { return vsBlob.Get(); }
protected:
    wrl::ComPtr<ID3DBlob> vsBlob;
    wrl::ComPtr<ID3D11VertexShader> vertexShader;
};