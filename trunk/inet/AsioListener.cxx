/**
 *  Version:     @(#)AsioListener.cxx    0.0.1 23/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     To implement INET::Client by boost::asio. you can 
 *               get boost:;asio from boost website.
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

#ifdef _WITH_BOOST_ASIO

#include "AsioListener.hxx"

namespace INET
{
    class AsioListenerImpl
    {
    public:
        AsioListenerImpl(AsioListener* listener, AsioService& service, uint16_t port)
            : mListener(listener) , mAsioService(service)
            , mAsioService(*(asio::io_service *)(service.getService()))
            , mAsioListener(mAsioService, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)) 
        {}

        virtual ~AsioListenerImpl() {}

        bool create(uint16_t port, const int8_t* ip = 0) 
        {

        }

        bool start(uint32_t backlog = 200, bool ssl = false)
        {

        }

        void update()
        {

        }

        void close()
        {

        }

        AsioListener* mListener;
        AsioService* mService;
        asio::ip::tcp::acceptor mAcceptor;
    };

} // namespace

using namespace INET;

AsioListener::AsioListener(AsioService& service, uint16_t port) 
{
    mImpl = new AsioListenerImpl(this, service, port);
}

AsioListener::~AsioListener()
{
    delete mImpl;
}

bool 
AsioListener::create(uint16_t port, const int8_t* ip)
{
    assert(mImpl);
    mImpl->create(port, ip);
}

bool
AsioListener::start(uint32_t backlog, bool ssl)
{
    assert(mImpl); 
    mImpl->start(backlog, ssl);
}

void
AsioListener::update()
{
    assert(mImpl);
    mImpl->update();
}

void
AsioListener::close()
{
    assert(mImpl);
    mImpl->close();
}

#endif // #ifdef _WITH_BOOST_ASIO

