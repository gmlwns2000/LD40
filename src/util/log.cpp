#include <fstream>
#include <mutex>
#include <cstdarg>
#include "log.h"

using namespace ldjam;

static struct LoggerInstance
{
    ~LoggerInstance()
    {
        if (file) fclose(file);
    }

    bool quiet = false;
    LogLevel level = LogLevel::Info;
    
    FILE* file = nullptr;
    std::mutex mtx;
} L;

constexpr const char* logLevelStrings[] = { "[Debug]", "[Info]", "[Warn]", "[Error]", "[Fatal]" };

void ldjam::AbortProcess(const char* msg)
{
    LD_LOG_FATAL("%s", msg);
    LD_LOG_FATAL("%s", "Aborting process...");
    exit(1);
}

// -- Credit: https://github.com/rxi/log.c/blob/master/src/log.c
void ldjam::Log(LogLevel level, SourceInfo info, const char * fmt, ...)
{
    if (level < L.level) return;
    
    L.mtx.lock();
    {
        time_t tstamp = time(NULL);
        struct tm *lt = localtime(&tstamp);

        if (!L.quiet)
        {
            va_list args;
            char buff[16];
            buff[strftime(buff, sizeof(buff), "%H:%M:%S", lt)] = '\0';
            fprintf(stderr, "- %s %-7s (%-20s| line:%-5d): ", buff, logLevelStrings[(int)level], info.filename, info.line);

            va_start(args, fmt);
            vfprintf(stderr, fmt, args);
            va_end(args);
            fprintf(stderr, "\n");
        }

        if (L.file)
        {
            va_list args;
            char buf[32];
            buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
            
            fprintf(L.file, "- %s %-7s (%s, line:%d): ", buf, logLevelStrings[(int)level], info.filename, info.line);
            va_start(args, fmt);
            vfprintf(L.file, fmt, args);
            va_end(args);
            fprintf(L.file, "\n");
        }

    } L.mtx.unlock();
}

void ldjam::log::SetLevel(LogLevel level) { L.level = level; }
void ldjam::log::SetQuiet(bool quiet) { L.quiet = quiet; }

void ldjam::log::SetFile(const char* filename)
{
    if (L.file) fclose(L.file);
    L.file = fopen(filename, "w");
}
