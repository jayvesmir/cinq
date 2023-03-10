#include "Windows.hpp"
#include "Window.hpp"
#include "Exception.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* cmdLine, int cmdShow) {
    try {
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
    } catch (const CinqException& e) {
        MessageBox(nullptr, e.what(), e.getType(), MB_OK | MB_ICONERROR);
    } catch (const std::exception& e) {
        MessageBox(nullptr, e.what(), "Exception", MB_OK | MB_ICONERROR);
    } catch (...) {
        MessageBox(nullptr, "Unknown error", "Unknown Error", MB_OK | MB_ICONERROR);
    }
    
    return -1;
}