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

#include "AsioListener.hxx"
#include "AsioClient.hxx"

#include <boost/bind.hpp>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::system;
using namespace boost;

namespace INet
{
    class AsioListenerImpl
    {
    public:
        AsioListenerImpl(AsioListener* wrapper, AsioService& service)
            : mWrapper(wrapper), mService(service)
            , mAcceptor(NULL), mPendingAcceptCount(0), mPrePostAcceptCount(0)
        {}

        virtual ~AsioListenerImpl() 
        {
            delete mAcceptor;
        }

        bool create(UInt16 port, const Int8* ip = 0) 
        {
            mAcceptor = new tcp::acceptor(*(io_service *)mService.get(), tcp::endpoint(tcp::v4(), port));
            assert(mAcceptor);
			return true;
        }

        bool accept(UInt32 postCount, UInt32 backlog = 200, bool ssl = false)
        {
            mPrePostAcceptCount = postCount; 
            for (Int32 i = 0; i < mPrePostAcceptCount; i++)
            {
                AsioClient* client = new AsioClient(mService);
                tcp::socket* sock = (tcp::socket *)client->getSocket();
                mAcceptor->async_accept(*sock, boost::bind(&AsioListenerImpl::onAccepted, 
                    this, client, asio::placeholders::error));
                mPendingAcceptCount++;
            }
			return true;
        }

        void onAccepted(AsioClient* client, const error_code& error)
        {
            mPendingAcceptCount--;
            if (!error)
            {
                EventArgs args;
                args.mListener = mWrapper;
                args.mClient = client;
                mWrapper->mOnAccepted(&args, NULL);
            } 
            else
            {
                ;//mWrapper->onConnectionBroken(conn, error.value());
            }

            if (mPendingAcceptCount <= mPrePostAcceptCount/3+1)
            {
                accept(mPrePostAcceptCount/3*2+1);
            }
        }

        AsioListener* mWrapper;
        AsioService& mService;
        tcp::acceptor* mAcceptor;
        UInt32 mPendingAcceptCount;
        UInt32 mPrePostAcceptCount;
    };

} // namespace

INet::AsioListener::AsioListener(AsioService& service) 
{
    mImpl = new AsioListenerImpl(this, service);
}

INet::AsioListener::~AsioListener()
{
    delete mImpl;
}

bool 
INet::AsioListener::create(UInt16 port, const Int8* ip)
{
    assert(mImpl);
    return mImpl->create(port, ip);
}

bool
INet::AsioListener::start(UInt32 backlog, bool ssl)
{
    assert(mImpl); 
	return mImpl->accept(backlog, ssl);
}

void
INet::AsioListener::update()
{
    assert(mImpl);
    //mImpl->update();
}

void
INet::AsioListener::close()
{
    assert(mImpl);
    //mImpl->close();
}

