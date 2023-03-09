#pragma once
#include "Windows.hpp"

#include <cstdint>

class Window {
private:
    HWND hWnd;
    int width, height;

    static Result CALLBACK handleMsgSetup(HWND hWnd, uint msg, uint64_t uParam, int64_t param) noexcept;
    static Result CALLBACK handleMsgThunk(HWND hWnd, uint msg, uint64_t uParam, int64_t param) noexcept;
    Result handleMsg(HWND hWnd, uint msg, uint64_t uParam, int64_t param) noexcept;
public:
    Window(int width, int height, const char* title) noexcept;
    ~Window() { DestroyWindow(hWnd); }
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

private:
    class WindowClass {
    private:
        HINSTANCE hInstance; 
        static WindowClass windowClass;
        static constexpr const char* className = "Cinq Renderer";

        WindowClass() noexcept;
        ~WindowClass() { UnregisterClass(className, getInstance()); }
        WindowClass(const WindowClass&) = delete;
        WindowClass& operator=(const WindowClass&) = delete;
    public:
        static const char* getName() noexcept { return className; }
        static HINSTANCE getInstance() noexcept { return windowClass.hInstance; }
    };
};