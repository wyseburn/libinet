/**
 *  Version:     @(#)libinet/service.hxx    0.0.2 24/05/2008
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
 *  with this library; if not, write to the Free Software Foundation, Inc., 
 *  59 Temple  Place, Suite 330, Boston, MA 02111-1307  USA
 */

#ifndef __LIBINET_SERVICE_H__
#define __LIBINET_SERVICE_H__

#include "compat.hxx"

namespace inet
{
    class service_impl;
    class service
    {
    public:
        service();
        virtual ~service();

        void* get();
        inet_uint32 poll();
        inet_uint32 poll_one();
        inet_uint32 run();
        inet_uint32 run_one();

    private:
        service_impl* impl_; 
    };
} // namespace

#endif // #ifndef __LIBINET_SERVICE_H__
