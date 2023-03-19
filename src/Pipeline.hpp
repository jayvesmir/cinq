#pragma once

#include "Windows.hpp"
#include <d3d11.h>

class Pipeline {
public:
    Pipeline(HWND hWnd);
    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;
    ~Pipeline() { 
        if (device != nullptr) device->Release();
        if (swapchain != nullptr) swapchain->Release();
        if (deviceContext != nullptr) deviceContext->Release();
    }

    void presentFrame();

private:
    ID3D11Device* device = nullptr;
    IDXGISwapChain* swapchain = nullptr;
    ID3D11DeviceContext* deviceContext = nullptr;
};