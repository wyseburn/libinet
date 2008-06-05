/**
 *  Version:     @(#)Listener.cxx    0.0.1 23/05/2008
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

#include "Listener.hxx"
#include "Session.hxx"

#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::system;
using namespace boost;

namespace INet
{
    class ListenerImpl
    {
    public:
        ListenerImpl(Listener* wrapper, Service& service, UInt16 port, const Int8* ip = 0)
            : mWrapper(wrapper), mService(service)
            , mAcceptor(*(io_service *)mService.get(), tcp::endpoint(tcp::v4(), port))
        {}

        ~ListenerImpl() {}

        Service& getService() { return mService; }

        void postAsyncAccept(Session* session)
        {
            assert(session->getTransportType() == INet::TCP);
            mAcceptor.async_accept(*(tcp::socket *)session->getSocket(), 
                boost::bind(&ListenerImpl::onAccepted, this, session, asio::placeholders::error));
        }

        void onAccepted(Session* session, const error_code& error)
        {
            if (!error)
            {
                session->asyncReceive();
                mWrapper->mOnAccepted(session);
            } 
            else
            {
                ;//mWrapper->onConnectionBroken(conn, error.value());
            }
        }

        Listener* mWrapper;
        Service& mService;
        tcp::acceptor mAcceptor;
    };

} // namespace

INet::Listener::Listener(Service& service, UInt16 port, const Int8* ip)
{
    mImpl = new ListenerImpl(this, service, port, ip);
}

INet::Listener::~Listener()
{
    delete mImpl;
}

INet::Service& 
INet::Listener::getService()
{
    assert(mImpl);
    return mImpl->getService();
}

void
INet::Listener::postAsyncAccept(Session* session)
{
    assert(mImpl);
    mImpl->postAsyncAccept(session);
}

void
INet::Listener::close()
{
    assert(mImpl);
    //mImpl->close();
}
