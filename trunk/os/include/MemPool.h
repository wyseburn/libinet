/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_MEM_POOL_H
#define IGAME_OS_MEM_POOL_H

#include "os/include/MemAllocator.h"

namespace IGame
{
    template <_UInt32 NodeSize, _UInt32 Size>
    class MemPool
    {
    public:
        MemPool() : m_Length(0) {}
        virtual ~MemPool() 
        {
            for (_UInt32 i = 0; i < m_Length; ++i)
            {
                XFREE(m_Nodes[i]);
            }
        }

        inline void* Alloc()
        {
            return m_Length == 0 ? XALLOC(NodeSize) : m_Nodes[--m_Length];
        }

        inline void Dealloc(void* p)
        {
            m_Length == Size ? XFREE(p) : (m_Nodes[m_Length++] = p);
        }

    private:
        _UInt32 m_Length;
        void* m_Nodes[Size];
    };

    
} // namespace IGame

#endif // IGAME_OS_MEM_POOL_H

