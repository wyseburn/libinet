/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_CONDITION_H
#define IGAME_OS_CONDITION_H

#ifndef _WIN32
# include <pthread.h>
#endif

namespace IGame
{
    class Mutex;   
    class Condition
    {
    public:
        Condition();
        virtual ~Condition();

        void Wait (Mutex& mtx);
        bool Wait (Mutex& mutex, unsigned int ms);

        void Wait (Mutex* mutex);
        bool Wait (Mutex* mutex, unsigned int ms);
        void Signal();
        void Broadcast();

    private:
        Condition (const Condition&);
        Condition& operator=(const Condition&);

#ifndef _WIN32
        mutable  pthread_cond_t m_Id;
#else
        void EnterWait ();

        void* m_Gate;
        void* m_Queue;
        void* m_Mutex;
        unsigned m_Gone; 
        unsigned long m_Blocked;
        unsigned m_Waiting;
#endif
   };
   
} // namespace Game

#endif // #ifndef IGAME_OS_CONDITION_H



