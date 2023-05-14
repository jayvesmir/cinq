#include "Pipeline.hpp"

Pipeline::Pipeline(HWND hWnd, int width, int height) : width(width), height(height) {
    DXGI_SWAP_CHAIN_DESC swapchainDesc{};
    // Front & Back buffer settings
    swapchainDesc.BufferDesc.Width = width;
    swapchainDesc.BufferDesc.Height = height;
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapchainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.BufferCount = 1;
    // Anti-aliasing settings
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;
    // Other Settings
    swapchainDesc.OutputWindow = hWnd;
    swapchainDesc.Windowed = TRUE;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapchainDesc.Flags = NULL;

    Logger::trace("[Pipeline] Creating Device and Swapchain");

    IDXGIFactory* dxgiFactory;
    LOG_ERROR(
        CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFactory),
        "[Pipeline] Failed to create adapter factory");
    dxgiFactory->EnumAdapters(0, &adapter);
    dxgiFactory->Release();
    
    LOG_ERROR(
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
    ), "[Pipeline] Failed to create device and swapchain");

    DXGI_ADAPTER_DESC adapterDesc;
    adapter->GetDesc(&adapterDesc);
    Logger::infoW(std::format(L"[Pipeline] Using adapter: {} (id: {})", adapterDesc.Description, adapterDesc.DeviceId));

    Logger::trace(std::format("[Pipeline] Creating render targets"));

    // Create render target
    wrl::ComPtr<ID3D11Resource> backBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
    LOG_ERROR(
        device->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        &renderTarget
    ), "[Pipeline] Failed to create main render target");

    // Create depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
    depthStencilDesc.DepthEnable = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    wrl::ComPtr<ID3D11DepthStencilState> depthStencilState;
    LOG_ERROR(
        device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState
    ), "[Pipeline] Failed to create depth stencil state");

    // Bind depth state
    deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 1);

    // Create depth stencil texture
    wrl::ComPtr<ID3D11Texture2D> depthStencil;
    D3D11_TEXTURE2D_DESC depthTextureDesc{};
    depthTextureDesc.Width = width;
    depthTextureDesc.Height = height;
    depthTextureDesc.MipLevels = 1;
    depthTextureDesc.ArraySize = 1;
    depthTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthTextureDesc.SampleDesc.Count = swapchainDesc.SampleDesc.Count;
    depthTextureDesc.SampleDesc.Quality = swapchainDesc.SampleDesc.Quality;
    depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
    depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    LOG_ERROR(
        device->CreateTexture2D(&depthTextureDesc, nullptr, &depthStencil
    ), "[Pipeline] Failed to create depth stencil buffer");

    // Create depth stencil
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
    depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;
    LOG_ERROR(
    device->CreateDepthStencilView(
        depthStencil.Get(),
        &depthStencilViewDesc,
        &depthView
    ), "[Pipeline] Failed to create depth stencil view");

    // Bind depth stencil
    deviceContext->OMSetRenderTargets(1, renderTarget.GetAddressOf(), depthView.Get());

    D3D11_VIEWPORT viewport;
    viewport.Width    = (float)width;
    viewport.Height   = (float)height;
    viewport.MinDepth = 0.;
    viewport.MaxDepth = 1.;
    viewport.TopLeftX = 0.;
    viewport.TopLeftY = 0.;
    Logger::trace(std::format("[Pipeline] Creating viewport"));
    deviceContext->RSSetViewports(1, &viewport);

    // Init ImGui Renderer
    Logger::trace(std::format("[Pipeline] Initializing ImGui (ImplDX11)"));
    ImGui_ImplDX11_Init(device.Get(), deviceContext.Get());
}

void Pipeline::presentBuffer() {
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    swapchain->Present(vsync, NULL);
}

void Pipeline::draw(size_t count) {
    deviceContext->DrawIndexed(count, 0, 0);
}