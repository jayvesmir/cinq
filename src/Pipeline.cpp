#include "Pipeline.hpp"

Pipeline::Pipeline(HWND hWnd, int width, int height) : width(width), height(height){
    DXGI_SWAP_CHAIN_DESC swapchainDesc{};
    swapchainDesc.BufferDesc.Width = 0;
    swapchainDesc.BufferDesc.Height = 0;
    swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchainDesc.BufferDesc.RefreshRate.Numerator = 0;
    swapchainDesc.BufferDesc.RefreshRate.Denominator = 0;
    swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.BufferCount = 1;
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

    wrl::ComPtr<ID3D11Resource> backBuffer = nullptr;
    swapchain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
    device->CreateRenderTargetView(
        backBuffer.Get(),
        nullptr,
        &renderTarget
    );
}

void Pipeline::presentBuffer() {
    swapchain->Present(1, NULL);
}

void Pipeline::draw() {
    struct Point {
        float x, y;
        float r, g, b;
    };

    Point vertices[] = {
        { .5f,  .5f, 1.f, 0.f, 0.f}, // top-right
        { .5f, -.5f, 0.f, 1.f, 0.f}, // bottom-right
        {-.5f, -.5f, 0.f, 0.f, 1.f}, // bottom-left

        {-.5f,  .5f, 0.f, 1.f, 0.f}, // top-left
    };

    uint16_t indices[] = {
        0, 1, 2,
        3, 0, 2
    };

    wrl::ComPtr<ID3D11Buffer> vertexBuffer;
    D3D11_BUFFER_DESC vertexBufferDescriptor{};
    vertexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDescriptor.CPUAccessFlags = NULL;
    vertexBufferDescriptor.MiscFlags = NULL;
    vertexBufferDescriptor.ByteWidth = sizeof(vertices);
    vertexBufferDescriptor.StructureByteStride = sizeof(Point);

    D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
    vertexSubresourceData.pSysMem = vertices;

    wrl::ComPtr<ID3D11Buffer> indexBuffer;
    D3D11_BUFFER_DESC indexBufferDescriptor{};
    indexBufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    indexBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDescriptor.CPUAccessFlags = NULL;
    indexBufferDescriptor.MiscFlags = NULL;
    indexBufferDescriptor.ByteWidth = sizeof(vertices);
    indexBufferDescriptor.StructureByteStride = sizeof(uint16_t);

    D3D11_SUBRESOURCE_DATA indexSubresourceData{};
    indexSubresourceData.pSysMem = indices;

    device->CreateBuffer(&vertexBufferDescriptor, &vertexSubresourceData, vertexBuffer.GetAddressOf());
    device->CreateBuffer(&indexBufferDescriptor, &indexSubresourceData, indexBuffer.GetAddressOf());

    // Bind index buffer
    deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    // Bind vertex buffer
    const uint stride = sizeof(Point);
    const uint offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    wrl::ComPtr<ID3DBlob> blob;
    wrl::ComPtr<ID3D11PixelShader> pixelShader;
    D3DReadFileToBlob(L"shader/pixel.cso", &blob);
    device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());
    deviceContext->PSSetShader(pixelShader.Get(), nullptr, NULL);

    wrl::ComPtr<ID3D11VertexShader> vertexShader;
    D3DReadFileToBlob(L"shader/vertex.cso", &blob);
    device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
    deviceContext->VSSetShader(vertexShader.Get(), nullptr, NULL);

    wrl::ComPtr<ID3D11InputLayout> inputLayout;
    const D3D11_INPUT_ELEMENT_DESC elementDescriptor[] = {
        {"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(float) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    device->CreateInputLayout(elementDescriptor, std::size(elementDescriptor), blob->GetBufferPointer(), blob->GetBufferSize(), inputLayout.GetAddressOf());
    deviceContext->IASetInputLayout(inputLayout.Get());

    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    D3D11_VIEWPORT viewport;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    deviceContext->OMSetRenderTargets(1, renderTarget.GetAddressOf(), nullptr);
    deviceContext->RSSetViewports(1, &viewport);

    deviceContext->DrawIndexed(std::size(indices), 0, 0);
}