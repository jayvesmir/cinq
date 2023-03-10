#include "Exception.hpp"

CinqException::CinqException(int line, const char* filepath) 
    : line(line), file(filepath) {}

const char* CinqException::what() const {
    char format[512];
    sprintf(format, "%s thrown in %s", getType(), getOriginString().c_str());
    whatBuffer = std::string(format);
    return whatBuffer.c_str();
}

const char* CinqException::getType() const {
    return "Base Cinq Exception";
}

std::string CinqException::getOriginString() const {
    char format[512];
    sprintf(format, "%s on line %d", getFile().c_str(), getLine());
    return std::string(format);
}