/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_THREAD_H
#define IGAME_OS_THREAD_H

#ifndef _WIN32
# include <pthread.h>
#endif

namespace IGame
{
    class Thread
    {
#if defined (_WIN32)
        typedef unsigned int Id;
#else
        typedef pthread_t Id;
#endif

    public:
        Thread();
        virtual ~Thread();

        void Start();
        virtual void Stop();
        void Join();
        bool IsShutdown();

        virtual void ThreadFunc() = 0;
    protected:
        Id m_Id;
#if !defined (_WIN32)
        volatile unsigned long m_Shutdown;   
#else
        volatile long m_Shutdown;
#endif

#if defined (_WIN32)
        void* m_Thread;
#endif
    };   
} // namespace IGame    

#endif // #ifndef IGAME_OS_THREAD_H


