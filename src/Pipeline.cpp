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
    swapchainDesc.SampleDesc.Count = 8;
    swapchainDesc.SampleDesc.Quality = DXGI_STANDARD_MULTISAMPLE_QUALITY_PATTERN;
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

    wrl::ComPtr<ID3D11Resource> backBuffer;
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

// Most of this is temporary test code :)
void Pipeline::draw(float time, float x, float y) {
    struct Point {
        float x, y, z;
    };

    Point vertices[] = {
        {-1.f, -1.f, -1.f},
        { 1.f, -1.f, -1.f},
        {-1.f,  1.f, -1.f},
        {-1.f, -1.f,  1.f},
        { 1.f,  1.f, -1.f},
        { 1.f, -1.f,  1.f},
        {-1.f,  1.f,  1.f},
        { 1.f,  1.f,  1.f}
    };

    uint16_t indices[] = {
        0, 2, 1,   2, 4, 1,
        1, 4, 5,   4, 7, 5,
        2, 6, 4,   4, 6, 7,
        3, 5, 7,   3, 7, 6,
        0, 3, 2,   2, 3, 6,
        0, 1, 3,   1, 5, 3
    };

    // Both of these are constant buffers
    struct TransformBuffer {
        DirectX::XMMATRIX transform;
    };

    struct ColorBuffer {
        struct {
            float r, g, b, a;
        } faceColors[6];
    };

    ColorBuffer colorBuffer = {
        {
            {.2f, .2f, .2f},
            {.4f, .4f, .4f},
            {.5f, .5f, .5f},
            {.7f, .7f, .7f},
            {.8f, .8f, .8f},
            {.9f, .9f, .9f},
        }
    };

    float angle = time * 1.5f;
    float aspectRatio = height / (float)width;

    TransformBuffer tranformBuffer = {
        {
            DirectX::XMMatrixTranspose(                                       // Transpose matrix into column major
                DirectX::XMMatrixRotationZ(angle)                           * // Rotate square along Z
                DirectX::XMMatrixRotationX(angle)                           * // Rotate square along X
                DirectX::XMMatrixRotationY(angle)                           * // Rotate square along Y
                DirectX::XMMatrixTranslation(.0f, .0f, 6.f)                 * // Move the square in front of the camera
                DirectX::XMMatrixPerspectiveLH(1.f, aspectRatio, .5f, 10.f) * // Perspective magic
                DirectX::XMMatrixTranslation(x, -y, 0.f)                      // Move the square to x, y
            )
        }
    };

    // Create vertex buffer
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

    // Create index buffer
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

    // Create constant buffers
    wrl::ComPtr<ID3D11Buffer> tranformConstantBuffer;
    D3D11_BUFFER_DESC transformConstantBufferDescriptor{};
    transformConstantBufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    transformConstantBufferDescriptor.Usage = D3D11_USAGE_DYNAMIC;
    transformConstantBufferDescriptor.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    transformConstantBufferDescriptor.MiscFlags = NULL;
    transformConstantBufferDescriptor.ByteWidth = sizeof(tranformBuffer);
    transformConstantBufferDescriptor.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA transformConstantSubresourceData{};
    transformConstantSubresourceData.pSysMem = &tranformBuffer;

    wrl::ComPtr<ID3D11Buffer> colorConstantBuffer;
    D3D11_BUFFER_DESC colorConstantBufferDescriptor{};
    colorConstantBufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    colorConstantBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
    colorConstantBufferDescriptor.CPUAccessFlags = NULL;
    colorConstantBufferDescriptor.MiscFlags = NULL;
    colorConstantBufferDescriptor.ByteWidth = sizeof(colorBuffer);
    colorConstantBufferDescriptor.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA colorConstantSubresourceData{};
    colorConstantSubresourceData.pSysMem = &colorBuffer;

    // Create the buffers but in a different kinda way ykwim
    device->CreateBuffer(&vertexBufferDescriptor, &vertexSubresourceData, vertexBuffer.GetAddressOf());
    device->CreateBuffer(&indexBufferDescriptor, &indexSubresourceData, indexBuffer.GetAddressOf());
    device->CreateBuffer(&transformConstantBufferDescriptor, &transformConstantSubresourceData, tranformConstantBuffer.GetAddressOf());
    device->CreateBuffer(&colorConstantBufferDescriptor, &colorConstantSubresourceData, colorConstantBuffer.GetAddressOf());

    // Bind index buffer
    deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

    // Bind vertex buffer
    const uint stride = sizeof(Point);
    const uint offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

    // Bind constant buffers
    deviceContext->VSSetConstantBuffers(0, 1, tranformConstantBuffer.GetAddressOf());
    deviceContext->PSSetConstantBuffers(0, 1, colorConstantBuffer.GetAddressOf());

    // Read & Set shaders
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
        {"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
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