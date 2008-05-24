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

namespace INet
{
    class AsioListenerImpl
    {
    public:
        AsioListenerImpl(AsioListener* wrapper, AsioService& service)
            : mWrapper(wrapper), mAsioService(service)
            , mAcceptor(NULL), mPendingAcceptCount(0), mPrePostAcceptCount(0)
        {}

        virtual ~AsioListenerImpl() 
        {
            delete mAcceptor;
        }

        bool create(uint16_t port, const int8_t* ip = 0) 
        {
            mAcceptor = new asio::ip::tcp::acceptor(mService, 
                asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port));
            assert(mAcceptor);
        }

        bool accept(uint32_t postCount, uint32_t backlog = 200, bool ssl = false)
        {
            mPrePostAcceptCount = postCount; 
            for (int32_t i = 0; i < mPrePostAcceptCount; i++)
            {
                AsioClient* client = new AsioClient(mService);
                asio::ip::tcp::socket* sock = (asio::ip::tcp::socket *)client->getSocket();
                mAcceptor->async_accept(*sock, boost::bind(&AsioListenerImpl::onAccepted, 
                    this, client, asio::placeholders::error));
                mPendingAcceptCount++;
            }
        }

        void onAccepted(AsioClient* client, const asio::error_code& error)
        {
            mPendingAcceptCount--;
            if (!error)
            {
                EventArgs args;
                args->mListener = mWrapper;
                args->mClient = client;
                mWrapper->mOnAccpted(&client, NULL);
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
        AsioService* mService;
        asio::ip::tcp::acceptor* mAcceptor;
        uint32_t mPendingAcceptCount;
        uint32_t mPrePostAcceptCount;
    };

} // namespace

INet::AsioListener::AsioListener(AsioService& service) 
{
    mImpl = new AsioListenerImpl(this, service);
}

AsioListener::~AsioListener()
{
    delete mImpl;
}

bool 
INet::AsioListener::create(uint16_t port, const int8_t* ip)
{
    assert(mImpl);
    mImpl->create(port, ip);
}

bool
INet::AsioListener::start(uint32_t backlog, bool ssl)
{
    assert(mImpl); 
    mImpl->start(backlog, ssl);
}

void
INet::AsioListener::update()
{
    assert(mImpl);
    mImpl->update();
}

void
INet::AsioListener::close()
{
    assert(mImpl);
    mImpl->close();
}

