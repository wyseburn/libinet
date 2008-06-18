/**
 *  Version:     @(#)libinet/timer.cxx    0.0.1 19/06/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :      
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

#include "timer.hxx"

#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace boost;
using namespace boost::asio;
using namespace boost::system;

namespace inet
{
    class timer_impl 
    {
    public:
        timer_impl(inet::timer* wrapper, inet::service& service, bool reset_flag) 
            : wrapper_(wrapper), service_(service), timer_(*(io_service *)service.get())
            , reset_flag_(reset_flag), strand_(*(io_service *)service.get())
        {
        }

        virtual ~timer_impl() 
        { 
        }

        inet::service& get_service() { return service_; }

        void set(inet_uint32 duration)
        {
            duration_ = duration;
            timer_.expires_from_now(deadline_timer::duration_type(0, 0, 0, duration_ * 1000));
            timer_.async_wait(strand_.wrap(boost::bind(&timer_impl::handler, this, _1)));
        }

        void cancel()
        {
            reset_flag_ = false;
        }

        void handler(const error_code& error)
        {
            wrapper_->callback_(wrapper_->args_);
            if (reset_flag_)
            {
                timer_.expires_from_now(deadline_timer::duration_type(0, 0, 0, duration_ * 1000));
                timer_.async_wait(strand_.wrap(boost::bind(&timer_impl::handler, this, _1)));
            }
        }

    private:
        inet::timer* wrapper_;
        inet::service& service_;
        deadline_timer timer_;
        inet_uint32 duration_;
        bool reset_flag_;
        boost::asio::strand strand_;

    };
}

inet::timer::timer(inet::service& service, bool reset_flag) : args_(NULL)
{
    impl_ = new timer_impl(this, service, reset_flag);
    assert(impl_);
}

inet::timer::~timer()
{
    delete impl_;
}

inet::service& 
inet::timer::get_service()
{
    assert(impl_);
    return impl_->get_service();
}

void 
inet::timer::set(inet_uint32 duration, const TIMER_EVENT_CB& cb, void* args)
{
    assert(impl_);
    callback_ = cb;
    args_ = args;
    impl_->set(duration);
}

void 
inet::timer::cancel()
{
    assert(impl_);
    impl_->cancel();
}

