#pragma once

#include <queue>
#include <bitset>

class Keyboard {
    friend class Window;
public:
    class Event {
    public:
        enum Type {
            PRESS = 0,
            RELEASE,
        };

        Event() = default;
        Event(Type type, uint8_t code) noexcept
            : type(type), code(code) {}

        bool isPress() const noexcept { return type == Type::PRESS; }
        bool isRelease() const noexcept { return type == Type::RELEASE; }
        uint8_t getCode() const noexcept { return code; }

    private:
        Type type;
        uint8_t code;
    };

private:
    bool autorepeatEnabled = false;
    static constexpr uint32_t nKeys = 256;
    static constexpr uint32_t bufferSize = 16;

    std::queue<Event> keyBuffer;
    std::queue<char> charBuffer;
    std::bitset<nKeys> keystates;

    void onKeyPressed(unsigned char keycode) noexcept;
    void onKeyReleased(unsigned char keycode) noexcept;
    void onChar(char character) noexcept;
    void clearState() noexcept { keystates.reset(); }
    template<typename T>
    static void trimBuffer(std::queue<T>& buffer) noexcept;

public:
    Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;

    bool keyIsPressed(unsigned char keycode) const noexcept { return keystates[keycode]; }
    Event readKey() noexcept;
    bool keyIsEmpty() const noexcept { return keyBuffer.empty(); }
    void flushKey() noexcept { keyBuffer = std::queue<Event>(); }

    char readChar() noexcept;
    bool charIsEmpty() const noexcept { return charBuffer.empty(); }
    void flushChar() noexcept { charBuffer = std::queue<char>(); }
    void flush() noexcept;

    void enableAutorepeat() noexcept { autorepeatEnabled = true; }
    void disableAutorepeat() noexcept { autorepeatEnabled = false; }
    bool autorepeatIsEnabled() const noexcept { return autorepeatEnabled; }
};