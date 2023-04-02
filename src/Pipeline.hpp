#pragma once
#include "Windows.hpp"

#include <list>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class Pipeline {
    friend class Bindable;
public:
    Pipeline(HWND hWnd, int width, int height);
    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    void presentBuffer();
    void clearBuffer(const float color[4]) {
        deviceContext->ClearRenderTargetView(renderTarget.Get(), color);
        deviceContext->ClearDepthStencilView(depthView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
    }

    void draw(size_t count);
    void setProjection(DirectX::XMMATRIX projection) { this->projection = projection; }
    DirectX::XMMATRIX getProjection() const { return projection; }

private:
    int width, height;
    
    DirectX::XMMATRIX projection;
    wrl::ComPtr<ID3D11Device> device;
    wrl::ComPtr<IDXGISwapChain> swapchain;
    wrl::ComPtr<ID3D11DepthStencilView> depthView;
    wrl::ComPtr<ID3D11DeviceContext> deviceContext;
    wrl::ComPtr<ID3D11RenderTargetView> renderTarget;
};