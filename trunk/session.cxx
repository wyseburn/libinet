/**
 *  Version:     @(#)libinet/session.cxx    0.0.1 25/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
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

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::system;
using namespace boost;

namespace inet
{
    class session_impl 
    {
    public:
        session_impl(inet::session* wrapper, inet::service& service, inet::transport transport) 
            : wrapper_(wrapper), service_(service), transport_(transport)
            , pending_recv_request_count_(0), pending_send_request_count_(0)
        {
            if (transport == inet::tcp)
            {
                socket_.reset(new tcp::socket(*(io_service *)service_.get())); 
                assert(socket_.get());
            }
        }

        virtual ~session_impl() 
        {
            if (socket_.get()) socket_->close(); 
        }

        inet::service& get_service() { return service_; }
        void* get_socket() { return socket_.get(); }
        inet_int32 get_socket_fd() const { return socket_->native();}
        inet::transport get_transport() const { return transport_; }
        void close() { if (socket_.get()) socket_->close(); }

        bool async_connect(const inet_int8* remote, inet_uint16 port)
        {
            if (transport_ == inet::tcp)
            {
                tcp::resolver resolver(*(io_service *)(service_.get()));
                tcp::resolver::iterator endpoint(resolver.resolve(tcp::resolver::query(remote, 
                    boost::lexical_cast<std::string>(port))));
                tcp::resolver::iterator last;
                assert(endpoint != last);

                socket_->async_connect(*endpoint, boost::bind(&session_impl::on_connected,
                    this, remote, port, asio::placeholders::error));
            }
            return true;
        }

        void async_send()
        {
            if (transport_ == inet::tcp)
            {
                inet::buffer::node* node;
                while (node = wrapper_->send_buffer_.pop_node())
                {
                    asio::async_write(*(tcp::socket *)socket_.get(), 
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
            if (!error)
            {
                async_receive();
                wrapper_->on_connected_(wrapper_);
            }
            else if (error.value() > 10054)
            {
                wrapper_->on_connect_failed_(wrapper_);
            }
            else
            {
                wrapper_->on_connect_broken_(wrapper_);
            }
        }

        void on_received(inet_uint32 nbytes, inet::buffer::node* node, const error_code& error)
        {
            pending_recv_request_count_--;
            node->len_ = nbytes;
            wrapper_->recv_buffer_.push_node(node);
            if (error.value() != 0)
            {
                if (!pending_recv_request_count_ && !pending_send_request_count_)
                    wrapper_->on_connect_broken_(wrapper_);
            }
            else
            {
                async_receive();
                wrapper_->on_received_(wrapper_);
            }
        }

        void on_sent(inet::buffer::node* node, const error_code& error)
        {
            pending_send_request_count_--;
            if (error.value() != 0)
            {
                if (!pending_recv_request_count_ && !pending_send_request_count_)
                {
                    wrapper_->on_connect_broken_(wrapper_);
                }
            } 
            else 
            {
                if (!wrapper_->on_sent_.IsEmpty())
                {
                     wrapper_->on_sent_(wrapper_, 
                         (const void *)((char *)node + sizeof(buffer::node)), node->len_);
                }
            }
            wrapper_->send_buffer_.dealloc_node(node);
        } 

        void async_receive()
        {
            if (transport_ == inet::tcp)
            {
                inet::buffer::node* node = wrapper_->recv_buffer_.alloc_node();
                socket_->async_receive(asio::buffer((char *)node + sizeof(inet::buffer::node), 
                    wrapper_->recv_buffer_.node_data_size_),
                    boost::bind(&session_impl::on_received, this,
                    asio::placeholders::bytes_transferred, node, asio::placeholders::error));
                pending_recv_request_count_++;
            }
        }

    private:
        inet::session* wrapper_;
        std::auto_ptr<tcp::socket> socket_; 
        inet::service& service_;
        inet::transport transport_;
        inet_uint32 pending_recv_request_count_;
        inet_uint32 pending_send_request_count_;
    };
}

inet::session::session(inet::service& service, inet::transport type)
    :send_buffer_(1000, 10), recv_buffer_(8000, 2)
{
    impl_ = new session_impl(this, service, type);
    assert(impl_);
}

inet::session::~session()
{
    delete impl_;
}

inet::service& 
inet::session::get_service()
{
    assert(impl_);
    return impl_->get_service();
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

inet::transport 
inet::session::get_transport() const 
{
    assert(impl_);
    return impl_->get_transport();
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


