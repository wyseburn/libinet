/**
 *  Version:     @(#)Service.cxx    0.0.1 24/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     boost::asio::io_service wrapper class.  
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

#ifndef __INET_SERVICE_H__
#define __INET_SERVICE_H__

#include "Compat.hxx"

namespace INet
{
    class ServiceImpl;
    class Service
    {
    public:
        Service();
        ~Service();

        void* get();
        UInt32 poll();
        UInt32 run();

    private:
        ServiceImpl* mImpl; 
    };
} // namespace

#endif // #ifndef __INET_SERVICE_H__
