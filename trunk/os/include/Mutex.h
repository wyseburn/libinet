/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_MUTEX_H
#define IGAME_OS_MUTEX_H

#include "os/include/Lock.h"
#include "os/include/Condition.h"

namespace IGame 
{
    class Mutex
    {
    protected:
#ifndef WIN32
        mutable  pthread_mutex_t m_Id;
#else
        void* m_Id; //CRITICAL_SECTION
#endif
    private:
        //  no value sematics, therefore private and not implemented.
        Mutex (const Mutex&);
        Mutex& operator= (const Mutex&);
    public:
        Mutex();
        virtual ~Mutex();

        void Lock();
        void Unlock();
#ifndef WIN32
        pthread_mutex_t* GetId()
        {
            return &m_Id;
        }
#endif
    };

    class SpinMutex
    {
    public:
        SpinMutex();
        virtual ~SpinMutex();

        void Lock();
        void Unlock();

    private:
#ifdef _WIN32
        void* m_Id; // CRITICAL_SECTION
#else
        mutable pthread_spinlock_t m_Id;
#endif
    };
   
    class ReadWriteMutex
    {
    public:
        ReadWriteMutex();
        virtual ~ReadWriteMutex();
         
        void Unlock();
        void ReadLock();
        void WriteLock();
#if 0
        unsigned int ReaderCount() const;
        unsigned int PendingWriterCount() const;
#endif
    private:
#ifdef WIN32
        Mutex m_Mutex;
        Condition m_ReadCondition;
        Condition m_PendingWriteCondition;
        unsigned int m_ReaderCount;
        bool m_WriterHasLock;
        unsigned int m_PendingWriterCount;
#else
        pthread_rwlock_t m_Id;
#endif
    };

	class LockTraits
    {
    public:
        inline static void Lock(Mutex& mtx)
        {
            mtx.Lock();
        }
    
        inline static void Unlock(Mutex& mtx)
        {
            mtx.Unlock();
        }
    };

    class SpinLockTraits
    {
    public:
        inline static void Lock(SpinMutex& mtx)
        {
            mtx.Lock();
        }

        inline static void Unlock(SpinMutex& mtx)
        {
            mtx.Unlock();
        }
    };

    class ReadLockTraits
    {
    public:
        inline static void Lock(ReadWriteMutex& mtx)
        {
            mtx.ReadLock();
        }
    
        inline static void Unlock(ReadWriteMutex& mtx)
        {
            mtx.Unlock();
        }
    };
   
    class WriteLockTraits
    {
    public:
        inline static void Lock(ReadWriteMutex& mtx)
        {
            mtx.WriteLock();
        }
    
        inline static void Unlock(ReadWriteMutex& mtx)
        {
            mtx.Unlock();
        }
    };

    typedef BaseLock<Mutex, LockTraits> Lock;
    typedef BaseLock<SpinMutex, SpinLockTraits> SpinLock;
    typedef BaseLock<ReadWriteMutex, ReadLockTraits> ReadLock;
    typedef BaseLock<ReadWriteMutex, WriteLockTraits> WriteLock;
   
} // namespace IGame 

#endif // IGAME_OS_MUTEX_H


