/**
 *  Version:     @(#)Session.hxx    0.0.1 25/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     A async IO interface. You can include or inhert it.
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

#ifndef __INET_SESSION_H__
#define __INET_SESSION_H__

#include "Service.hxx"
#include "Delegate.hxx"

namespace INet
{
    typedef enum { ICMP, UDP, TCP } TransportType;
    typedef Delegate<Int32 (char*, UInt32)> EventCallback;
    static const UInt32 sSessionMaxRecvBufferSize = 1024 * 64; 

    class SessionImpl;
    class Session
    {
    public:
        Session(Service& service, TransportType type);
        virtual ~Session();

        Int32 getSocket();

        void close();
        bool asyncConnect(const Int8* remote, UInt16 port);
        void asyncReceive(UInt32 nbytes); 
        void asyncSend(void* buffer, UInt32 length);
        void asyncSendTo(const Int8* remote, UInt16 port, void* buffer, UInt32 length);

        EventCallback mOnSent;
        EventCallback mOnSendFailed;
        EventCallback mOnReceived;

        EventCallback mOnConnected;
        EventCallback mOnConnectFailed;
        EventCallback mOnConnectBroken;
    private:
        SessionImpl* mImpl; 
    };
}

#endif // #ifndef __INET_SESSION_H__

