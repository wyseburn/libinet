/**
 *  Version:     @(#)libinet/session.cxx    0.0.1 25/05/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     Session implement class by boost::asio. 
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

#include "session.hxx"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>

using namespace boost;
using namespace boost::asio;
using namespace boost::system;

namespace inet
{
    class session_impl 
    {
    public:
        session_impl(inet::session* wrapper) 
            : wrapper_(wrapper), service_(wrapper->service_), transport_(wrapper->transport_)
            , socket_(*(io_service *)wrapper->service_.get()), aliving_(true)
            , pending_recv_request_count_(0), pending_send_request_count_(0)
        {
        }
            
        virtual ~session_impl() 
        {
            assert(aliving_ == false);
            socket_.close();
        }

        void* get_socket() { return &socket_; }
        inet_int32 get_socket_fd() { return socket_.native();}

        void close() 
        { 
            if (aliving_ == true)
            {
                socket_.shutdown(ip::tcp::socket::shutdown_both); 
                aliving_ = false;
            }
        }

        bool async_connect(const inet_int8* remote, inet_uint16 port)
        {
            if (transport_ == inet::tcp)
            {
                ip::tcp::resolver resolver(*(io_service *)(service_.get()));
                ip::tcp::resolver::iterator endpoint(resolver.resolve(ip::tcp::resolver::query((const char *)remote, 
                    boost::lexical_cast<std::string>(port))));
                ip::tcp::resolver::iterator last;
                assert(last != endpoint);

                socket_.async_connect(*endpoint, boost::bind(&session_impl::on_connected,
                    this, remote, port, asio::placeholders::error));
            }
            return true;
        }

        void async_receive()
        {
            if (transport_ == inet::tcp)
            {
                inet::buffer::node* node = wrapper_->recv_buffer_.alloc_node();
                socket_.async_receive(asio::buffer((char *)node + sizeof(inet::buffer::node), 
                    wrapper_->recv_buffer_.node_data_size_),
                    boost::bind(&session_impl::on_received, this,
                    asio::placeholders::bytes_transferred, node, asio::placeholders::error));
                pending_recv_request_count_++;
            }
        }

        void async_send()
        {
            if (transport_ == inet::tcp)
            {
                inet::buffer::node* node;
                while (node = wrapper_->send_buffer_.pop_node())
                {
                    asio::async_write(socket_, 
                        asio::buffer((char *)node + sizeof(inet::buffer::node), node->len_),
                        boost::bind(&session_impl::on_sent, this, node, asio::placeholders::error));
                    pending_send_request_count_++;
                }
            }
        }

        void async_send(const void* data, inet_uint32 len)
        {
            wrapper_->send_buffer_.write((const inet_int8 *)data, len);
            async_send();
        }

        void async_send(inet::buffer& buffer)
        {
            wrapper_->send_buffer_ += buffer;
            async_send();
        }

        void on_connected(const std::string& remote, inet_uint16 port, const error_code& error)
        {
            if (error.value() == 0)
            {
                //socket_base::linger option(false, 0);
                //socket_.set_option(option);
                async_receive();
                wrapper_->on_connected_(wrapper_, wrapper_->recv_buffer_, wrapper_->send_buffer_);
            }
            else if (error.value() > 10054)
            {
                wrapper_->on_connect_failed_(wrapper_, wrapper_->recv_buffer_, wrapper_->send_buffer_);
            }
            else
            { 
                wrapper_->on_connect_broken_(wrapper_, wrapper_->recv_buffer_, wrapper_->send_buffer_);
            }
        }

        void on_received(inet_uint32 nbytes, inet::buffer::node* node, const error_code& error)
        {
            pending_recv_request_count_--;
            node->len_ = nbytes;
            wrapper_->recv_buffer_.push_node(node);
            if (error.value() != 0)
            {
                if (pending_recv_request_count_ == 0 && pending_send_request_count_ == 0)
                {
                    this->close();  
                    wrapper_->on_connect_broken_(wrapper_, wrapper_->recv_buffer_, wrapper_->send_buffer_);
                }
            }
            else
            {
                async_receive();
                wrapper_->on_received_(wrapper_, wrapper_->recv_buffer_, wrapper_->send_buffer_);
            }
        }

        void on_sent(inet::buffer::node* node, const error_code& error)
        {
            pending_send_request_count_--;
            if (error.value() != 0)
            {
                if (pending_recv_request_count_ == 0 && pending_send_request_count_ == 0)
                {
                    this->close();
                    wrapper_->on_connect_broken_(wrapper_, wrapper_->recv_buffer_, wrapper_->send_buffer_);
                }
            } 
            else 
            {
                if (!wrapper_->on_sent_.empty())
                {
                     wrapper_->on_sent_(wrapper_, 
                         (const void *)((char *)node + sizeof(buffer::node)), node->len_);
                }
            }
            wrapper_->send_buffer_.dealloc_node(node);
        } 

    private:
        inet::session* wrapper_;
        ip::tcp::socket socket_; 
        inet::service& service_;
        inet::transport transport_;
        inet_uint32 pending_recv_request_count_;
        inet_uint32 pending_send_request_count_;
        bool aliving_;
    };
}

inet::session::session(inet::service& service, inet::transport type)
    :service_(service), transport_(type), send_buffer_(8000, 4), recv_buffer_(8000, 4)
{
    impl_ = new session_impl(this);
    assert(impl_);
}

inet::session::~session()
{
    delete impl_;
}

void
inet::session::reset()
{
    delete impl_;
    impl_ = new session_impl(this);
    assert(impl_);
}

void *
inet::session::get_socket() 
{
    assert(impl_);
    return impl_->get_socket();
}

inet_int32   
inet::session::get_socket_fd() const 
{
    assert(impl_);
    return impl_->get_socket_fd();
}

void 
inet::session::close()
{
    assert(impl_);
    impl_->close();
}

bool
inet::session::async_connect(const inet_int8* remote, inet_uint16 port)
{
    assert(impl_);
    return impl_->async_connect(remote, port);
}

void
inet::session::async_receive()
{
    assert(impl_);
    impl_->async_receive();
}

void
inet::session::async_send()
{
    assert(impl_);
    impl_->async_send();
}

void
inet::session::async_send(const void* data, inet_uint32 len)
{
    assert(impl_);
    impl_->async_send(data, len);
}

void 
inet::session::async_send(inet::buffer& buffer)
{
    assert(impl_);
    impl_->async_send(buffer);
}

