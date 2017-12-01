#pragma once
#include <string.h>

#ifdef _WIN32
#define LD_AUX_UNDECORRATED_FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define LD_AUX_UNDECORRATED_FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#define LD_LOG_DEBUG(...) ::ldjam::Log(::ldjam::LogLevel::Debug,    ::ldjam::SourceInfo {LD_AUX_UNDECORRATED_FILENAME, __LINE__ }, __VA_ARGS__);
#define LD_LOG_INFO(...)  ::ldjam::Log(::ldjam::LogLevel::Info,     ::ldjam::SourceInfo {LD_AUX_UNDECORRATED_FILENAME, __LINE__ }, __VA_ARGS__);
#define LD_LOG_WARN(...)  ::ldjam::Log(::ldjam::LogLevel::Warn,     ::ldjam::SourceInfo {LD_AUX_UNDECORRATED_FILENAME, __LINE__ }, __VA_ARGS__);
#define LD_LOG_ERROR(...) ::ldjam::Log(::ldjam::LogLevel::Error,    ::ldjam::SourceInfo {LD_AUX_UNDECORRATED_FILENAME, __LINE__ }, __VA_ARGS__);
#define LD_LOG_FATAL(...) ::ldjam::Log(::ldjam::LogLevel::Fatal,    ::ldjam::SourceInfo {LD_AUX_UNDECORRATED_FILENAME, __LINE__ }, __VA_ARGS__);

namespace ldjam
{
    struct SourceInfo
    {
        const char* filename;
        int line;
    };

    enum class LogLevel
    {
        Debug, Info, Warn, Error, Fatal
    };

    void AbortProcess(const char *abortMsg);
    void Log(LogLevel level, SourceInfo info, const char* fmt, ...);
    
    namespace log
    {
        void SetLevel(LogLevel level);
        void SetQuiet(bool quiet);
        
        void SetFile(const char* filename);
    }
}