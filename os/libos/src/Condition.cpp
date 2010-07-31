/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#include <cassert>
#include <climits>

#if defined(WIN32)
# include <windows.h>
#else
# include <pthread.h>
# include <errno.h>
# include <sys/time.h>
#endif

#include "os/include/Mutex.h"
#include "os/include/Time.h"
#include "os/include/Condition.h"
#include "os/include/Logger.h"

using namespace IGame;

Condition::Condition()
{
#ifndef WIN32
    int ret = pthread_cond_init(&m_Id,0);
    assert(ret == 0);
#else
    m_Blocked = 0;
    m_Gone = 0;
    m_Waiting = 0;
    m_Gate = reinterpret_cast<void*>(CreateSemaphore(0, 1, 1, 0));
    m_Queue = reinterpret_cast<void*>(CreateSemaphore(0, 0, LONG_MAX, 0));
    m_Mutex = reinterpret_cast<void*>(CreateMutex(0, 0, 0));

    if (!m_Gate || !m_Queue || !m_Mutex)
    {
       int res = 0;
       if (m_Gate)
       {
           res = CloseHandle(reinterpret_cast<HANDLE>(m_Gate));
           assert(res);
       }
       if (m_Queue)
       {
           res = CloseHandle(reinterpret_cast<HANDLE>(m_Queue));
           assert(res);
       }
       if (m_Mutex)
       {
           res = CloseHandle(reinterpret_cast<HANDLE>(m_Mutex));
           assert(res);
       }

       assert(0);
   }
#endif
}

Condition::~Condition ()
{
#ifndef WIN32
    int ret = pthread_cond_destroy(&m_Id);
    assert(ret != EBUSY);
#else
    int res = 0;
    res = CloseHandle(reinterpret_cast<HANDLE>(m_Gate));
    assert(res);
    res = CloseHandle(reinterpret_cast<HANDLE>(m_Queue));
    assert(res);
    res = CloseHandle(reinterpret_cast<HANDLE>(m_Mutex));
    assert(res);
#endif
}

#if defined(WIN32)
void
Condition::EnterWait ()
{
    int res = 0;
    res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_Gate), INFINITE);
    assert(res == WAIT_OBJECT_0);
    ++m_Blocked;
    res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_Gate), 1, 0);
    assert(res);
}
#endif

void
Condition::Wait(Mutex& mutex)
{
#ifndef WIN32
    int ret = pthread_cond_wait(&m_Id, mutex.GetId());
    assert(ret == 0);
#else
    EnterWait();

    // Release the mutex
    mutex.Unlock();

    int res = 0;
    res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_Queue), INFINITE);
    assert(res == WAIT_OBJECT_0);

    unsigned wasWaiting = 0;
    unsigned wasGone = 0;

    res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_Mutex), INFINITE);
    assert(res == WAIT_OBJECT_0);
    wasWaiting = m_Waiting;
    wasGone = m_Gone;
    if (wasWaiting != 0)
    {
        if (--m_Waiting == 0)
        {
            if (m_Blocked != 0)
            {
                res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_Gate), 1, 0); // open m_Gate
                assert(res);
                wasWaiting = 0;
            }
            else if (m_Gone != 0)
            {
                m_Gone = 0;
            }
        }
    }
    else if (++m_Gone == (ULONG_MAX / 2))
    {
        // timeout occured, normalize the m_Gone count
        // this may occur if many calls to wait with a timeout are made and
        // no call to notify_* is made
        res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_Gate), INFINITE);
        assert(res == WAIT_OBJECT_0);
        m_Blocked -= m_Gone;
        res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_Gate), 1, 0);
        assert(res);
        m_Gone = 0;
    }
    res = ReleaseMutex(reinterpret_cast<HANDLE>(m_Mutex));
    assert(res);

    if (wasWaiting == 1)
    {
        for (/**/ ; wasGone; --wasGone)
        {
            // better now than spurious later
            res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_Queue), INFINITE);
            assert(res == WAIT_OBJECT_0);
        }
        res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_Gate), 1, 0);
        assert(res);
    }

    // Reacquire the mutex
    mutex.Lock();
#endif
}

void
Condition::Wait(Mutex* mutex)
{
    Wait(*mutex);
}

