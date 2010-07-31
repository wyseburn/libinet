/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_LOCK_H
#define IGAME_OS_LOCK_H

namespace IGame
{
    template <typename MutexTraits, typename LockTraits>
    class BaseLock
    {
    public:    
        BaseLock(MutexTraits& mtx): m_Mutex(mtx)
        {
            LockTraits::Lock(m_Mutex);
        }

        ~BaseLock()
        {
            LockTraits::Unlock(m_Mutex);
        }

        inline void Lock()
        {
            LockTraits::Lock(m_Mutex);
        }
    
        inline void Unlock()
        {
            LockTraits::Unlock(m_Mutex);
        }
    protected:
        MutexTraits& m_Mutex;
    };
} // namespace IGame

#endif // #ifndef IGAME_OS_LOCK_H


