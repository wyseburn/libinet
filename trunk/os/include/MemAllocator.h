/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_MEM_ALLOCATOR_H
#define IGAME_OS_MEM_ALLOCATOR_H

#include "os/include/Compat.h"
#include "os/include/Mutex.h"

namespace IGame
{
    const _UInt32 MEM_ALLOCATOR_HEADER_LEN  = sizeof(_UInt16);
    const _UInt32 MEM_ALLOCATOR_TAILER_LEN  = sizeof(_UInt16);
    const _UInt32 MEM_ALLOCATOR_ANNEX_LEN   = (MEM_ALLOCATOR_HEADER_LEN + MEM_ALLOCATOR_TAILER_LEN);
    const _UInt32 MEM_ALLOCATOR_CHECKCODE   = 0xFEFF;
    const _UInt32 MEM_ALLOCATOR_ALLOC_PARAM = 0x10;

    template<_UInt32 Base = 8, _UInt32 Count = 16>
    class MemAllocator
    {
    public:
        MemAllocator();
        virtual ~MemAllocator();

        virtual void* Alloc(_UInt32 size);
        virtual void* Dealloc(void* p);

    private:
        inline _UInt32 RoundUp(_UInt32 size) 
        {
            return (((size)+(Base - 1))&~(Base - 1));
        }

        inline _UInt32 GetIndex(_UInt32 size)
        {
            return (((size) + (Base - 1)) / Base - 1); 
        }

        char* AllocBlock(_UInt32 size, _UInt32* n);
        void* Refill(_UInt32 size);

        union FreeNode 
        {
            union FreeNode* m_Next;
            char m_Data[1];
        } *m_FreeList[Count];

        struct MemBlock
        {
            struct MemBlock* m_Next;
            char* m_Data; 
        } *m_BlockList;

        _UInt32 m_Size;
        mutable SpinMutex m_Mutex;
        char* m_FreeMemHead;
        char* m_FreeMemTail;
    };

    template<_UInt32 Base, _UInt32 Count>
    MemAllocator<Base, Count>::MemAllocator() : m_FreeMemHead(0), m_FreeMemTail(0)
    {
        SpinLock lock(m_Mutex);
        for (_Int32 i = 0; i < Count; i++) 
            m_FreeList[i] = NULL;
        m_BlockList = NULL;
    }

    template<_UInt32 Base, _UInt32 Count>
    MemAllocator<Base, Count>::~MemAllocator()
    {
        SpinLock lock(m_Mutex);
        MemBlock* tmpBlock = NULL;
        while (m_BlockList) 
        {
            tmpBlock = m_BlockList->m_Next;
            ::free(m_BlockList);
            m_BlockList = tmpBlock;
        }
    }

    template<_UInt32 Base, _UInt32 Count>
    void* MemAllocator<Base, Count>::Alloc(_UInt32 size)
    {
        assert(size <= 65530);
        FreeNode **freelist, *node;
        if (size + MEM_ALLOCATOR_ANNEX_LEN > Base * Count) 
        {
            node = (FreeNode*)malloc(size + MEM_ALLOCATOR_ANNEX_LEN);
        } 
        else 
        {
            SpinLock lock(m_Mutex);
            freelist = m_FreeList + GetIndex(size + MEM_ALLOCATOR_ANNEX_LEN);
            if (node = *freelist) 
            {
                *freelist = (*freelist)->m_Next;
            } 
            else 
            {
                node = (FreeNode*)Refill(RoundUp(size + MEM_ALLOCATOR_ANNEX_LEN));
            }
        }

        *((_UInt16*)(node)) = size;
        *((_UInt16*)(node->m_Data + MEM_ALLOCATOR_HEADER_LEN + size)) = MEM_ALLOCATOR_CHECKCODE;
        return node->m_Data + MEM_ALLOCATOR_HEADER_LEN;
    }

