/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "os/include/Logger.h"
#include "os/include/Compat.h"
#include "os/include/Mutex.h"

#ifdef USE_LOG4CPP
#include <locale.h>
#include "log4cxx/stream.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/PropertyConfigurator.h"
#include "log4cxx/helpers/exception.h"
#endif	// USE_LOG4CPP

static FILE* s_LogOutputFile = 0;
static IGame::SpinMutex s_SpinMutex;
static _UInt32 s_PaginationSize = 10 * 1024 * 1024; // bytes
static bool s_OutputFileAndFunc = false;
static bool s_Append = false;
static char s_OutputFileDir[256] = { 0 };
static _UInt32 s_OutputFileIndex = 0;

int
IGame::LogInit(const char* configFile, bool outputFileAndFunc, bool append)
{
#ifdef USE_LOG4CPP
    if (configFile == NULL)
    {
        return -1;
    }

    setlocale(LC_ALL, "");

    try
    {
        log4cxx::PropertyConfigurator::configure(log4cxx::File(configFile));
    }
    catch(std::exception&)
    {
        return -1;
    }
#else
    // Nothing to do
#endif  // USE_LOG4CPP
    s_OutputFileAndFunc = outputFileAndFunc;
    s_Append = append;
    LogSetPagination();
    return 0;
}

void 
IGame::LogSetPagination(int size)
{
    assert(size < 1024 * 4); // Max 4G
    s_PaginationSize = size * 1024 * 1024;
}

#ifdef HAVE_SYSLOG_H

#define SYSLOG_NAMES
#include <syslog.h>
using namespace IGame;
/* Log anything (except mail) of level info or higher.  messages */
#if defined (__FreeBSD__)
static int SyslogLevel = LOG_ERR;
#elif defined (__linux__)
static int SyslogLevel = LOG_USER | LOG_INFO;
#endif
static int LogToSyslog = 0;
#endif /* ifdef HAVE_SYSLOG_H */

void
IGame::LogSetToSyslog()
{
#ifdef HAVE_SYSLOG_H
    LogToSyslog = 1;
#endif
}

void
IGame::LogSetSyslogLevel(const char* level)
{
#ifdef HAVE_SYSLOG_H
    int i, found = 0;

    for (i = 0; facilitynames[i].c_name != NULL; i++) 
    {
        if (!strcmp(level, facilitynames[i].c_name)) 
        {
            SyslogLevel = facilitynames[i].c_val | LOG_DEBUG;
            found = 1;
        }
    }

    if (found == 0) 
    {
        LOG_WARN("Failed to set syslog level %s", level);
    }
#endif
}

void
IGame::LogSetOutputFile(const char* fileDir)
{
    assert(strlen(fileDir) < 256);
    strcpy(s_OutputFileDir, fileDir);

    char tmpFileDir[1024] = { 0 };
    sprintf(tmpFileDir, "%s(%d).log", s_OutputFileDir, s_OutputFileIndex++);

    if (s_LogOutputFile)
    {
        fclose(s_LogOutputFile);
    }

    if (s_Append)
    {
        s_LogOutputFile = fopen(tmpFileDir, "a+");
    }
    else
    {
        s_LogOutputFile = fopen(tmpFileDir, "w+");
    }

    if (s_LogOutputFile == 0) 
    {
        LOG_WARN("Failed to open log output file %s", tmpFileDir);
    }
}

int g_LogSilenceLevel = 0;

void
IGame::LogSetSilenceLevel(int level)
{
    g_LogSilenceLevel = level;
}

void 
IGame::LogFunc(const char* subsys, const char* file, int line, const char* func, const char* format, ...)
{
    va_list ap;
    va_start(ap, format);

#ifdef USE_LOG4CPP
    log4cxx::logstream logstream(log4cxx::Logger::getRootLogger(), log4cxx::Level::getDebug());
    char string[LIBINET_LOG_BUFSIZE];
    vsprintf(string, format, ap);
    logstream << file << " " << line << " " << string << LOG4CXX_ENDMSG;
#else

#ifdef HAVE_SYSLOG_H 
    if (!LogToSyslog) 
    {
#endif
        /* not running as daemon - log to STDERR */
        
        if (s_LogOutputFile == 0)
            s_LogOutputFile = stderr;
        time_t t;
        time(&t);
        struct tm *tim = localtime(&t);

        {
            SpinLock lock(s_SpinMutex);
            if (s_OutputFileAndFunc)
            {
                fprintf(s_LogOutputFile, "%4i:%2.2i:%2.2i %2.2i:%2.2i:%2.2i %s %s:%i %s ", 
                    tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour,
                    tim->tm_min, tim->tm_sec, subsys, file, line, func);
            }
            else
            {
                fprintf(s_LogOutputFile, "%4i:%2.2i:%2.2i %2.2i:%2.2i:%2.2i %s %i ", 
                    tim->tm_year + 1900, tim->tm_mon + 1, tim->tm_mday, tim->tm_hour,
                    tim->tm_min, tim->tm_sec, subsys, line);
            }
            vfprintf(s_LogOutputFile, format, ap);
            fprintf(s_LogOutputFile, "\n"); 
            fflush(s_LogOutputFile);

            if ((s_LogOutputFile != stderr) && (ftell(s_LogOutputFile) >= s_PaginationSize))  // pagination
            {
                char tmpFileDir[1024] = { 0 };
                sprintf(tmpFileDir, "%s(%d).log", s_OutputFileDir, s_OutputFileIndex++);
                fclose(s_LogOutputFile);
                s_LogOutputFile = fopen(tmpFileDir, "w+");
                if (s_LogOutputFile == 0) 
                {
                    LOG_WARN("Failed to open log output file %s", tmpFileDir);
                }
            }
        }
#ifdef HAVE_SYSLOG_H 
    } 
    else 
    {
        /* Running as daemon - log via SYSLOG facility */
        char string[LIBINET_LOG_BUFSIZE];
        vsnprintf(string, sizeof(string), format, ap);
        syslog(SyslogLevel, "%s %s:%i %s %s", subsys, file, line, func, string);
    }
#endif // HAVE_SYSLOG_H
#endif // USE_LOG4CPP
    va_end(ap);
}


/**
 * Log Usage: 
 * int main()
 * {
 *       LOG_INIT("configFile", false, false);
 *       Log to a specfiy file. Default output to stderr.
 *       LOG_OUTPUT_FILE("./aaa");
 *
 *       Set silence level, n = 0/1/2/3/4
 *       LOG_SILENCE_LEVEL(n)
 *
 *#ifndef _HAVE_SYSLOG_H
 *       If you want to log to syslog, call below function.
 *       LOG_TO_SYSLOG();
 *
 *       The default syslog_level is "LOG_USER | LOG_INFO".
 *       If you want to change syslog level, you can call below function.
 *       LOG_SYSLOG_LEVEL("local5");
 *#endif
 *       LOG_DEBUG("debug log test...");
 *       LOG_INFOS("infos log test...");
 *       LOG_WARNS("warns log test...");
 *       LOG_ERROR("error log test...");
 *       LOG_PRINT("print log test...");
 *       return 0;
 * }
 */
