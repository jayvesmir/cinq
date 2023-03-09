#include <stdexcept>
#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* cmdLine, int cmdShow) {
    const char* className = "Cinq";

    // Register the window class
    WNDCLASSEX windowClass;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = nullptr;
    windowClass.hIconSm = nullptr;
    windowClass.hCursor = nullptr;
    windowClass.hbrBackground = nullptr;
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_OWNDC;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = className;
    RegisterClassEx(&windowClass);

    // Create the window
    HWND hWnd = CreateWindowEx(
        0,                                          // Optional window styles
        className,                                  // Window class
        className,                                  // Window text
        WS_MINIMIZEBOX | WS_CAPTION | WS_SYSMENU,   // Window style
        // Position and size
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        nullptr,                                    // Parent window    
        nullptr,                                    // Menu
        hInstance,                                  // Instance handle
        nullptr                                     // Additional application data
    );

    if (hWnd == NULL) {
        throw std::runtime_error("Couldn't create window");
    }

    // Show the window
    ShowWindow(hWnd, SW_SHOW);

    // Process Messages
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}