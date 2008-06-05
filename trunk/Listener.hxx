/**
 *  Version:     @(#)Listener.hxx    0.0.1 23/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     To implement INET::Client by boost::asio. you can 
 *               get boost:;asio from boost website.
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

#include "Service.hxx"
#include "Session.hxx"

namespace INet
{
    typedef Delegate<void (Session*)> AcceptEventCallback;
    class ListenerImpl;
    class Listener
    {
    public:
        Listener(Service& service, UInt16 port, const Int8* ip = 0);
        ~Listener();
        
        Service& getService();
        void postAsyncAccept(Session* session);
        void close();

        AcceptEventCallback mOnAccepted;
       
    private:
        ListenerImpl* mImpl;
    };
} // namespace

#endif // #ifndef __INET_LISTENER_H__
