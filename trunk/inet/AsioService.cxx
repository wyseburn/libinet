/**
 *  Version:     @(#)AsioService.cxx    0.0.1 24/05/2008
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

#include "AsioService.h"

namespace INet
{
    class AsioServiceImpl
    {
    public:
        asio::io_service mService;
    };
} // namespace

INet::AsioService::AsioService()
{
    mImpl = new AsioServiceImpl(); 
    assert(mImpl);
}

INet::AsioService::~AsioService()
{
    delete mImpl;
}

INet::AsioService::get()
{
    assert(mImpl);
    return &mImpl->mService;
}

INet::AsioService::poll()
{
    assert(mImpl);
    return mImpl->mService.poll();
}

INet::AsioService::run()
{
    assert(mImpl);
    return mImpl->mService.run();
}

