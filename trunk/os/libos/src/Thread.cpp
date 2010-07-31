/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#include <cassert>
#include <iostream>

#if defined(WIN32)
# include <windows.h>
# include <winbase.h>
# include <process.h> // for _beginthreadex()
typedef unsigned(__stdcall *FG_THREAD_START_ROUTINE)(void*);
#endif

#include "os/include/Atomic.h"
#include "os/include/Thread.h"
#include "os/include/Logger.h"

using namespace IGame;

extern "C"
{
    static void*
#ifdef WIN32
    __stdcall
#endif
    ThreadWrapper(void* threadParm)
    {
#if defined(USE_PROFILER)
        ProfilerRegisterThread();
#endif
        assert(threadParm);
        Thread* t = static_cast <Thread*> (threadParm);

        assert(t);
        t->ThreadFunc();
#if defined(WIN32)
       _endthreadex(0);
#endif
        return 0;
    }
}

Thread::Thread(): m_Id(0), m_Shutdown(-1)
{
}

Thread::~Thread()
{
   Stop();
   Join();
}

void 
Thread::Start()
{
    assert(m_Id == 0);
#if defined (_WIN32)
    m_Thread = (HANDLE)_beginthreadex(NULL, 0, FG_THREAD_START_ROUTINE(ThreadWrapper), this, 0, &m_Id);
    assert(m_Thread != 0);
#else
    // spawn the thread
    if (pthread_create(&m_Id, 0, ThreadWrapper, this) != 0)
    {
        LOG_ERROR("Failed to spawn thread\n");
        assert(0);
    }
#endif
    m_Shutdown = 0;
}

void 
Thread::Stop()
{
    InterlockedIncrement(&m_Shutdown);
}

void 
Thread::Join()
{
    if (m_Id == 0)
    {
        return;
    }

#if defined (_WIN32)
   DWORD exitCode;
   while (true)
   {
       if (GetExitCodeThread(m_Thread, &exitCode) != 0)
       {
            if (exitCode != STILL_ACTIVE)
            {
                break;
            }
            else
            {
                WaitForSingleObject(m_Thread, INFINITE);
            }
        }
        else
        {
            // log something here
            break;
        }
    }

    CloseHandle(m_Thread);
#else
    void* stat;
    if (m_Id != pthread_self())
    {
        int r = pthread_join(m_Id, &stat);
        if (r != 0)
        {
            LOG_WARN("Failed to call pthread_join\n");
            assert(0);
        }
    }
#endif
    m_Id = 0;
}

bool 
Thread::IsShutdown()
{
   return m_Shutdown == 1;
}

