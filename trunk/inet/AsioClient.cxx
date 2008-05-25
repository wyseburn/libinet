/**
 *  Version:     @(#)AsioClient.cxx    0.0.1 23/05/2008
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

#include "AsioClient.hxx"
#include "Message.hxx"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::system;
using namespace boost;

namespace INet
{
    class AsioClientImpl 
    {
    public:
        friend class AsioClient;

        AsioClientImpl(AsioClient* wrapper, AsioService& service) 
            : mWrapper(wrapper), mService(service)
            , mPendingRecvRequestCount(0), mPendingSendRequestCount(0)
            , mRecvBufOffset(0), mRecvBufDataLength(0)
        {
            mSocket.reset(new tcp::socket(*(io_service *)service.get())); 
            assert(mSocket.get());
        }

        virtual ~AsioClientImpl() 
        {
            close();
        }

        tcp::socket* getSocket()
        {
            return mSocket.get();
        }

        void asyncConnect(const std::string& host, UInt16 port, bool ssl = false)
        {
            tcp::resolver resolver(*(io_service *)(mService.get()));
            tcp::resolver::iterator endpoint(resolver.resolve(tcp::resolver::query(host, 
                boost::lexical_cast<std::string>(port))));
            tcp::resolver::iterator last;
            assert(endpoint != last);

            mSocket->async_connect(*endpoint, boost::bind(&AsioClientImpl::onConnectCompletion,
                this, host, port, asio::placeholders::error));
        }

        void close()
        {
            if (mSocket.get()) mSocket->close();
        }

        bool asyncReceive(Int32 nbytes)
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
            return true;
        }

        void asyncWrite(const Message& msg)
        {
            UInt32 length = msg.getLen();
            if (length < MsgHdr::sSize) 
            {
                std::cout << "Invalid message contents" << std::endl;
                return;
            }

            void* buffer = malloc(length);
            assert(buffer);
            memcpy(buffer, msg.get(), length);
            asio::async_write(*(tcp::socket *)mSocket.get(), 
                asio::buffer(buffer, length),
                boost::bind(&AsioClientImpl::onWriteCompletion, this, buffer, length, 
                asio::placeholders::error));
            mPendingSendRequestCount++;
        }

        void onConnectCompletion(const std::string& host, UInt16 port, const error_code& error)
        {
            if (!error)
            {
                //mWrapper->mOnConnected();
                asyncReceive(MsgHdr::sSize);
            }
            else if (error.value() > 10054)
            {
                //mWrapper->mOnConnectFailed();
            }
            else
            {
                //mWrapper->mOnConnectBroken();
            }
        }

        void onReceiveCompletion(Int32 nbytes, const error_code& error)
        {
            mPendingRecvRequestCount--;
            if (error.value() != 0)
            {
                if (mPendingRecvRequestCount > 0 || mPendingSendRequestCount > 0)
                    return;
                //mWrapper->mOnConnectionLost();
            }
            else
            {
                mRecvBufDataLength += nbytes;
                if (mRecvBufDataLength < mNextToReadBytes)
                {
                    asyncReceive(mNextToReadBytes - mRecvBufDataLength);
                    return;
                }

                // Process data 
                if (mRecvBufDataLength < MsgHdr::sSize) // ???
                    return;

                MsgHdr* msgHdr = (MsgHdr *)(mRecvBuf + mRecvBufOffset);
                if (mRecvBufDataLength < msgHdr->mLength)
                {
                    asyncReceive(msgHdr->mLength - mRecvBufDataLength);
                    return;
                }

                Message msg(mRecvBuf + mRecvBufOffset, msgHdr->mLength); 
                if (!mWrapper->getHandlers().handleMsg(msg, NULL))
                {
                    std::cout << "The message can't match any registered id" << std::endl;
                }

                mRecvBufOffset += msgHdr->mLength;
                mRecvBufDataLength -= msgHdr->mLength; 

                //if (mRecvBufOffset > Message::sMaxSize) // User operate error
                //{
                //    if (mPendingRecvRequestCount > 0 || mPendingSendRequestCount > 0)
                //        return;
                //    close();
                //}
            }
        }

        void onWriteCompletion(void* buffer, UInt32 len, const error_code& error)
        {
            mPendingSendRequestCount--;
            if (error.value() != 0)
            {
                if (!mPendingRecvRequestCount && !mPendingSendRequestCount)
                    close();
                free(buffer);
            } 
            else 
            {
                Message msg(buffer, len);  
                if (mWrapper->getHandlers().handleMsg(msg, NULL))
                {
                    std::cout << "The message can't match any registered id" << std::endl;
                }
                free(buffer);
            }
        } 

    private:
        AsioClient* mWrapper;
        std::auto_ptr<tcp::socket> mSocket; 
        AsioService& mService;
        UInt32 mPendingRecvRequestCount;
        UInt32 mPendingSendRequestCount;
        char mRecvBuf[Message::sMaxSize]; 
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


