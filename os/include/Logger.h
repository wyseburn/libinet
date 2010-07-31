/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_LOGGER_H
#define IGAME_OS_LOGGER_H

/**
 * What shall I log to syslog?
 * 0 - DEBUGs, INFOs, WARNINGs and ERRORs (this is the default)
 * 1 - INFOs, WARNINGs and ERRORs and PRINTs
 * 2 - WARNINGs and ERRORs and PRINTs
 * 3 - only ERRORs and PRINTs
 * 4 - only PRINTs 
 * 5 - absolutely nothing (be careful - you will have no way to
 */

#define LIBINET_LOG_BUFSIZE 2048

extern int g_LogSilenceLevel;
    
namespace IGame
{
    int LogInit(const char* configfile, bool outputFileAndFunc = false, bool append = false);
    void LogSetPagination(int size = 10/*mega*/);
    void LogSetSilenceLevel(int level);
    void LogSetToSyslog();
    void LogSetSyslogLevel(const char* level);
    void LogSetOutputFile(const char* fileDir);
    void LogFunc(const char* subsys, const char* file, int line, const char* func, const char* format, ...);
} // namespace IGame

#define LOG_INIT(configFile, outputFunc, append) IGame::LogInit(configFile, outputFunc, append)
#define LOG_PAGINATION(size) IGame::LogSetPagination(size)
#define LOG_SILENCE_LEVEL(level) IGame::LogSetSilenceLevel(level)
#define LOG_TO_SYSLOG() IGame::LogSetToSyslog()
#define LOG_SYSLOG_LEVEL(level) IGame::LogSetSyslogLevel(level)
#define LOG_OUTPUT_FILE(fileDir) IGame::LogSetOutputFile(fileDir)

/**
 * Variadic Macros notice:
 * Only support for variadic macros was introduced in Visual C++ 2005.
 * Below usage only support by C99, don't compile as a c++ file.
 * There is a expand usage in GCC, it may be complie success both c and c++.
 * eg: ERROR(F...) LogFunc(__FILE__, __LINE__, F)
 */

#define LOG_DEBUG(...)                                                                      \
do {                                                                                        \
    if (g_LogSilenceLevel < 1)                                                              \
        IGame::LogFunc("DEBUG", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);             \
} while (0)

#define LOG_INFO(...)                                                                       \
do {                                                                                        \
    if (g_LogSilenceLevel < 2)                                                              \
        IGame::LogFunc("INFOS", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);             \
} while (0)

#define LOG_WARN(...)                                                                       \
do {                                                                                        \
    if (g_LogSilenceLevel < 3)                                                              \
        IGame::LogFunc("WARNS", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);             \
} while (0)

#define LOG_ERROR(...)                                                                      \
do {                                                                                        \
    if (g_LogSilenceLevel < 4)                                                              \
        IGame::LogFunc("ERROR", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);             \
} while (0)

#define LOG_PRINT(...)                                                                      \
do {                                                                                        \
    if (g_LogSilenceLevel < 5)                                                              \
        IGame::LogFunc("PRINT", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);             \
} while (0)


#endif // #ifndef IGAME_OS_LOGGER_H
