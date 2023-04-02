#include "Pipeline.hpp"

Pipeline::Pipeline(HWND hWnd, int width, int height) : width(width), height(height){
    DXGI_SWAP_CHAIN_DESC swapchainDesc{};
    // Front & Back buffer settings
    swapchainDesc.BufferDesc.Width = 0;
    swapchainDesc.BufferDesc.Height = 0;
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapchainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.BufferCount = 1;
    // Anti-aliasing settings
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
    // Other Settings
    swapchainDesc.OutputWindow = hWnd;
    swapchainDesc.Windowed = TRUE;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapchainDesc.Flags = NULL;

    D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        NULL,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapchainDesc,
        swapchain.GetAddressOf(),
        device.GetAddressOf(),
        nullptr,
        deviceContext.GetAddressOf()
    );

    // Create render target
    wrl::ComPtr<ID3D11Resource> backBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
    device->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        &renderTarget
    );

    // Create depth stensil state
    D3D11_DEPTH_STENCIL_DESC depthStencilDescriptor{};
    depthStencilDescriptor.DepthEnable = TRUE;
    depthStencilDescriptor.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDescriptor.DepthFunc = D3D11_COMPARISON_LESS;
    wrl::ComPtr<ID3D11DepthStencilState> depthStencilState;
    device->CreateDepthStencilState(&depthStencilDescriptor, &depthStencilState);

    // Bind depth state
    deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 1);

    // Create depth stencil texture
    wrl::ComPtr<ID3D11Texture2D> depthStencil;
    D3D11_TEXTURE2D_DESC depthTextureDescriptor{};
    depthTextureDescriptor.Width = width;
    depthTextureDescriptor.Height = height;
    depthTextureDescriptor.MipLevels = 1;
    depthTextureDescriptor.ArraySize = 1;
    depthTextureDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
    depthTextureDescriptor.SampleDesc.Count = 1;
    depthTextureDescriptor.SampleDesc.Quality = 0;
    depthTextureDescriptor.Usage = D3D11_USAGE_DEFAULT;
    depthTextureDescriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    device->CreateTexture2D(&depthTextureDescriptor, nullptr, &depthStencil);

    // Create depth stencil
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescriptor{};
    depthStencilViewDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDescriptor.Texture2D.MipSlice = 0;
    device->CreateDepthStencilView(
        depthStencil.Get(),
        &depthStencilViewDescriptor,
        &depthView
    );

    // Bind depth stencil
    deviceContext->OMSetRenderTargets(1, renderTarget.GetAddressOf(), depthView.Get());

    D3D11_VIEWPORT viewport;
    viewport.Width    = (float)width;
    viewport.Height   = (float)height;
    viewport.MinDepth = 0.;
    viewport.MaxDepth = 1.;
    viewport.TopLeftX = 0.;
    viewport.TopLeftY = 0.;
    deviceContext->RSSetViewports(1, &viewport);
}

void Pipeline::presentBuffer() {
    // Changing SyncInterval to 0 will probably make windows bleed at this stage,
    // because the frames take too little time to render and it can't handle renaming the window
    // that fast. || Cinq.cpp ln. 44-47
    swapchain->Present(1, NULL);
    //                ~|~
}

void Pipeline::draw(size_t count) {
    deviceContext->DrawIndexed(count, 0, 0);
}