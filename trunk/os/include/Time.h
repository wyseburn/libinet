/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_TIME_H
#define IGAME_OS_TIME_H

#include "os/include/Compat.h"

namespace IGame
{
    class Time
    {
    public:
        static _UInt64 GetSystemTime();
        static _UInt64 GetTimeMicroSec();
        static _UInt64 GetTimeMilliSec();
        static _UInt32 GetTimeSec();
        static _UInt32 GetTodayBaseSec();
        static void GetTimestamp(char* buf, _Int32 size);
        static void GetTimestamp(char* buf, _Int32 size, _UInt64 millisec);
        static _Int32 GetTimeOfDay(struct timeval* tv, struct timezone* tz);
        static void Sleep(_UInt64 millisec);
    };
} // namespace IGame 

#endif // #ifndef IGAME_OS_TIME_H


