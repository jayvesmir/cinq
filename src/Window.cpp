#include "Window.hpp"

Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass() noexcept 
    : hInstance(GetModuleHandle(nullptr)) {
    // Create window class
    WNDCLASSEX windowClass;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = (HICON)(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 32, 32, 0));
    windowClass.hIconSm = (HICON)(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 16, 16, 0));; // It looks like total garbage but I like it :3
    windowClass.hCursor = nullptr;
    windowClass.hbrBackground = nullptr;
    windowClass.cbSize = sizeof(windowClass);
    windowClass.style = CS_OWNDC;
    windowClass.lpfnWndProc = handleMsgSetup;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = className;
    RegisterClassEx(&windowClass);
}

Window::Window(int width, int height, const char* title) 
    : width(width), height(height) {
    // Compute the real size of the window 
    // so that the client region size matches
    // the width and height specified.
    RECT windowRect;
    windowRect.left = 100;
    windowRect.right = width + windowRect.left;
    windowRect.top = 100;
    windowRect.bottom = height + windowRect.top;
    if (AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == FALSE)
        throw CINQ_LAST_EXCEPT();

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

    if (!hWnd)
        throw CINQ_LAST_EXCEPT();

    ShowWindow(hWnd, SW_SHOWDEFAULT);

    pipeline = std::make_unique<Pipeline>(hWnd, width, height);
}

void Window::setTitle(const char* title) {
    this->title = (char*)title;
    if (title) {
        if (SetWindowText(hWnd, title) == FALSE)
            throw CINQ_LAST_EXCEPT();
    }
}

bool Window::processMessages(int* exitCode) {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            *exitCode = (int)msg.wParam;
            return true;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return false;
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
            
        // Keyboard Messages
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            // Check if the key was in a down state before this event
            // in order to filter out autorepeated key presses.
            if (!(param & 0x40000000) || keyboard.autorepeatIsEnabled())
                keyboard.onKeyPressed((uint8_t)uParam);
            break;
        case WM_KEYUP:
        case WM_SYSKEYUP:
            keyboard.onKeyReleased((uint8_t)uParam);
            break;
        case WM_CHAR:
            keyboard.onChar((uint8_t)uParam);
            break;
        case WM_KILLFOCUS:
            keyboard.clearState();
            break;
        
        // Mouse Messages
        case WM_MOUSEMOVE: {
            POINTS pts = MAKEPOINTS(param);
            // if in client region register mouse move and and capture mouse
            if (pts.x >= 0 && pts.x < width && pts.y >= 0 && pts.y < height) {
                mouse.onMouseMove(pts.x, pts.y);
                if (!mouse.inClientRegion()) {
                    SetCapture(hWnd);
                    mouse.onMouseEnter();
                }
            } else {
                // if left or right buttons are down register mouse move
                if (uParam & (MK_LBUTTON | MK_RBUTTON))
                    mouse.onMouseMove(pts.x, pts.y);
                else {
                    ReleaseCapture();
                    mouse.onMouseLeave();
                }
            }
            break;
        }
        case WM_LBUTTONDOWN: {
            POINTS pts = MAKEPOINTS(param);
            mouse.onLeftPressed(pts.x, pts.y);
            break;
        }
        case WM_LBUTTONUP: {
            POINTS pts = MAKEPOINTS(param);
            mouse.onLeftReleased(pts.x, pts.y);
            break;
        }
        case WM_RBUTTONDOWN: {
            POINTS pts = MAKEPOINTS(param);
            mouse.onRightPressed(pts.x, pts.y);
            break;
        }
        case WM_RBUTTONUP: {
            POINTS pts = MAKEPOINTS(param);
            mouse.onRightReleased(pts.x, pts.y);
            break;
        }
        case WM_MOUSEWHEEL: {
            POINTS pts = MAKEPOINTS(param);
            mouse.onWheelSpin(pts.x, pts.y, GET_WHEEL_DELTA_WPARAM(uParam));
        }
    }

    return DefWindowProc(hWnd, msg, uParam, param);
}

Window::Exception::Exception(int line, const char* filepath, Result result)
    : CinqException(line, filepath), result(result) {}

const char* Window::Exception::what() const {
    char format[512];
    sprintf(format, "%s thrown in %s\n\n[Code] %lld\n[Description] %s", getType(), getOriginString().c_str(), getErrorCode(), getErrorString());
    whatBuffer = std::string(format);
    return whatBuffer.c_str();
}

const char* Window::Exception::getType() const {
    return "Cinq Window Exception";
}

// https://github.com/planetchili/hw3d/blob/master/hw3d/Window.cpp
std::string Window::Exception::translateErrorCode(Result code) {
    char* pMsgBuf = nullptr;
    // windows will allocate memory for err string and make our pointer point to it
    const DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,code,MAKELANGID( LANG_NEUTRAL,SUBLANG_DEFAULT ),
        reinterpret_cast<LPSTR>(&pMsgBuf),0,nullptr
    );
    // 0 string length returned indicates a failure
    if( nMsgLen == 0 )
    {
        return "Unidentified error code";
    }
    // copy error string from windows-allocated buffer to std::string
    std::string errorString = pMsgBuf;
    // free windows buffer
    LocalFree( pMsgBuf );
    return errorString;
}