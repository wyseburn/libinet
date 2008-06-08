/**
 *  Version:     @(#)libinet/session.hxx    0.0.4 25/05/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     A async IO interface. You can include or inhert it.
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

#ifndef __LIBINET_SESSION_H__
#define __LIBINET_SESSION_H__

#include "service.hxx"
#include "delegate.hxx"
#include "buffer.hxx"


#define INET_REGISTER_CONNECTED(session, obj, func) \
    (session)->on_connected_ += inet::CONN_EVENT(obj, func)

#define INET_REGISTER_CONNECT_FAILED(session, obj, func) \
    (session)->on_connect_failed_ += inet::CONN_EVENT(obj, func)

#define INET_REGISTER_CONNECT_BROKEN(session, obj, func) \
    (session)->on_connect_broken_ += inet::CONN_EVENT(obj, func)

#define INET_REGISTER_RECEIVED(session, obj, func) \
    (session)->on_received_ += inet::RECV_EVENT(obj, func)

#define INET_REGISTER_SENT(session, obj, func) \
    (session)->on_sent_ += inet::SEND_EVENT(obj, func)

namespace inet
{
    class session;
    typedef enum { icmp = 0, udp, tcp } transport;
    typedef Delegate<void (session*)> CONN_EVENT;
    typedef Delegate<void (session*)> RECV_EVENT;
    typedef Delegate<void (session*, const void*, inet_uint32)> SEND_EVENT;

    class session_impl;
    class session
    {
    public:
        session(inet::service& service, inet::transport transport);
        virtual ~session();

        friend class listener_impl;

        inet::service& get_service();
        void* get_socket(); // return boost::ip::tcp::socket pointer
        inet_int32 get_socket_fd() const;
        inet::transport get_transport() const;

        void close();
        bool async_connect(const inet_int8* remote, inet_uint16 port);
        void async_send();
        void async_send(inet::buffer& buffer);
        void async_send(const void* data, inet_uint32 len);
        inet_uint32 read(void* data, inet_uint32 len) 
        { 
            return recv_buffer_.read(data, len); 
        }

        CONN_EVENT on_connected_;
        CONN_EVENT on_connect_failed_;
        CONN_EVENT on_connect_broken_;

        SEND_EVENT on_sent_;
        RECV_EVENT on_received_;
        inet::buffer send_buffer_;
        inet::buffer recv_buffer_;
        
    private:
        void async_receive();
        session_impl* impl_; 
    };
}

#endif // #ifndef __INET_SESSION_H__

