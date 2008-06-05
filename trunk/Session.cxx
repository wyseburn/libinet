/**
 *  Version:     @(#)SessionImpl.cxx    0.0.1 25/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     Session implement class by boost::asio. 
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

#include "Session.hxx"
#include "Buffer.hxx"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::system;
using namespace boost;

namespace INet
{
    class SessionImpl 
    {
    public:
        SessionImpl(Session* wrapper, Service& service, TransportType type) 
            : mWrapper(wrapper), mService(service), mTransportType(type)
            , mPendingRecvRequestCount(0), mPendingSendRequestCount(0)
        {
            if (mTransportType == TCP)
            {
                mSocket.reset(new tcp::socket(*(io_service *)service.get())); 
                assert(mSocket.get());
            }
        }

        virtual ~SessionImpl() 
        {
            if (mSocket.get()) mSocket->close(); 
        }

        Service& getService() { return mService; }
        void* getSocket() { return mSocket.get(); }
        Int32 getSocketFd() const { return mSocket->native();}
        TransportType getTransportType() const { return mTransportType; }
        void close() { if (mSocket.get()) mSocket->close(); }

        bool asyncConnect(const Int8* remote, UInt16 port)
        {
            if (mTransportType == TCP)
            {
                tcp::resolver resolver(*(io_service *)(mService.get()));
                tcp::resolver::iterator endpoint(resolver.resolve(tcp::resolver::query(remote, 
                    boost::lexical_cast<std::string>(port))));
                tcp::resolver::iterator last;
                assert(endpoint != last);

                mSocket->async_connect(*endpoint, boost::bind(&SessionImpl::onConnectCompletion,
                    this, remote, port, asio::placeholders::error));
            }
            return true;
        }

        void asyncSend()
        {
            if (mTransportType == TCP)
            {
                Buffer::Node* node;
                while (node = mWrapper->mSendBuffer.popNode())
                {
                    asio::async_write(*(tcp::socket *)mSocket.get(), 
                        asio::buffer((char *)node + sizeof(Buffer::Node), node->mLen),
                        boost::bind(&SessionImpl::onWriteCompletion, this, node,
                        asio::placeholders::error));
                    mPendingSendRequestCount++;
                }
            }
        }

        void asyncSend(const void* data, UInt32 len)
        {
            mWrapper->mSendBuffer.write((const Int8 *)data, len);
            asyncSend();
        }

        void onConnectCompletion(const std::string& remote, UInt16 port, const error_code& error)
        {
            if (!error)
            {
                asyncReceive();
                mWrapper->mOnConnected(mWrapper);
            }
            else if (error.value() > 10054)
            {
                mWrapper->mOnConnectFailed(mWrapper);
            }
            else
            {
                mWrapper->mOnConnectBroken(mWrapper);
            }
        }

        void onReceiveCompletion(Int32 nbytes, Buffer::Node* node, const error_code& error)
        {
            mPendingRecvRequestCount--;
            node->mLen = nbytes;
            mWrapper->mRecvBuffer.pushNode(node);
            if (error.value() != 0)
            {
                if (!mPendingRecvRequestCount && !mPendingSendRequestCount)
                    mWrapper->mOnConnectBroken(mWrapper);
            }
            else
            {
                asyncReceive();
                mWrapper->mOnReceived(mWrapper);
            }
        }

        void onWriteCompletion(Buffer::Node* node, const error_code& error)
        {
            mPendingSendRequestCount--;
            if (error.value() != 0)
            {
                if (!mPendingRecvRequestCount && !mPendingSendRequestCount)
                {
                    mWrapper->mOnConnectBroken(mWrapper);
                }
            } 
            else 
            {
                if (!mWrapper->mOnSent.IsEmpty())
                {
                     mWrapper->mOnSent(mWrapper,(const void *)((char *)node + sizeof(Buffer::Node)), node->mLen);
                }
            }
            mWrapper->mSendBuffer.deallocNode(node);
        } 

        void asyncReceive()
        {
            if (mTransportType == TCP)
            {
                Buffer::Node* node = mWrapper->mRecvBuffer.allocNode();
                mSocket->async_receive(asio::buffer((char *)node + sizeof(Buffer::Node), 
                    mWrapper->mRecvBuffer.mNodeDataSize),
                    boost::bind(&SessionImpl::onReceiveCompletion, this,
                    asio::placeholders::bytes_transferred, node, asio::placeholders::error));
                mPendingRecvRequestCount++;
            }
        }

    private:
        Session* mWrapper;
        std::auto_ptr<tcp::socket> mSocket; 
        Service& mService;
        TransportType mTransportType;
        UInt32 mPendingRecvRequestCount;
        UInt32 mPendingSendRequestCount;
    };
}

INet::Session::Session(Service& service, TransportType type)
    :mSendBuffer(1000, 10), mRecvBuffer(8000, 2)
{
    mImpl = new SessionImpl(this, service, type);
    assert(mImpl);
}

INet::Session::~Session()
{
    delete mImpl;
}

INet::Service& 
INet::Session::getService()
{
    assert(mImpl);
    return mImpl->getService();
}

void *
INet::Session::getSocket() 
{
    assert(mImpl);
    return mImpl->getSocket();
}

Int32   
INet::Session::getSocketFd() const 
{
    assert(mImpl);
    return mImpl->getSocketFd();
}

INet::TransportType 
INet::Session::getTransportType() const 
{
    assert(mImpl);
    return mImpl->getTransportType();
}

void 
INet::Session::close()
{
    assert(mImpl);
    mImpl->close();
}

bool
INet::Session::asyncConnect(const Int8* remote, UInt16 port)
{
    assert(mImpl);
    return mImpl->asyncConnect(remote, port);
}

void
INet::Session::asyncReceive()
{
    assert(mImpl);
    return mImpl->asyncReceive();
}

void
INet::Session::asyncSend()
{
    assert(mImpl);
    return mImpl->asyncSend();
}

void
INet::Session::asyncSend(const void* data, UInt32 len)
{
    assert(mImpl);
    return mImpl->asyncSend(data, len);
}


