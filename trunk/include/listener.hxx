/**
 *  Version:     @(#)libinet/listener.hxx    0.0.2 23/05/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     To implement INET::Client by boost::asio. you can 
 *               get boost:;asio from boost website.
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

#ifndef __LIBINET_LISTENER_H__
#define __LIBINET_LISTENER_H__

#include "session.hxx"

#define INET_REGISTER_ACCEPTED(listener, obj, func) \
    (listener)->on_accepted_ += inet::ACCEPT_EVENT(obj, func)

namespace inet
{
    typedef Delegate<void (session*, buffer&/*istream*/, buffer&/*ostream*/)> ACCEPT_EVENT;
    class listener_impl;
    class listener
    {
    public:
        listener(inet::service& service, inet_uint16 port, const inet_int8* ip = 0);
        ~listener();
        
        inet::service& get_service();
        void async_accept(inet::session* session);
        void close();

        ACCEPT_EVENT on_accepted_;
       
    private:
        listener_impl* impl_;
    };
} // namespace

#endif // #ifndef __LIBINET_LISTENER_H__
