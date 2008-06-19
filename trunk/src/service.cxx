/**
 *  Version:     @(#)libinet/service.cxx    0.0.2 24/05/2008
 *  Authors:     Hailong Xia <xhl_c@hotmail.com> 
 *  Brief  :     boost::asio::io_service wrapper class.  
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

#include "service.hxx"

#include <boost/asio.hpp>

namespace inet
{
    class service_impl
    {
    public:
        boost::asio::io_service service_;
    };
} // namespace

inet::service::service()
{
    impl_ = new service_impl();
    assert(impl_);
}

inet::service::~service()
{
    delete impl_;
}

void*
inet::service::get()
{
    assert(impl_);
    return &impl_->service_;
}
    
inet_uint32
inet::service::poll()
{
    assert(impl_);
    return (inet_uint32)impl_->service_.poll();
}

inet_uint32
inet::service::poll_one()
{
    assert(impl_);
    return (inet_uint32)impl_->service_.poll_one();
}

inet_uint32
inet::service::run()
{
    assert(impl_);
    return (inet_uint32)impl_->service_.run();
}

inet_uint32
inet::service::run_one()
{
    assert(impl_);
    return (inet_uint32)impl_->service_.run_one();
}

