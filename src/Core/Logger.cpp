#include "Logger.hpp"

namespace Logger {
    void allocConsole() {
        AllocConsole();
    }

    void bindConsoleOutput() {
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stderr);
        freopen("CONOUT$", "w", stdout);
        
        HANDLE hStdout = CreateFile("CONOUT$",  GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                    NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        HANDLE hStdin = CreateFile("CONIN$",  GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                    NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        
        SetStdHandle(STD_OUTPUT_HANDLE, hStdout);
        SetStdHandle(STD_ERROR_HANDLE, hStdout);
        SetStdHandle(STD_INPUT_HANDLE, hStdin);

        std::wclog.clear();
        std::clog.clear();
        std::wcout.clear();
        std::cout.clear();
        std::wcerr.clear();
        std::cerr.clear();
        std::wcin.clear();
        std::cin.clear();
    }

    std::string formatTime(Severity severity) {
        time_t t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << "[" << severityStrings.at(severity) << " " << std::put_time(&tm, "%D %T") << "] ";
        return oss.str();
    }

    std::wstring formatTimeW(Severity severity) {
        time_t t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::wostringstream oss;
        oss << L"[" << severityStringsW.at(severity) << L" " << std::put_time<wchar_t>(&tm, L"%D %T") << L"] ";
        return oss.str();
    }

    void logCore(Severity severity, const std::string& msg) {
        std::clog << severityColors.at(severity) << formatTime(severity) << rang::fg::reset << msg << '\n';
    }
    
    void logCoreW(Severity severity, const std::wstring& msg) {
        std::wclog << severityColorsW.at(severity) << formatTimeW(severity) << L"\033[0m" << msg << '\n';
    }

    void trace(const std::string& msg)    { logCore(Trace, msg);    }
    void debug(const std::string& msg)    { logCore(Debug, msg);    }
    void info(const std::string& msg)     { logCore(Info, msg);     }
    void warning(const std::string& msg)  { logCore(Warning, msg);  }
    void error(const std::string& msg)    { logCore(Error, msg);    }
    void critical(const std::string& msg) { logCore(Critical, msg); }

    void traceW(const std::wstring& msg)    { logCoreW(Trace, msg);    }
    void debugW(const std::wstring& msg)    { logCoreW(Debug, msg);    }
    void infoW(const std::wstring& msg)     { logCoreW(Info, msg);     }
    void warningW(const std::wstring& msg)  { logCoreW(Warning, msg);  }
    void errorW(const std::wstring& msg)    { logCoreW(Error, msg);    }
    void criticalW(const std::wstring& msg) { logCoreW(Critical, msg); }
} // namespace Logger 