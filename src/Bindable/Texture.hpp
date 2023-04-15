#pragma once

#include "Image.hpp"
#include "Bindable.hpp"

class Texture : public Bindable {
public:
    Texture(Pipeline& pipeline, const Image& image);
    void bind(Pipeline& pipeline) override;

protected:
    wrl::ComPtr<ID3D11ShaderResourceView> textureView;
};