/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#ifndef IGAME_NETWORK_TIMER_H
#define IGAME_NETWORK_TIMER_H

#include "os/include/Delegate.h"
#include "network/include/Service.h"

namespace IGame
{
   typedef Delegate<void (void* /*args */)> TimerEvent;
   class TimerImpl;
   class Timer
   {
   public:
       Timer(Service& service, bool resetFlsg = true);
       virtual ~Timer();

       Service& GetService();
       void Set(_UInt64 duration/*millisecond*/, const TimerEvent& cb, void* args);
       void Cancel();

       void* m_Args;
       TimerEvent m_Callback;
   private:
       TimerImpl* m_Impl;
   };
} // namespace IGame

#endif // #ifndef IGAME_NETWORK_TIMER_H


