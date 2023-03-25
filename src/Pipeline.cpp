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
    };

    const Point vertices[] = {
        { .5f,  .5f},
        { .5f, -.5f},
        {-.5f, -.5f},

        {-.5f,  .5f},
        { .5f,  .5f},
        {-.5f, -.5f}
    };

    wrl::ComPtr<ID3D11Buffer> vertexBuffer;
    D3D11_BUFFER_DESC bufferDescriptor{};
    bufferDescriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
    bufferDescriptor.CPUAccessFlags = NULL;
    bufferDescriptor.MiscFlags = NULL;
    bufferDescriptor.ByteWidth = sizeof(vertices);
    bufferDescriptor.StructureByteStride = sizeof(Point);

    D3D11_SUBRESOURCE_DATA subresourceData{};
    subresourceData.pSysMem = vertices;

    device->CreateBuffer(&bufferDescriptor, &subresourceData, vertexBuffer.GetAddressOf());
    const uint stride = sizeof(Point);
    const uint offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    wrl::ComPtr<ID3D11InputLayout> inputLayout;
    const D3D11_INPUT_ELEMENT_DESC elementDescriptor[] = {
        {"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    wrl::ComPtr<ID3DBlob> blob;
    wrl::ComPtr<ID3D11PixelShader> pixelShader;
    D3DReadFileToBlob(L"shader/pixel.cso", &blob);
    device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());
    deviceContext->PSSetShader(pixelShader.Get(), nullptr, NULL);

    wrl::ComPtr<ID3D11VertexShader> vertexShader;
    D3DReadFileToBlob(L"shader/vertex.cso", &blob);
    device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
    deviceContext->VSSetShader(vertexShader.Get(), nullptr, NULL);

    device->CreateInputLayout(elementDescriptor, 1, blob->GetBufferPointer(), blob->GetBufferSize(), inputLayout.GetAddressOf());
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

    deviceContext->Draw(6, 0);
}