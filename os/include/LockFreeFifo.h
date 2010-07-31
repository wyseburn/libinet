/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_LOCK_FREE_FIFO_H
#define IGAME_OS_LOCK_FREE_FIFO_H

#include "os/include/Compat.h"

namespace IGame
{
    template <typename DataType >
    class LockFreeFifo
    {
    public:
        struct Node
        {
            DataType m_Data;
            Node* volatile m_Next;
            Node() : m_Data(), m_Next(NULL) {}
        };
        LockFreeFifo(Node* dummyNode) : m_Pops(0), m_Pushes(0)
        {
            m_Head = m_Tail = dummyNode;
        }
        void Insert(Node* node)
        {
            node->m_Next = NULL;
            _UInt32 pushes;
            Node* tail;
            while (1)
            {
                pushes = m_Pushes;
                tail = m_Tail;
                // NOTE: The Queue has the same consideration as the Stack. if m_Tail is
                // freed on different thread, the this code can cause a access violation.
                // If the node that the tail points to is the last node
                // then update the last node to point at the new node.
                if (CAS(&(m_Tail->m_Next), reinterpret_cast<Node*>(NULL), node))
                {
                    break;
                }
                else
                {
                    // Since the tail does not point at the last node,
                    // need to keep updating the tail until it does.
                    CAS2(&m_Tail, tail, pushes, m_Tail->m_Next, pushes + 1);
                }
            }
            // If the tail points to what we thought was the last node
            // then update the tail to point to the new node.
            CAS2(&m_Tail, tail, pushes, node, pushes + 1);
        }

        Node* Remove()
        {
            DataType data = DataType();
            Node* head;
            while (1)
            {
                _UInt32 pops = m_Pops;
                _UInt32 pushes = m_Pushes;
                head = m_Head;
                Node* next = head->m_Next;
                // Verify that we did not get the pointers in the middle
                // of auther update.
                if (pops != m_Pops)
                {
                    continue;
                }
                // Check if the queue is empty
                if (head == m_Tail)
                {
                    if (next == NULL)
                    {
                        head = NULL; // queue is empty
                        break;
                    }
                    // Special case if the queue has nodes but the tail
                    // is just behind. Move the tail off of the head 
                    CAS2(&m_Tail, head, pushes, next, pushes + 1);
                }
                else if (next != NULL)
                {
                    data = next->m_Data;
                    // Move the head pointer, effectively removing the node
                    if (CAS2(&m_Head, head, pops, next, pops + 1))
                    {
                        break;
                    }
                }
            }
            if (head != NULL)
            {
                head->m_Data = data;
            }
            return head;
        }
    private:
        bool CAS(Node* volatile* _ptr, Node* oldVal, Node* newVal)
        {
            register bool f;
#ifdef __GNUC__
            __asm__ __volatile__(
                "lock; cmpxchgl %%ebx, %1;"
                "setz %0;"
                : "=r"(f), "=m"(*(_ptr))
                : "a"(oldVal), "b" (newVal)
                : "memory");
#else
            _asm
            {
                mov ecx, _ptr
                    mov eax, oldVal
                    mov ebx, newVal
                    lock cmpxchg [ecx], ebx
                    setz f
            }
#endif // __GNUC__
            return f;
        }
        bool CAS2(Node* volatile* _ptr, Node* oldVal1, _UInt32 oldVal2, Node* newVal1, _UInt32 newVal2)
        {
            register bool f;
#ifdef __GNUC__
            __asm__ __volatile__(
                "lock; cmpxchg8b %1;"
                "setz %0;"
                : "=r"(f), "=m"(*(_ptr))
                : "a"(oldVal1), "b" (newVal1), "c"(newVal2), "d"(oldVal2)
                : "memory");
#else
            _asm
            {
                mov esi, _ptr
                    mov eax, oldVal1
                    mov edx, oldVal2
                    mov ebx, newVal1
                    mov ecx, newVal2
                    lock cmpxchg8b [esi]
                setz f
            }
#endif
            return f;
        }
        Node* volatile m_Head;
        volatile unsigned int m_Pops;
        Node* volatile m_Tail;
        volatile unsigned int m_Pushes;
    };
} // namespace IGame

#endif // #ifndef IGAME_OS_LOCK_FREE_FIFO_H


