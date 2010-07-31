/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#include <cassert>
#include "os/include/Mutex.h"

#if defined (_WIN32)
# include <windows.h>
#endif 

using namespace IGame;

// Mutex Lock

Mutex::Mutex()
{
#ifndef WIN32
    int ret = pthread_mutex_init(&m_Id, NULL);
    assert(ret == 0);
#else
    // Note:  Windows Critical sections are recursive in nature and perhaps
    //        this implementation calls for a non-recursive implementation
    //        (since there also exists a RecursiveMutex class).  The effort
    //        to make this non-recursive just doesn't make sense though. (SLG)
    m_Id = new CRITICAL_SECTION();
    assert(m_Id);
    InitializeCriticalSection((CRITICAL_SECTION *)m_Id);
#endif
}

Mutex::~Mutex()
{
#ifndef WIN32
    int ret = pthread_mutex_destroy(&m_Id);
    assert(ret == 0);
#else
    DeleteCriticalSection((CRITICAL_SECTION *)m_Id);
    delete m_Id;
#endif
}

void 
Mutex::Lock()
{
#ifndef WIN32
    int ret = pthread_mutex_lock(&m_Id);
    assert(ret == 0);
#else
    EnterCriticalSection((CRITICAL_SECTION *)m_Id);
#endif
}

// Spin Lock

void 
Mutex::Unlock()
{
#ifndef WIN32
    int ret = pthread_mutex_unlock(&m_Id);
    assert(ret == 0);
#else
    LeaveCriticalSection((CRITICAL_SECTION *)m_Id);
#endif
}

SpinMutex::SpinMutex()
{
#ifndef WIN32
    int ret = pthread_spin_init(&m_Id, PTHREAD_PROCESS_PRIVATE);
    assert(ret == 0);
#else
    m_Id = new CRITICAL_SECTION();
    assert(m_Id);
    InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION *)m_Id, 4000);
#endif
}

SpinMutex::~SpinMutex()
{
#ifndef WIN32
    int ret = pthread_spin_destroy(&m_Id);
    assert(ret == 0);
#else
    DeleteCriticalSection((CRITICAL_SECTION *)m_Id);
    delete m_Id;
#endif
}

void 
SpinMutex::Lock()
{
#ifndef WIN32
    int ret = pthread_spin_lock(&m_Id);
    assert(ret == 0);
#else
    EnterCriticalSection((CRITICAL_SECTION *)m_Id);
#endif
}
		
void 
SpinMutex::Unlock()
{
#ifndef WIN32
    int ret = pthread_spin_unlock(&m_Id);
    assert(ret == 0);
#else
    LeaveCriticalSection((CRITICAL_SECTION *)m_Id);
#endif
}

// Read and Write Lock.

ReadWriteMutex::ReadWriteMutex()
#ifdef WIN32
    :m_ReaderCount(0), m_WriterHasLock(false), m_PendingWriterCount(0)
#endif
{
#ifndef WIN32
    pthread_rwlock_init(&m_Id, 0);
#endif
}

ReadWriteMutex::~ReadWriteMutex()
{
#ifndef WIN32
    pthread_rwlock_destroy(&m_Id);
#endif
}

void 
ReadWriteMutex::Unlock()
{
#ifdef WIN32
    Lock lock(m_Mutex);

    // Unlocking a write lock.
    if (m_WriterHasLock)
    {
        assert(m_ReaderCount == 0);
        m_WriterHasLock = false;

        // Pending writers have priority. Could potentially starve readers.
        if (m_PendingWriterCount > 0)
        {
            m_PendingWriteCondition.Signal();
        }

        // No writer, no pending writers, so all the readers can go.
        else
        {
            m_ReadCondition.Broadcast();
        }
    }

    // Unlocking a read lock.
    else
    {
        assert(m_ReaderCount > 0);
        m_ReaderCount--;

        if (m_ReaderCount == 0 && m_PendingWriterCount > 0 )
        {
            m_PendingWriteCondition.Signal();
        }
    }
#else
    pthread_rwlock_unlock(&m_Id);
#endif
}

void 
ReadWriteMutex::ReadLock()
{
#ifdef WIN32
    Lock lock(m_Mutex);
   
    while (m_WriterHasLock || m_PendingWriterCount > 0)
    {
        m_ReadCondition.Wait(m_Mutex);
    }
   
    m_ReaderCount++;
#else
    pthread_rwlock_rdlock(&m_Id);
#endif
}

void 
ReadWriteMutex::WriteLock()
{
#ifdef WIN32
    Lock lock(m_Mutex);
  
    m_PendingWriterCount++;
    while (m_WriterHasLock || m_ReaderCount > 0)
    {
        m_PendingWriteCondition.Wait(m_Mutex);
    }
   
    m_PendingWriterCount--;
    m_WriterHasLock = true;
#else
    pthread_rwlock_wrlock(&m_Id);
#endif
}

#if 0
unsigned int 
ReadWriteMutex::ReaderCount() const
{
   return (m_ReaderCount);   
}

unsigned int 
ReadWriteMutex::PendingWriterCount() const
{
   return (m_PendingWriterCount);
}
#endif


