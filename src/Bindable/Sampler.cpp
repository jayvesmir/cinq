#include "Sampler.hpp"

Sampler::Sampler(Pipeline& pipeline) {
    // Create the descriptor
    D3D11_SAMPLER_DESC samplerDesc{};
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.MaxAnisotropy = 16;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;

    // create the sampler
    getDevice(pipeline)->CreateSamplerState(&samplerDesc, &sampler);
}

void Sampler::bind(Pipeline& pipeline) {
    getDeviceContext(pipeline)->PSSetSamplers(0, 1, sampler.GetAddressOf());
}