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
    (listener)->on_accepted_ += std::make_pair(obj, func)

namespace inet
{
    typedef delegate<void (session*, buffer&/*istream*/, buffer&/*ostream*/)> ACCEPT_EVENT;
    typedef delegate<void ()> ACCEPT_ERROR_EVENT;
    class listener_impl;
    class listener
    {
    public:
        listener(inet::service& service, inet_uint16 port, const inet_int8* ip = 0);
        virtual ~listener();

        friend class session;
        friend class listener_impl;
        
        inet::service& get_service() { return service_; }
        inet_uint32 get_last_error() const { return errno_; }
        void async_accept(inet::session* session);
        void close();

        inet_uint32  errno_;
        ACCEPT_EVENT on_accepted_;
        ACCEPT_ERROR_EVENT on_accepted_error_;
       
    private:
        listener_impl* impl_;
        inet::service& service_;
    };
} // namespace

#endif // #ifndef __LIBINET_LISTENER_H__
