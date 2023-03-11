#include "Keyboard.hpp"

Keyboard::Event Keyboard::readKey() {
	if (keyBuffer.size() > 0) {
		Keyboard::Event e = keyBuffer.front();
		keyBuffer.pop();
		return e;
	}
    return Keyboard::Event();
}

char Keyboard::readChar() {
	if (charBuffer.size() > 0u) {
		unsigned char charcode = charBuffer.front();
		charBuffer.pop();
		return charcode;
	}
    return 0;
}

void Keyboard::flush() {
	flushKey();
	flushChar();
} 

void Keyboard::onKeyPressed(unsigned char keycode) {
	keystates[keycode] = true;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::PRESS, keycode));
	trimBuffer(keyBuffer);
}

void Keyboard::onKeyReleased(unsigned char keycode) {
	keystates[keycode] = false;
	keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::RELEASE, keycode));
	trimBuffer(keyBuffer);
}

void Keyboard::onChar(char character) {
	charBuffer.push(character);
	trimBuffer(charBuffer);
}

template<typename T>
void Keyboard::trimBuffer(std::queue<T>& buffer) {
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}