#pragma once

#include "Bindable.hpp"

class Sampler : public Bindable {
public:
    Sampler(Pipeline& pipeline);
    void bind(Pipeline& pipeline) override;

protected:
    wrl::ComPtr<ID3D11SamplerState> sampler;
};