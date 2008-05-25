/**
 *  Version:    @(#)Client.hxx   0.0.1 23/05/2008
 *  Authors:    Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :    INET::Clent interface class for tcp or udp client.
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

#ifndef __INET_CLIENT_H__
#define __INET_CLIENT_H__

#include "Event.hxx"
#include "MsgHandlers.hxx"

namespace INet
{
    class Client
    {
    protected:
        Client() {}

    public:
        virtual ~Client() {}

        MsgHandlers& getHandlers() { return mHandlers; }

        virtual void update() = 0;
        virtual void connect(const Int8* host, UInt16 port, bool ssl = false) = 0;
        virtual void close() = 0;
        virtual void pause() = 0;
        virtual void resume() = 0;
        virtual void send(Message& msg) = 0;
        virtual void sentto(const Int8* ip, UInt16 port, Message& msg) = 0;
        
        EventCallback mOnConnected;
        EventCallback mOnConnectFailed;
        EventCallback mOnConnectLost;

    protected:
        MsgHandlers mHandlers;
    };
}

#endif // #ifndef __INET_CLIENT_H__
