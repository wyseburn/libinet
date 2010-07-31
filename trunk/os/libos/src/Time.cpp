/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#if defined(_WIN32)
# include <windows.h>
# include <winbase.h>
# include <sys/timeb.h>
#else
# include <sys/time.h>
# include <unistd.h>
#endif

#include <iostream>
#include <cassert>
#include <time.h>

#include "os/include/Time.h"

using namespace IGame;

#ifdef _WIN32
static int
gettimeofday(struct timeval* tv, struct timezone* tz)
{
    struct _timeb tb;

    if (tv == NULL) return -1;

    _ftime(&tb);
    tv->tv_sec = (long)tb.time;
    tv->tv_usec = ((int)tb.millitm) * 1000;
    return 0;
}
#endif 

_UInt64
Time::GetSystemTime()
{
    assert(sizeof(_UInt64) == 64 / 8);
    _UInt64 time = 0;
//#if defined (_WIN32)
//    FILETIME ft;
//    GetSystemTimeAsFileTime(&ft);
//    ULARGE_INTEGER li;
//    li.LowPart = ft.dwLowDateTime;
//    li.HighPart = ft.dwHighDateTime;
//    time = li.QuadPart/10;
//#else
    struct timeval now;
    gettimeofday(&now, NULL);
    time = now.tv_sec;
    time = time * 1000000;
    time += now.tv_usec;
//#endif
    return time;
}

_UInt64
Time::GetTimeMicroSec()
{
    return GetSystemTime();
}

_UInt64
Time::GetTimeMilliSec()
{
    return GetSystemTime() / 1000;
}

_UInt32
Time::GetTimeSec()
{
    time_t t;
    return time(&t);
    //return GetSystemTime() / 1000000;
}

_UInt32
Time::GetTodayBaseSec()
{
    time_t t = time(NULL);
    struct tm* _tm = localtime(&t);  
    _tm->tm_hour = 0;
    _tm->tm_min = 0;
    _tm->tm_sec = 0;
    return mktime(_tm);
}

void
Time::GetTimestamp(char* buf, _Int32 size)
{
#if defined (_WIN32)
    _Int32 result = 1;
    SYSTEMTIME systemTime;
    struct { time_t tv_sec; int tv_usec; } tv = {0, 0};
    time(&tv.tv_sec);
    GetLocalTime(&systemTime);
    tv.tv_usec = systemTime.wMilliseconds * 1000;
#else
    struct timeval tv;
    _Int32 result = gettimeofday(&tv, NULL);
#endif

    _Int32 outTextLen = 0;
    if (result == -1)
    {
        buf[0] = 0;
        return;
    }
    else
    {
        /**
         * The tv_sec filed represennts the number of seconds passed since
         * the Epoch, which is exactly the argument gettimeofday needs.
         */

        const time_t timeInSeconds = (time_t) tv.tv_sec;
        outTextLen = (int)strftime(buf, size, "%Y%m%d-%H%M%S", localtime(&timeInSeconds));
    }

    char msBuf[5];

   /**
    * Dividing (without remainder) by 1000 reounds the microseconds
    * measure to the nearest millisecond
    **/

    int msLen = sprintf(msBuf, ".%3.3ld", long(tv.tv_usec/1000));
    memcpy(buf + outTextLen, msBuf, msLen);
    buf[outTextLen + msLen] = '\0';
}

void 
Time::GetTimestamp(char* buf, _Int32 size, _UInt64 ms)
{
    const time_t timeInSeconds = ms/1000;
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", localtime(&timeInSeconds));
}

_Int32
Time::GetTimeOfDay(struct timeval* tv, struct timezone* tz)
{
#if defined (_WIN32)
    struct _timeb tb;

    if (tv == NULL) return -1;

    _ftime(&tb);
    tv->tv_sec = (long)tb.time;
    tv->tv_usec = ((long)tb.millitm) * 1000;
    return 0;
#else
    return gettimeofday(tv, tz);
#endif
}

void
Time::Sleep(_UInt64 ms)
{
#if defined (_WIN32)
    ::Sleep((DWORD)ms);
#else
    usleep(1000 * ms);
#endif
}


