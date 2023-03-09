#include "Window.hpp"

Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass() noexcept 
    : hInstance(GetModuleHandle(nullptr)) {
    // Create window class
    WNDCLASSEX wc;
    wc.hInstance = hInstance;
    wc.hIcon = nullptr;
    wc.hIconSm = nullptr;
    wc.hCursor = nullptr;
    wc.hbrBackground = nullptr;
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = handleMsgSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = className;
    RegisterClassEx(&wc);
}

Window::Window(int width, int height, const char* title) noexcept {
    // Compute the real size of the window 
    // so that the client region size matches
    // the width and height specified.
    RECT windowRect;
    windowRect.left = 100;
    windowRect.right = width + windowRect.left;
    windowRect.top = 100;
    windowRect.bottom = height + windowRect.top;
    AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

    // Create window
    hWnd = CreateWindowEx(
        0,
        WindowClass::getName(),                     // Window class
        title,                                      // Window text
        WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU,   // Window style
        // Position
        CW_USEDEFAULT, CW_USEDEFAULT,
        // Size
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        nullptr,                                    // Parent window    
        nullptr,                                    // Menu
        WindowClass::getInstance(),                 // Instance handle
        this                                        // Additional application data
    );

    ShowWindow(hWnd, SW_SHOWDEFAULT);
}

Result CALLBACK Window::handleMsgSetup(HWND hWnd, uint msg, uint64_t uParam, int64_t param) noexcept {
    if (msg == WM_NCCREATE) {
        const CREATESTRUCT* const createStruct = reinterpret_cast<CREATESTRUCT*>(param);
        Window* const window = (Window*)(createStruct->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<uint64_t>(window));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<uint64_t>(&Window::handleMsgThunk));
        return window->handleMsg(hWnd, msg, uParam, param);
    }
    return DefWindowProc(hWnd, msg, uParam, param);
}

Result CALLBACK Window::handleMsgThunk(HWND hWnd, uint msg, uint64_t uParam, int64_t param) noexcept {
    Window* const window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return window->handleMsg(hWnd, msg, uParam, param);
}

Result Window::handleMsg(HWND hWnd, uint msg, uint64_t uParam, int64_t param) noexcept {
    switch (msg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, msg, uParam, param);
}