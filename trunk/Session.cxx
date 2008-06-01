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
        SessionImpl(Session* wrapper, Service& service, TransportType type) 
            : mWrapper(wrapper), mService(service), mTransportType(type)
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

        bool asyncConnect(const Int8* remote, UInt16 port)
        {
            if (mTransportType == TCP)
            {
                tcp::resolver resolver(*(io_service *)(mService.get()));
                tcp::resolver::iterator endpoint(resolver.resolve(tcp::resolver::query(host, 
                    boost::lexical_cast<std::string>(port))));
                tcp::resolver::iterator last;
                 assert(endpoint != last);

                mSocket->async_connect(*endpoint, boost::bind(&AsioClientImpl::onConnectCompletion,
                    this, host, port, asio::placeholders::error));
            }

            return true;
        }

        bool asyncReceive(Int32 nbytes)
        {
            if (mTransportType == TCP)
            {
                if (nbytes > Message::sMaxSize - mRecvBufDataLength)
                    return false;
 
                if (nbytes > Message::sMaxSize - mRecvBufOffset - mRecvBufDataLength)
                {
                    memcpy(mRecvBuf, 
                        mRecvBuf + mRecvBufOffset + mRecvBufDataLength, 
                        mRecvBufDataLength);
                    mRecvBufOffset = 0;
                }

                mNextToReadBytes = nbytes;
                mSocket->async_receive(asio::buffer(mRecvBuf + mRecvBufOffset + mRecvBufDataLength,
                    Message::sMaxSize - mRecvBufOffset - mRecvBufDataLength),
                    boost::bind(&AsioClientImpl::onReceiveCompletion, this,
                    asio::placeholders::bytes_transferred, asio::placeholders::error));
                mPendingRecvRequestCount++;
            }
            return true;
        }

        void asyncSend(void* buffer, UInt32 length)
        {
            if (mTransportType == TCP)
            {
                if (buffer && length > 0)
                {
                    asio::async_write(*(tcp::socket *)mSocket.get(), 
                        asio::buffer(buffer, length),
                        boost::bind(&AsioClientImpl::onWriteCompletion, this, buffer, length, 
                        asio::placeholders::error));
                    mPendingSendRequestCount++;
                }
            }
        }

        void asyncSendTo(const Int8* remote, UInt16 port, void* buffer, UInt32 length)
        {
            return true;
        }

        void onConnectCompletion(const std::string& remote, UInt16 port, const error_code& error)
        {
            if (!error)
            {
                mWrapper->mOnConnected(NULL, 0);
                asyncReceive(MsgHdr::sSize);
            }
            else if (error.value() > 10054)
            {
                mWrapper->mOnConnectFailed(NULL, 0);
            }
            else
            {
                mWrapper->mOnConnectLost(NULL, 0);
            }
        }

        void onReceiveCompletion(Int32 nbytes, const error_code& error)
        {
            mPendingRecvRequestCount--;
            if (error.value() != 0)
            {
                if (!mPendingRecvRequestCount && !mPendingSendRequestCount)
                    close();
            }
            else
            {
                mRecvBufDataLength += nbytes;
                if (mRecvBufDataLength < mNextToReadBytes)
                {
                    asyncReceive(mNextToReadBytes - mRecvBufDataLength);
                    return;
                }

                UInt32 drain = mWrapper->mOnReceived(mRecvBuf + mRecvBufOffset, mRecvBufDataLength);

                mRecvBufOffset += drain;
                mRecvBufDataLength -= drain; 
            }
        }

        void onWriteCompletion(void* buffer, UInt32 len, const error_code& error)
        {
            mPendingSendRequestCount--;
            if (error.value() != 0)
            {
                if (!mPendingRecvRequestCount && !mPendingSendRequestCount)
                    close();
                mWrapper->mOnSentFailed(buffer, len);
            } 
            else 
            {
                mWrapper->mOnSent(buffer, len);
            }
        } 

    private:
        Session* mWrapper;
        std::auto_ptr<tcp::socket> mSocket; 
        Service& mService;
        TransportPort mTransportType;
        UInt32 mPendingRecvRequestCount;
        UInt32 mPendingSendRequestCount;
        char mRecvBuf[sSessionMaxRecvBufferSize]; 
        UInt32 mRecvBufOffset;
        UInt32 mRecvBufDataLength;
        UInt32 mNextToReadBytes;
    };
}

INet::AsioClient::AsioClient(AsioService& service)
{
    mImpl = new AsioClientImpl(this, service);
}

INet::AsioClient::~AsioClient()
{
    delete mImpl;
}

void* 
INet::AsioClient::getSocket()
{
    assert(mImpl);
    return mImpl->getSocket();
}

void 
INet::AsioClient::update()
{

}
 
void 
INet::AsioClient::connect(const Int8* host, UInt16 port, bool ssl)
{
    assert(mImpl);
    mImpl->asyncConnect(host, port, ssl);
}
        
void 
INet::AsioClient::close()
{
    assert(mImpl);
    mImpl->close();
}

void 
INet::AsioClient::pause()
{

}

void 
INet::AsioClient::resume()
{

}
        
void 
INet::AsioClient::send(Message& msg)
{
    assert(mImpl);
    mImpl->asyncWrite(msg);
}

void 
INet::AsioClient::sentto(const Int8* ip, UInt16 port, Message& msg)
{
    assert(mImpl);
}

