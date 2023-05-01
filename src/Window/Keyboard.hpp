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
        Event(Type type, uint8_t code) 
            : type(type), code(code) {}

        bool isPress() const  { return type == Type::PRESS; }
        bool isRelease() const  { return type == Type::RELEASE; }
        uint8_t getCode() const  { return code; }

    private:
        Type type;
        uint8_t code;
    };

private:
    bool autorepeatEnabled = false;
    static constexpr uint32_t nKeys = 256;
    static constexpr uint32_t bufferSize = 16;

    std::queue<char> charBuffer;
    std::queue<Event> keyBuffer;
    std::bitset<nKeys> keystates;

    void onKeyPressed(unsigned char keycode);
    void onKeyReleased(unsigned char keycode);
    void onChar(char character);
    void clearState()  { keystates.reset(); }
    template<typename T>
    static void trimBuffer(std::queue<T>& buffer);

public:
    Keyboard() = default;
    Keyboard(const Keyboard&) = delete;
    Keyboard& operator=(const Keyboard&) = delete;

    bool keyIsPressed(unsigned char keycode) const  { return keystates[keycode]; }
    Event readKey();
    bool keyIsEmpty() const  { return keyBuffer.empty(); }
    void flushKey()  { keyBuffer = std::queue<Event>(); }

    char readChar();
    bool charIsEmpty() const  { return charBuffer.empty(); }
    void flushChar()  { charBuffer = std::queue<char>(); }
    void flush();

    void enableAutorepeat()  { autorepeatEnabled = true; }
    void disableAutorepeat()  { autorepeatEnabled = false; }
    bool autorepeatIsEnabled() const  { return autorepeatEnabled; }
};