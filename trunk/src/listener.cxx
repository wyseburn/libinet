/**
 *  Version:     @(#)libinet/listener.cxx    0.0.2 23/05/2008
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
 *  with this library; if not, write to the Free Software Foundation, Inc., 59 Temple 
 *  Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "listener.hxx"

#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace boost;
using namespace boost::asio;
using namespace boost::system;

namespace inet
{
    class listener_impl
    {
    public:
        listener_impl(listener* wrapper, inet::service& service, inet_uint16 port, const inet_int8* ip)
            : wrapper_(wrapper), service_(service)
            , acceptor_(*(io_service *)service.get(), ip::tcp::endpoint(ip::tcp::v4(), port))
        {}

        virtual ~listener_impl() {}

        void async_accept(inet::session* session)
        {
            assert(session->get_transport() == inet::tcp);
            acceptor_.async_accept(*(ip::tcp::socket *)session->get_socket(), 
                boost::bind(&listener_impl::on_accepted, this, session, asio::placeholders::error));
        }

        void on_accepted(inet::session* session, const error_code& error)
        {
            if (error.value() == 0)
            {
                ip::tcp::socket* sock = (ip::tcp::socket *)session->get_socket();
                ip::tcp::endpoint endpoint = sock->remote_endpoint();
                session->remote_ip_ = endpoint.address().to_string();
                session->remote_port_ = endpoint.port();
                //socket_base::linger option(false, 0);
                //((ip::tcp::socket *)session->get_socket())->set_option(option);
                session->async_receive();
                wrapper_->on_accepted_(session, session->recv_buffer_, session->send_buffer_);
            } 
            else
            {
                std::cout << "Accecpt error" << std::endl;
            }
        }

        inet::listener* wrapper_;
        inet::service& service_;
        ip::tcp::acceptor acceptor_;
    };

} // namespace

inet::listener::listener(inet::service& service, inet_uint16 port, const inet_int8* ip)
    : service_(service)
{
    impl_ = new listener_impl(this, service, port, ip);
}

inet::listener::~listener()
{
    delete impl_;
}

void
inet::listener::async_accept(inet::session* session)
{
    assert(impl_);
    impl_->async_accept(session);
}

void
inet::listener::close()
{
    assert(impl_);
    //impl_->close();
}
