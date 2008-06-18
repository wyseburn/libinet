/**
 *  Version:     @(#)libinet/timer.hxx    0.0.1 19/06/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     
 *
 *  This library is free software; you can redistribute it and/or modify it under 
 *  the terms of the GNU Lesser General Public License as published by the Free 
 *  Software Foundation; either version 2 of the License, or (at your option) any 
 *  later version.
 *
 *  This library is distributed in the hope that it will be useful, but WITHOUT ANY 
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 *  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along 
 *  with this library; if not, write to the Free Software Foundation, Inc., 
 *  59 Temple  Place, Suite 330, Boston, MA 02111-1307  USA
 */

#ifndef __LIBINET_TIMER_H__
#define __LIBINET_TIMER_H__

#include "util.hxx"
#include "delegate.hxx"
#include "service.hxx"

namespace inet
{
   typedef Delegate<void (void* /*args */)> TIMER_EVENT_CB;
   class timer_impl;
   class timer
   {
   public:
       timer(inet::service& service, bool reset_flsg = true);
       ~timer();

       inet::service& get_service();
       void set(inet_uint32 duration/*millisecond*/, const TIMER_EVENT_CB& cb, void* args);
       void cancel();

       void* args_;
       TIMER_EVENT_CB callback_;

   private:
       timer_impl* impl_;
   };
} // namespace 

#endif // #ifndef __LIBINET_TIMER_H__

