#pragma once
#include "Imgui.hpp"
#include "Mouse.hpp"
#include "Windows.hpp"
#include "Keyboard.hpp"
#include "Exception.hpp"
#include "Core/Logger.hpp"
#include "Core/Pipeline.hpp"
#include "../res/Resource.hpp"

#include <memory>
#include <optional>

class Window {
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

public:
    class Exception : public CinqException {
    private:
        Result result;

    public:
        Exception(int line, const char* filepath, Result result);
        virtual const char* what() const override;
        virtual const char* getType() const;
        static std::string translateErrorCode(Result code);
        Result getErrorCode() const { return result; };
        const char* getErrorString() const { return translateErrorCode(result).c_str(); }
    };

private:
    HWND window;
    char* title;
    int width, height;
    std::unique_ptr<Pipeline> pipeline;

    static Result CALLBACK handleMsgSetup(HWND hWnd, uint msg, uint64_t uParam, int64_t param) noexcept;
    static Result CALLBACK handleMsgThunk(HWND hWnd, uint msg, uint64_t uParam, int64_t param) noexcept;
    Result handleMsg(HWND hWnd, uint msg, uint64_t uParam, int64_t param) noexcept;
public:
    Mouse mouse;
    Keyboard keyboard;

    Window(int width, int height, const char* title);
    ~Window() { 
        DestroyWindow(window);

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
    }
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void setCursor(int x, int y);
    static void setCursorRaw(int x, int y);
    void setTitle(const char* title = nullptr);
    const char* getTitle() { return title; }
    static bool processMessages(int* exitCode);
    static void hideCursor() { while (ShowCursor(false) >= 0); }
    static void showCursor() { while (ShowCursor(true) < 0); }
    void clipCursor();
    void freeCursor();

    Pipeline& getGraphicsPipeline() { return *pipeline; }
};

#define CINQ_WINDOW_EXCEPT(code) \
    Window::Exception(__LINE__, __FILE__, code)

#define CINQ_LAST_EXCEPT() \
    Window::Exception(__LINE__, __FILE__, GetLastError())