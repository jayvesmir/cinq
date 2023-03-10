#pragma once
#include "Windows.hpp"
#include "Exception.hpp"
#include "../res/Resource.hpp"

class Window {
private:
    HWND hWnd;
    int width, height;

    static Result CALLBACK handleMsgSetup(HWND hWnd, uint msg, uint64_t uParam, int64_t param) noexcept;
    static Result CALLBACK handleMsgThunk(HWND hWnd, uint msg, uint64_t uParam, int64_t param) noexcept;
    Result handleMsg(HWND hWnd, uint msg, uint64_t uParam, int64_t param) noexcept;
public:
    Window(int width, int height, const char* title);
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
};

#define CINQ_WINDOW_EXCEPT(code) \
    Window::Exception(__LINE__, __FILE__, code)

#define CINQ_LAST_EXCEPT() \
    Window::Exception(__LINE__, __FILE__, GetLastError())