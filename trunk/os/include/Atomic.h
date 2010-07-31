/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved 
 */

#ifndef IGAME_OS_ATOMIC_H
#define IGAME_OS_ATOMIC_H

#include "os/include/Compat.h"

#if defined(__linux__) || defined(__FreeBSD__)

namespace IGame
{
    static inline void					
    AtomicAddLong(volatile unsigned long *p, unsigned long v)
    {							
        __asm __volatile("lock ; addl %1,%0"	
                         : "=m" (*p)			
                         : "ir" (v), "m" (*p));		
    }

    static inline void
    AtomicSubtractLong(volatile unsigned long *p, unsigned long v)
    {							
        __asm __volatile(" lock ; subl %1,%0"	
                         : "=m" (*p)			
                         : "ir" (v), "m" (*p));		
    }

} // namespace IGame

#define InterlockedIncrement(inc) IGame::AtomicAddLong(inc, 1)
#define InterlockedDecrement(dec) IGame::AtomicSubtractLong(dec, 1)
#endif

#endif // IGAME_OS_ATOMIC_H


