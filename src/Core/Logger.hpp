#pragma once

#include "Window/Windows.hpp"

#include <rang.hpp>

#include <ctime>
#include <string>
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

    static const std::unordered_map<Severity, rang::fg> severityColors = {
        { Trace   , rang::fg::cyan    },
        { Debug   , rang::fg::green   },
        { Info    , rang::fg::yellow  },
        { Warning , rang::fg::blue    },
        { Error   , rang::fg::red     },
        { Critical, rang::fg::magenta }
    };

    void allocConsole();
    void bindConsoleOutput();
    std::string formatTime(Severity severity);
    void logCore(Severity severity, const std::string& msg);

    // Actual log functions
    void trace(const std::string& msg);
    void debug(const std::string& msg);
    void info(const std::string& msg);
    void warning(const std::string& msg);
    void error(const std::string& msg);
    void critical(const std::string& msg);
} // namespace Logger