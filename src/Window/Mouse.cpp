#include "Mouse.hpp"

Mouse::Event Mouse::read() {
    if (eventBuffer.size() > 0) {
        Mouse::Event e = eventBuffer.front();
        eventBuffer.pop();
        return e;
    }

    return Mouse::Event();
}

void Mouse::flush() {
    eventBuffer = std::queue<Event>();
}

void Mouse::onMouseMove(int x_, int y_) {
    x = x_;
    y = y_;

    eventBuffer.push(Mouse::Event(Mouse::Event::Type::MOVE, *this));
    trimBuffer();
}

void Mouse::onMouseEnter() {
    cursorInWindow = true;
    eventBuffer.push(Mouse::Event(Mouse::Event::Type::ENTER, *this));
    trimBuffer();
}

void Mouse::onMouseLeave() {
    cursorInWindow = false;
    eventBuffer.push(Mouse::Event(Mouse::Event::Type::LEAVE, *this));
    trimBuffer();
}

void Mouse::onLeftPressed(int x, int y) {
    leftDown = true;
    eventBuffer.push(Mouse::Event(Mouse::Event::Type::LPRESS, *this));
    trimBuffer();
}

void Mouse::onLeftReleased(int x, int y) {
    leftDown = false;
    eventBuffer.push(Mouse::Event(Mouse::Event::Type::LRELEASE,*this));
    trimBuffer();
}

void Mouse::onRightPressed(int x, int y) {
    rightDown = true;
    eventBuffer.push(Mouse::Event(Mouse::Event::Type::RPRESS, *this));
    trimBuffer();
}

void Mouse::onRightReleased(int x, int y) {
    rightDown = false;
    eventBuffer.push(Mouse::Event(Mouse::Event::Type::RRELEASE, *this));
    trimBuffer();
}

void Mouse::onWheelUp(int x, int y) {
    eventBuffer.push(Mouse::Event(Mouse::Event::Type::WHEELUP, *this));
    trimBuffer();
}

void Mouse::onWheelDown(int x, int y) {
    eventBuffer.push(Mouse::Event(Mouse::Event::Type::WHEELDOWN, *this));
    trimBuffer();
}

void Mouse::onWheelSpin(int x, int y, int delta) {
    wheelDelta += delta;
    while(wheelDelta >= WHEEL_DELTA) {
        wheelDelta -= WHEEL_DELTA;
        onWheelUp(x, y);
    }

    while(wheelDelta <= -WHEEL_DELTA) {
        wheelDelta += WHEEL_DELTA;
        onWheelDown(x, y);
    }
}

void Mouse::trimBuffer() {
    while(eventBuffer.size() > bufferSize) {
        eventBuffer.pop();
    }
}