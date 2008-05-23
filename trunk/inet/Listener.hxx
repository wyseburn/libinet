/**
 *  Version:     @(#)Listener.hxx    0.0.1 23/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     INET::Listener interface class for tcp server.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty 
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
 *  the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General
 *  Public License along with this library; if not, write to the 
 *  Free Software Foundation, Inc., 59 Temple Place, Suite 330, 
 *  Boston, MA 02111-1307  USA
 */

#ifndef __INET_LISTENER_H__
#define __INET_LISTENER_H__

#include "Event.hxx"

namespace INET 
{
    class Listener
    {
    protected:
        Listener() {}

    public:
        virtual ~Listener() {}
        
        virtual bool create(uint16_t port, const int8_t* ip = 0) = 0; 
        virtual bool start(uint32_t backlog = 200, bool ssl = false) = 0;

        virtual void update() = 0;
        virtual void close() = 0;

        EventCallback mOnAccepted;
    };
} // namespace

#endif // #ifndef __INET_LISTENER_H__
