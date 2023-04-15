#include "Texture.hpp"

Texture::Texture(Pipeline& pipeline, const Image& image) {
    // Create the descriptor
    D3D11_TEXTURE2D_DESC textureDesc{};
    // Main settings
    textureDesc.Width = image.getWidth();
    textureDesc.Height = image.getHeight();
    textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    // Mipmap settings
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    // Anti-aliasing settings
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    // Flags
    textureDesc.MiscFlags = NULL;
    textureDesc.CPUAccessFlags = NULL;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    // Set the image as the texture
    D3D11_SUBRESOURCE_DATA textureSubData{};
    textureSubData.pSysMem = image.getBuffer();
    textureSubData.SysMemPitch = image.getWidth() * sizeof(Color);

    // Actually create the texture
    wrl::ComPtr<ID3D11Texture2D> texture;
    getDevice(pipeline)->CreateTexture2D(&textureDesc, &textureSubData, &texture);

    // Create a view for the texture
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc{};
    viewDesc.Format = textureDesc.Format;
    viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MipLevels = 1;
    viewDesc.Texture2D.MostDetailedMip = 0;

    getDevice(pipeline)->CreateShaderResourceView(texture.Get(), &viewDesc, &textureView);
}

void Texture::bind(Pipeline& pipeline) {
    getDeviceContext(pipeline)->PSSetShaderResources(0, 1, textureView.GetAddressOf());
}