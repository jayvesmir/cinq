#include "Cinq.hpp"
#include "Window.hpp"
#include "Windows.hpp"
#include "Exception.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* cmdLine, int cmdShow) {
    Cinq cinq(1600, 900, "Cinq");
    try {
        cinq.run();
    } catch (const CinqException& e) {
        MessageBox(nullptr, e.what(), e.getType(), MB_OK | MB_ICONERROR);
    } catch (const std::exception& e) {
        MessageBox(nullptr, e.what(), "Exception", MB_OK | MB_ICONERROR);
    } catch (...) {
        MessageBox(nullptr, "Unknown error", "Unknown Error", MB_OK | MB_ICONERROR);
    }
    
    return -1;
}