/**
 *  Version:     @(#)AsioClient.hxx    0.0.1 23/05/2008
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

#ifndef __INET_ASIO_CLIENT_H__
#define __INET_ASIO_CLIENT_H__

#include "Client.h"
#include "AsioService.h"

namespace INet
{
    class AsioClientImpl;
    class AsioClient : Client 
    {
    public:
        AsioClient(AsioService& service);
        virtual ~AsioClient() {}

        virtual void update();
        virtual void connect(const int8_t* host, uint16_t port, bool ssl = false);
        virtual void close();
        virtual void pause();
        virtual void resume();
        virtual void send(msg& msg, bool closed = false);
        virtual void sentto(const int8_t* ip, uint16_t port, msg& msg);
    private:
        AsioClientImpl* mImpl;
    };
}

#endif // #ifndef __INET_ASIO_CLIENT_H__

