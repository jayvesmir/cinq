#include "Window.hpp"

Window::WindowClass Window::WindowClass::windowClass;

Window::WindowClass::WindowClass() noexcept 
    : hInstance(GetModuleHandle(nullptr)) {
    // Create window class
    WNDCLASSEX wc;
    wc.hInstance = hInstance;
    wc.hIcon = (HICON)(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 32, 32, 0));
    wc.hIconSm = (HICON)(LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 16, 16, 0));; // It looks like total garbage but I like it :3
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

Window::Window(int width, int height, const char* title) {
    // Compute the real size of the window 
    // so that the client region size matches
    // the width and height specified.
    RECT windowRect;
    windowRect.left = 100;
    windowRect.right = width + windowRect.left;
    windowRect.top = 100;
    windowRect.bottom = height + windowRect.top;
    if (FAILED(AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)))
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