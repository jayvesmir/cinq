#include "Core/Cinq.hpp"
#include "Core/Logger.hpp"
#include "Window/Window.hpp"
#include "Window/Windows.hpp"
#include "Window/Exception.hpp"

// TODO: More/better error handling

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char *cmdLine, int cmdShow) {
    const int width = 1600;
    const float aspectRatio = 2 / 1.;
    const int height = width / aspectRatio;
    
    try {
        Cinq cinq(width, height, "Cinq");
        cinq.run();
    } catch (const CinqException &e) {
        MessageBox(nullptr, e.what(), e.getType(), MB_OK | MB_ICONERROR);
    } catch (const std::exception &e) {
        MessageBox(nullptr, e.what(), "Exception", MB_OK | MB_ICONERROR);
    } catch (...) {
        MessageBox(nullptr, "Unknown error", "Unknown Error", MB_OK | MB_ICONERROR);
    }
    
    return -1;
}
