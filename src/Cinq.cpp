#include "Windows.hpp"
#include "Window.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* cmdLine, int cmdShow) {
    Window window(1280, 720, "Cinq");

    // Process messages
    MSG msg;
    int msgResult;
    while ((msgResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msgResult == -1);
        return msgResult;
    return msg.wParam;
}