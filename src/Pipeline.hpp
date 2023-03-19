#pragma once
#include "Windows.hpp"
#include <d3d11.h>
#include "Graphics.hpp"

class Pipeline {
public:
    Pipeline(HWND hWnd);
    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;
    ~Pipeline() { 
        if (device) device->Release();
        if (swapchain) swapchain->Release();
        if (deviceContext) deviceContext->Release();
        if (renderTarget) renderTarget->Release();
    }

    void presentBuffer();
    void clearBuffer(Color color) {
        deviceContext->ClearRenderTargetView(renderTarget, color.data);
    }

private:
    ID3D11Device* device = nullptr;
    IDXGISwapChain* swapchain = nullptr;
    ID3D11DeviceContext* deviceContext = nullptr;
    ID3D11RenderTargetView* renderTarget = nullptr;
};