#pragma once
#include <queue>

// https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-mousewheel
#define WHEEL_DELTA 120

class Mouse {
    friend class Window;
public:
    class Event {
    public:
        enum Type {
            LPRESS = 0,
            LRELEASE,
            RPRESS,
            RRELEASE,
            WHEELUP,
            WHEELDOWN,
            MOVE,
            ENTER,
            LEAVE,
            NONE
        };

        Event()
            : type(Type::NONE), leftDown(false), rightDown(false) {}

        Event(Type type, const Mouse& mouse)
            : type(type), leftDown(mouse.leftDown), rightDown(mouse.rightDown), x(mouse.x), y(mouse.y) {}

        Type getType() const { return type; }
        bool valid() const { return type != Type::NONE; }
        int X() const { return x; }
        int Y() const { return y; }
        std::pair<int, int> getPosition() const { return { x, y }; }
        bool LDown() const { return leftDown; }
        bool RDown() const { return rightDown; }
    private:
        int x, y;
        Type type;
        bool leftDown;
        bool rightDown;
    };

public:
    Mouse() = default;
	Mouse( const Mouse& ) = delete;
	Mouse& operator=( const Mouse& ) = delete;

	int X() const { return x; }
	int Y() const { return y; }
    std::pair<int, int> getPosition() const { return { x, y }; }
	bool leftIsPressed() const { return leftDown; }
	bool rightIsPressed() const { return rightDown; }
	bool inClientRegion() const { return cursorInWindow; }
	Mouse::Event read();
	bool empty() const { return eventBuffer.empty(); }
	void flush();

private:
    int x, y;
    bool leftDown = false;
    bool rightDown = false;
    bool cursorInWindow = false;
    int wheelDelta = 0;
    uint32_t bufferSize = 16;
    std::queue<Event> eventBuffer;

    void onMouseEnter();
    void onMouseLeave();
    void onMouseMove(int x,int y);
	void onLeftPressed(int x,int y);
	void onLeftReleased(int x,int y);
	void onRightPressed(int x,int y);
	void onRightReleased(int x,int y);
	void onWheelUp(int x,int y);
	void onWheelDown(int x,int y);
    void onWheelSpin(int x, int y, int delta);
	void trimBuffer();
};