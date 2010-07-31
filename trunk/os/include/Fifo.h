/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_FIFO_H
#define IGAME_OS_FIFO_H 

#include "os/include/Mutex.h"
#include "os/include/Condition.h"
#include "os/include/Time.h"
#include "os/include/Container.h"

namespace IGame
{
    template <typename Type >
    class Fifo
    {
    public:
        Fifo(const Type& nullValue = 0) : m_NullValue(nullValue) {}
        ~Fifo() {}

        inline _UInt32 Size() const
        {
            Lock lock(m_Mutex);
            return m_List.Size();
        }

        inline bool Empty() const
        {
            Lock lock(m_Mutex);
            return m_List.m_Empty();
        }
         
        inline void Clear()
        {
            Lock lock(m_Mutex);
            m_List.Clear();
            m_Condition.Signal();
        }
         
        inline void Push(Type value)
        {
            Lock lock(m_Mutex);
            m_List.InsertTail(value);
            m_Condition.Signal();
        }

        inline void PushHead(Type value)
        {
            Lock lock(m_Mutex);
            m_List.InsertHead(value);
            m_Condition.Signal();
        }

        Type Pop()
        {
            Lock lock(m_Mutex);

            if (m_List.Empty())
            {
                return m_NullValue;
            }            
            // Return the first message on the fifo.
            typename List<Type >::Node* node = m_List.Begin();
            if (node == NULL) return m_NullValue;
            Type value = node->m_Value;
            m_List.Erase(node);
            return value;
        }

        std::vector<Type > Pops(_UInt32 maxCount = 10)
        {
            std::vector<Type > retVector;
            Lock lock(m_Mutex);

            if (m_List.Empty())
            {
                return retVector;
            }

            while (1)
            {
                for (typename List<Type >::Node* node = m_List.Begin(); node != m_List.End();)
                {
                    if (maxCount <= 0) 
                    {
                        break;
                    }
                    retVector.push_back(node->m_Value);
                    node = m_List.Erase(node);
                    maxCount--;
                }
                return retVector;
            }
        }

        Type GetNext()
        {
            Lock lock(m_Mutex);
            
            // Wait util there are messages available.
            while (m_List.Empty())
            {
                m_Condition.Wait(m_Mutex);
            }
            
            // Return the first message on the fifo.
            typename List<Type >::Node* node = m_List.Begin();
            if (node == NULL) return m_NullValue;
            Type value = node->m_Value;
            m_List.Erase(node);
            return value;
        }
         
        Type GetNext(_UInt32 ms)
        {
            assert(ms > 0);
            const _UInt64 begin(Time::GetTimeMilliSec());
            const _UInt64 end(begin + (unsigned int)(ms)); 
            Lock lock(m_Mutex); 

            // Wait until there are messages available
            while (m_List.Empty())
            {
               const _UInt64 now(Time::GetTimeMilliSec());
               if(now >= end)
               {
                   return m_NullValue;
               }

               unsigned int timeout((unsigned int)(end - now));
              
               // bail if total wait time exceeds limit
               bool signaled = m_Condition.Wait(m_Mutex, timeout);
               if (!signaled)
               {
                   return m_NullValue;
               }
            }

            // Return the first message on the fifo.
            typename List<Type >::Node* node = m_List.Begin();
            if (node == NULL) return m_NullValue;
            Type value = node->m_Value;
            m_List.Erase(node);
            return value;
        } 

        std::vector<Type > GetNexts(_UInt32 ms, _Int32 maxCount = 10)
        {
            assert(ms > 0);
            const _UInt64 begin(Time::GetTimeMilliSec());
            const _UInt64 end(begin + (unsigned int)(ms)); 
            std::vector<Type > retVector;
            Lock lock(m_Mutex); 

            // Wait until there are messages available
            while (m_List.Empty())
            {
               const _UInt64 now(Time::GetTimeMilliSec());
               if(now >= end)
               {
                   return retVector;
               }

               unsigned int timeout((unsigned int)(end - now));
              
               // bail if total wait time exceeds limit
               bool signaled = m_Condition.Wait(m_Mutex, timeout);
               if (!signaled)
               {
                   return retVector;
               }
            }

            while (1)
            {
                for (typename List<Type >::Node* node = m_List.Begin(); node != m_List.End();)
                {
                    if (maxCount <= 0) 
                    {
                        break;
                    }
                    retVector.push_back(node->m_Value);
                    node = m_List.Erase(node);
                    maxCount--;
                }
                return retVector;
            }
        }
            
    protected:
        Type m_NullValue;
        List<Type > m_List;
        mutable Mutex m_Mutex;
        Condition m_Condition;
    };
} // namespace IGame

#endif // IGAME_OS_FIFO_H



