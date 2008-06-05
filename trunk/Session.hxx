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
#include "Buffer.hxx"

namespace INet
{
    class Session;
    typedef enum { ICMP, UDP, TCP } TransportType;
    typedef Delegate<void (Session*)> ConnEventCallback;
    typedef Delegate<void (Session*)> RecvEventCallback;
    typedef Delegate<void (Session*, const void*, UInt32)> SendEventCallback;

    class SessionImpl;
    class Session
    {
    public:
        Session(Service& service, TransportType type);
        virtual ~Session();

        friend class ListenerImpl;

        Service& getService();
        void* getSocket(); // return boost::ip::tcp::socket pointer
        Int32 getSocketFd() const;
        TransportType getTransportType() const;

        void close();
        bool asyncConnect(const Int8* remote, UInt16 port);
        void asyncSend();
        void asyncSend(const void* data, UInt32 len);

        ConnEventCallback mOnConnected;
        ConnEventCallback mOnConnectFailed;
        ConnEventCallback mOnConnectBroken;

        SendEventCallback mOnSent;
        RecvEventCallback mOnReceived;
        Buffer mSendBuffer;
        Buffer mRecvBuffer;
        
    private:
        void asyncReceive();
        SessionImpl* mImpl; 
    };
}

#endif // #ifndef __INET_SESSION_H__

