#pragma once
#include "Bindable.hpp"

class PixelShader : public Bindable {
public:
    PixelShader(Pipeline& pipeline, const wchar_t* path);
    
    void bind(Pipeline& pipeline) override;
    ID3DBlob* getBytecode() const { return psBlob.Get(); }
protected:
    wrl::ComPtr<ID3DBlob> psBlob;
    wrl::ComPtr<ID3D11PixelShader> pixelShader;
};