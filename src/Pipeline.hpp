#pragma once
#include "Windows.hpp"
#include "Graphics.hpp"

#include <d3d11.h>
#include <d3dcompiler.h>

namespace wrl = Microsoft::WRL;

class Pipeline {
public:
    Pipeline(HWND hWnd, int width, int height);
    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    void presentBuffer();
    void clearBuffer(Color color) {
        deviceContext->ClearRenderTargetView(renderTarget.Get(), color.data);
    }

    void draw();

private:
    wrl::ComPtr<ID3D11Device>           device;
    wrl::ComPtr<IDXGISwapChain>         swapchain;
    wrl::ComPtr<ID3D11DeviceContext>    deviceContext;
    wrl::ComPtr<ID3D11RenderTargetView> renderTarget;

    int width, height;
};