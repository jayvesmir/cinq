#pragma once
#include "Windows.hpp"
#include "Graphics.hpp"

#include <list>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class Pipeline {
public:
    Pipeline(HWND hWnd, int width, int height);
    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    void presentBuffer();
    void clearBuffer(Color color) {
        deviceContext->ClearRenderTargetView(renderTarget.Get(), color.data);
        deviceContext->ClearDepthStencilView(depthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
    }

    void draw(float time, float x, float y);

private:
    int width, height;
    
    wrl::ComPtr<ID3D11Device> device;
    wrl::ComPtr<IDXGISwapChain> swapchain;
    wrl::ComPtr<ID3D11DepthStencilView> depthView;
    wrl::ComPtr<ID3D11DeviceContext> deviceContext;
    wrl::ComPtr<ID3D11RenderTargetView> renderTarget;
};