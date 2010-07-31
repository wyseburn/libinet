/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_TIMEOUT_SERVICE_H
#define IGAME_OS_TIMEOUT_SERVICE_H

#include "os/include/Compat.h"
#include "os/include/Delegate.h"
#include "os/include/DataStruct.h"
#include "os/include/Logger.h"

#ifdef _WIN32
#include <windows.h>
#endif

#if defined (__linux__) || defined (__FreeBSD__)
#include <sys/time.h>
#endif

namespace IGame
{
    typedef Delegate<bool (void* /*args */)> TimeoutEvent; /* return true indicate permanence */
    template <_UInt64 Capacity/*millisecond*/, _UInt64 Precision/*millisecond*/>
    class TimeoutService
    {
    public:
        TimeoutService(_UInt32 cachesize = 100) 
            : m_Scale(0), m_MaxScale((_UInt32)(Capacity / (Precision + 1))) // begin with 0  
            , m_CacheSize(cachesize), m_CacheLen(0)
        {
            for (_UInt32 i = 0; i <= m_MaxScale; ++i)
                IGAME_LIST_INIT(&m_HandlerList[i]);
            IGAME_LIST_INIT(&m_Cache);
  
            {
#if defined (_WIN32)
                m_InitTime = GetTickCount();
#elif defined (__linux__) || defined (__FreeBSD__)
                struct timeval tv;
                gettimeofday(&tv, NULL);
                m_InitTime = ((_UInt64)tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
#endif
            }
        }

        ~TimeoutService() 
        {
            Clean();
        }

        void Clean()
        {
            struct Handler* handler;
            for (_UInt32 i = 0; i < m_MaxScale; ++i)
            {
                while (handler = IGAME_LIST_FIRST(&m_HandlerList[i]))
                {
                    IGAME_LIST_REMOVE_HEAD(&m_HandlerList[i], m_Entries);
                    SAFE_DELETE(handler);
                }
            }
                
            while (handler = IGAME_LIST_FIRST(&m_Cache))
            {
                IGAME_LIST_REMOVE_HEAD(&m_Cache, m_Entries);
                SAFE_DELETE(handler);
            }

            m_Scale = 0;
            m_CacheLen = 0;
        }

        void Reset()
        {
            Clean();
#if defined (_WIN32)
            m_InitTime = GetTickCount();
#elif defined (__linux__) || defined (__FreeBSD__)
            struct timeval tv;
            gettimeofday(&tv, NULL);
            m_InitTime = ((_UInt64)tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
#endif
        }

        void* Register(_UInt32 duration/*millisecond*/, const TimeoutEvent& cb, void* args)
        {
            return Register(duration, cb, args, NULL);
        }

        void Cancel(void* hdl)
        {
            if (hdl == NULL) return;
            struct Handler* handler = (struct Handler *)hdl;
            handler->m_Index = -2;
        }

        void Update()
        {
#if defined (_WIN32)
                _UInt64 currTime = GetTickCount();
#elif defined (__linux__) || defined (__FreeBSD__)
                struct timeval tv;
                gettimeofday(&tv, NULL);
                _UInt64 currTime = ((_UInt64)tv.tv_sec * 1000000 + tv.tv_usec) / 1000;
#endif
            assert(currTime >= m_InitTime);
            _UInt32 scale = (_UInt32)((currTime - m_InitTime) / Precision);
            if (scale == 0) return;

            _UInt32 oldScale = m_Scale;
            m_Scale = scale % m_MaxScale;
                
            for (_UInt32 i = oldScale; i < scale && i <= m_MaxScale; ++i)
            {
                DoCallback(i);
            }

            if (scale >= m_MaxScale)
            {
                m_InitTime = currTime;
                for (_UInt32 i = 0; i < m_Scale; ++i)
                {
                    DoCallback(i);
                }
                LOG_INFO("Timer loopback, currTime: %llu", m_InitTime);
            }
        }

    private:
        struct Handler
        {
            Handler() : m_Index(-1), m_Args(NULL), m_Duration(0) {} 
            _Int32 m_Index;
            _UInt32 m_Duration;
            TimeoutEvent m_Callback;
            void* m_Args;
            IGAME_LIST_ENTRY(Handler) m_Entries;
        };

        struct Handler* AllocHandler()
        {
            struct Handler* handler;
            if (m_CacheLen <= 0) 
            {
                handler = XNEW(struct Handler)();
                assert(handler);
                return handler;
            }
				
            handler = IGAME_LIST_FIRST(&m_Cache);
            assert(handler); 
            IGAME_LIST_REMOVE_HEAD(&m_Cache, m_Entries);
            m_CacheLen--;
            return new (handler) struct Handler();
        }

        void DeallocHandler(struct Handler* handler)
        {
            assert(handler);
            if (m_CacheLen >= m_CacheSize)
            {
                SAFE_DELETE(handler);
                return;
            }
            handler->~Handler();
            IGAME_LIST_INSERT_HEAD(&m_Cache, handler, m_Entries);
            m_CacheLen++;
        }

        void* Register(_UInt32 duration/*millisecond*/, 
            const TimeoutEvent& cb, void* args, Handler* handler)
        {
            if (duration <= 0 || duration > Capacity || cb.Empty())
                return NULL;

            _Int32 index = (duration % Precision) ? 
                ((duration / Precision) + m_Scale) % (m_MaxScale + 1) :
                ((duration / Precision) + m_Scale) % (m_MaxScale + 1) - 1;

            if (index < 0) index = 0;

            if (handler == NULL)
            {
                handler = AllocHandler();
                if (!handler->m_Callback.Exist(cb)) 
                    handler->m_Callback = cb; 
                handler->m_Args = args;
                handler->m_Duration = duration;
            }

            handler->m_Index = index;

            if (m_HandlerList[index].m_UsrPtr == NULL)
                IGAME_LIST_INSERT_HEAD(&m_HandlerList[index], handler, m_Entries);
            else
                IGAME_LIST_INSERT_AFTER((struct Handler *)m_HandlerList[index].m_UsrPtr, handler, m_Entries);
            m_HandlerList[index].m_UsrPtr = handler;
            return handler;
        }

        void DoCallback(_UInt32 index)
        {
            struct Handler* handler;
            struct HandlerList handlerList;
            handlerList.m_First = m_HandlerList[index].m_First;
            //handlerList.m_UsrPtr = m_HandlerList[index].m_UsrPtr;
            IGAME_LIST_INIT(&m_HandlerList[index]);

            while (handler = IGAME_LIST_FIRST(&handlerList))
            {
                IGAME_LIST_REMOVE_HEAD(&handlerList, m_Entries);
                if (handler->m_Index < 0)
                {
                    //LOG_INFO("Timer was cancel, index: %d, handler: %p", handler->m_Index, handler);
                    DeallocHandler(handler);
                }
                else
                {
                    if (handler->m_Callback(handler->m_Args))
                    {
                        if (!Register(handler->m_Duration, handler->m_Callback, handler->m_Args, handler))
                        {
                            LOG_WARN("Failed to register timeout, handler: %p", handler);
                            DeallocHandler(handler);
                        }
                    }
                    else
                    {
                        //LOG_INFO("Timer was cancel by app, handler: %p", handler);
                        DeallocHandler(handler);
                    }
                }
            }
        }

        IGAME_LIST_HEAD(HandlerList, Handler) m_HandlerList[Capacity/(Precision+1)+1];
        IGAME_LIST_HEAD(HandlerCacheList, Handler) m_Cache;

        _UInt32 m_MaxScale;
        _UInt64 m_InitTime;     
        _UInt32 m_Scale;
        _UInt32 m_CacheSize; 
        _UInt32 m_CacheLen;
    };
} // namespace IGame

#endif // IGAME_OS_TIMEOUT_SERVICE_H