bool
Condition::Wait(Mutex& mutex, unsigned int ms)
{
    if (ms == 0)
    {
        Wait(mutex);
        return true;
    }
#ifndef WIN32
    uint64_t expires64 = Time::GetTimeMilliSec() + ms;
    timespec expiresTS;
    expiresTS.tv_sec = expires64 / 1000;
    expiresTS.tv_nsec = (expires64 % 1000) * 1000000L;

    assert( expiresTS.tv_nsec < 1000000000L );

    int ret = pthread_cond_timedwait(&m_Id, mutex.GetId(), &expiresTS);

    if (ret == EINTR || ret == ETIMEDOUT)
    {
        return false;
    }
    else
    {
        assert( ret == 0 );
        return true;
    }
#else
    EnterWait();

    // Release the mutex
    mutex.Unlock();

    //  do timed wait
    bool ret = false;
    unsigned int res = 0;

    res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_Queue),ms);
    assert(res != WAIT_FAILED && res != WAIT_ABANDONED);
    ret = (res == WAIT_OBJECT_0);

    unsigned wasWaiting = 0;
    unsigned wasGone = 0;

    res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_Mutex), INFINITE);
    assert(res == WAIT_OBJECT_0);
    wasWaiting = m_Waiting;
    wasGone = m_Gone;
    if (wasWaiting != 0)
    {
        if (!ret) // timeout
        {
            if (m_Blocked != 0)
                --m_Blocked;
            else
                ++m_Gone; // count spurious wakeups
        }
        if (--m_Waiting == 0)
        {
            if (m_Blocked != 0)
            {
                res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_Gate), 1, 0); // open m_Gate
                assert(res);
                wasWaiting = 0;
            }
            else if (m_Gone != 0)
                m_Gone = 0;
        }
    }
    else if (++m_Gone == (ULONG_MAX / 2))
    {
        // timeout occured, normalize the m_Gone count
        // this may occur if many calls to wait with a timeout are made and
        // no call to notify_* is made
        res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_Gate), INFINITE);
        assert(res == WAIT_OBJECT_0);
        m_Blocked -= m_Gone;
        res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_Gate), 1, 0);
        assert(res);
        m_Gone = 0;
    }
    res = ReleaseMutex(reinterpret_cast<HANDLE>(m_Mutex));
    assert(res);

    if (wasWaiting == 1)
    {
        for (/**/ ; wasGone; --wasGone)
        {
            // better now than spurious later
            res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_Queue), INFINITE);
            assert(res ==  WAIT_OBJECT_0);
        }
        res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_Gate), 1, 0);
        assert(res);
    }

    // Reacquire the mutex
    mutex.Lock();

    return ret;
#endif
}

bool
Condition::Wait (Mutex* mutex, unsigned int ms)
{
    return Wait(*mutex, ms);
}

void
Condition::Signal ()
{
#ifndef WIN32
    int ret = pthread_cond_signal(&m_Id);
    assert(ret == 0);
#else
    unsigned signals = 0;

    int res = 0;
    res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_Mutex), INFINITE);
    assert(res == WAIT_OBJECT_0);

    if (m_Waiting != 0) // the m_Gate is already closed
    {
        if (m_Blocked == 0)
        {
            res = ReleaseMutex(reinterpret_cast<HANDLE>(m_Mutex));
            assert(res);
            return;
        }

        ++m_Waiting;
        --m_Blocked;
        signals = 1;
    }
    else
    {
        res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_Gate), INFINITE);
        assert(res == WAIT_OBJECT_0);
        if (m_Blocked > m_Gone)
        {
            if (m_Gone != 0)
            {
                m_Blocked -= m_Gone;
                m_Gone = 0;
            }
            signals = m_Waiting = 1;
            --m_Blocked;
        }
        else
        {
            res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_Gate), 1, 0);
            assert(res);
        }
    }

    res = ReleaseMutex(reinterpret_cast<HANDLE>(m_Mutex));
    assert(res);

    if (signals)
    {
        res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_Queue), signals, 0);
        assert(res);
    }
#endif
}

void
Condition::Broadcast()
{
#ifndef WIN32
    pthread_cond_broadcast(&m_Id);
#else
    unsigned signals = 0;

    int res = 0;
    res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_Mutex), INFINITE);
    assert(res == WAIT_OBJECT_0);

    if (m_Waiting != 0) // the m_Gate is already closed
    {
        if (m_Blocked == 0)
        {
            res = ReleaseMutex(reinterpret_cast<HANDLE>(m_Mutex));
            assert(res);
            return;
        }

        m_Waiting += (signals = m_Blocked);
        m_Blocked = 0;
    }
    else
    {
        res = WaitForSingleObject(reinterpret_cast<HANDLE>(m_Gate), INFINITE);
        assert(res == WAIT_OBJECT_0);
        if (m_Blocked > m_Gone)
        {
            if (m_Gone != 0)
            {
                m_Blocked -= m_Gone;
                m_Gone = 0;
            }
            signals = m_Waiting = m_Blocked;
            m_Blocked = 0;
        }
        else
        {
            res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_Gate), 1, 0);
            assert(res);
        }
    }

    res = ReleaseMutex(reinterpret_cast<HANDLE>(m_Mutex));
    assert(res);

    if (signals)
    {
        res = ReleaseSemaphore(reinterpret_cast<HANDLE>(m_Queue), signals, 0);
        assert(res);
    }
#endif
}

