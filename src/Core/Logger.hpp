#pragma once

#include "Window/Windows.hpp"

#include <rang.hpp>

#include <ctime>
#include <string>
#include <locale>
#include <format>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>

namespace Logger {
    enum Severity {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    static const std::unordered_map<Severity, const char*> severityStrings = {
        { Trace   , "Trace"    },
        { Debug   , "Debug"    },
        { Info    , "Info"     },
        { Warning , "Warning"  },
        { Error   , "Error"    },
        { Critical, "Critical" }
    };

    static const std::unordered_map<Severity, const wchar_t*> severityStringsW = {
        { Trace   , L"Trace"    },
        { Debug   , L"Debug"    },
        { Info    , L"Info"     },
        { Warning , L"Warning"  },
        { Error   , L"Error"    },
        { Critical, L"Critical" }
    };

    static const std::unordered_map<Severity, rang::fg> severityColors = {
        { Trace   , rang::fg::cyan    },
        { Debug   , rang::fg::yellow  },
        { Info    , rang::fg::green   },
        { Warning , rang::fg::blue    },
        { Error   , rang::fg::red     },
        { Critical, rang::fg::magenta }
    };

    static const std::unordered_map<Severity, const wchar_t*> severityColorsW = {
        { Trace   , L"\033[36m" },
        { Debug   , L"\033[33m" },
        { Info    , L"\033[32m" },
        { Warning , L"\033[34m" },
        { Error   , L"\033[31m" },
        { Critical, L"\033[35m" }
    };

    void allocConsole();
    void bindConsoleOutput();
    std::string formatTime(Severity severity);
    std::wstring formatTimeW(Severity severity);
    void logCore(Severity severity, const std::string& msg);
    void logCoreW(Severity severity, const std::wstring& msg);

    // Actual log functions
    void trace(const std::string& msg);
    void debug(const std::string& msg);
    void info(const std::string& msg);
    void warning(const std::string& msg);
    void error(const std::string& msg);
    void critical(const std::string& msg);

    // Wide strings
    void traceW(const std::wstring& msg);
    void debugW(const std::wstring& msg);
    void infoW(const std::wstring& msg);
    void warningW(const std::wstring& msg);
    void errorW(const std::wstring& msg);
    void criticalW(const std::wstring& msg);
} // namespace Logger

#define LOG_ERROR(res, msg) \
    if (res != S_OK) { Logger::critical(std::string(msg) + std::format(" ({})\nPress any key to exit...", res)); std::cin.get(); exit(-1); }

#define LOG_ERROR_W(res, msg) \
    if (res != S_OK) { Logger::criticalW(std::wstring(msg) + std::format(L" ({})\nPress any key to exit...", res)); std::cin.get(); exit(-1); }