    template<_UInt32 Base, _UInt32 Count>
    void* MemAllocator<Base, Count>::Dealloc(void* p)
    {
        FreeNode* node = (FreeNode*)((char*)p - MEM_ALLOCATOR_HEADER_LEN);
        _UInt32 size = *(_UInt16*)(node);
        if (size + MEM_ALLOCATOR_ANNEX_LEN > Base * Count) 
        {
            free(node);
            return 0;
        }
        assert(*(_UInt16*)(node->m_Data + MEM_ALLOCATOR_HEADER_LEN + size) == MEM_ALLOCATOR_CHECKCODE);
        SpinLock lock(m_Mutex);
        FreeNode** freelist = m_FreeList + GetIndex(size + MEM_ALLOCATOR_ANNEX_LEN);
        node->m_Next = *freelist;
        *freelist = node;
        return 0;
    }

    template<_UInt32 Base, _UInt32 Count>
    char* MemAllocator<Base, Count>::AllocBlock(_UInt32 size, _UInt32* n)
    {
        char* result;
        _UInt32 totalBytes = size * (*n);
        _UInt32 leftBytes = (_UInt32)(m_FreeMemTail - m_FreeMemHead);

        if (leftBytes >= totalBytes) 
        {
            result = m_FreeMemHead;
            m_FreeMemHead += totalBytes;
            return result;
        } 
        else if (leftBytes >= size) 
        {
            *n = (_UInt32)(leftBytes / size);
            totalBytes = size * (*n);
            result = m_FreeMemHead;
            m_FreeMemHead += totalBytes;
            return result;
        } 
        else 
        {
            if (leftBytes > 0) 
            {
                FreeNode** freelist = m_FreeList + GetIndex(leftBytes);
                ((FreeNode*)m_FreeMemHead)->m_Next = *freelist;
                *freelist = (FreeNode*)(m_FreeMemHead);
            }

            _UInt32 allocBytes = 2 * totalBytes + RoundUp(m_Size / Count);
            MemBlock* block = (MemBlock*)malloc(sizeof(MemBlock) + allocBytes);
            block->m_Data = m_FreeMemHead = ((char *)block + sizeof(MemBlock));
            block->m_Next = m_BlockList;
            m_BlockList = block;

            if (m_FreeMemHead == NULL) 
            {
                FreeNode **freelist, *tmp;
                for (_Int32 i = size; i <= Base * Count; i += Base) 
                {
                    freelist = m_FreeList + GetIndex(i);
                    if ((tmp = *freelist) != NULL) 
                    {
                        *freelist = tmp->m_Next;
                        m_FreeMemHead = (char*)tmp;
                        m_FreeMemTail = m_FreeMemHead + i;
                        return AllocBlock(size, n);
                    }
                }
                m_FreeMemTail = 0;
                assert(0); // system memory lack
            }
            m_Size += allocBytes;
            m_FreeMemTail = m_FreeMemHead + allocBytes;
            return AllocBlock(size, n);
        }
    }

    template<_UInt32 Base, _UInt32 Count>
    void* MemAllocator<Base, Count>::Refill(_UInt32 size)
    {
        _UInt32 n = MEM_ALLOCATOR_ALLOC_PARAM;
        char* block = (char*)AllocBlock(size, &n);

        if (n == 1) return block;

        FreeNode **freelist, **freelist1;
        FreeNode *result, *currNode, *nextNode;

        freelist = freelist1 = m_FreeList + GetIndex(size);
        result = (FreeNode*)block;
        *freelist = nextNode = (FreeNode*)(block + size);

        for (_Int32 i = 1; ; ++i) 
        {
            currNode = nextNode;
            nextNode = (FreeNode*)((char*)nextNode + size);
            if (n - 1 == i) 
            {
                currNode->m_Next = NULL;
                break;
            } 
            else 
            {
                currNode->m_Next = nextNode;
            }
        }
        return result;
    }
}

#endif // IGAME_OS_MEM_ALLOCATOR_H


