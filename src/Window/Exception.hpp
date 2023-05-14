#pragma once

#include "Core/Logger.hpp"

#include <cstdio>
#include <string>
#include <exception>

class CinqException : public std::exception {
private:
    int line;
    std::string file;
protected:
    mutable std::string whatBuffer;
public:
    CinqException(int line, const char* filepath);
    virtual const char* what() const override;
    virtual const char* getType() const;
    int getLine() const { return line; }
    const std::string& getFile() const { return file; }
    std::string getOriginString() const;
};