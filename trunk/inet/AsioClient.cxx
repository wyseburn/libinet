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

namespace INet
{
    class AsioClientImpl 
    {
    public:
        AsioClientImpl(AsioClient* wrapper, AsioService& service) 
            : mWrapper(wrapper), mService(service)
            , mPendingRecvRequestCount(0), mPendingSendRequestCount(0)
            , mRecvBufOffset(0), mRecvBufDataLength(0)
        {
            mSocket.reset(new asio::ip::tcp::socket(service)); 
            assert(mAsioSocket.get());
        }

        virtual ~AsioClientImpl() 
        {
            close();
        }

        void asyncConnect(const std::string& host, uint16_t port, bool ssl = false)
        {
            tcp::resolver resolver(*(asio::io_service *)(mService.get()));
            tcp::resolver::iterator endpoint(resolver.resolve(tcp::resolver::query(host, 
                boost::lexical_cast<std::string>(port))));
            tcp::resolver::iterator last;
            assert(endpoint != last);

            mSocket->async_connect(*endpoint, boost::bind(&AsioClientImpl::handleConnect,
                this, this, host, port, asio::placeholders::error));
        }

        void close()
        {
            if (mSocket.get()) mSocket->close();
        }

        bool asyncReceive(int32_t nbytes)
        {
            if (nbytes > Message::sMaxSize - mRecvBufDataLength)
                return false;
 
            if (nbytes > Message::sMaxSize - mRecvBufOffset - mRecvBufDataLength)
            {
                memcpy(mRecvBuf, 
                    mRecvBuf + mRecvBufOffset + mRecvBufDataLen, 
                    mRecvBufDataLength);
                mRecvBufOffset = 0;
            }

            mNextToReadBytes = nbytes;
            mSocket->async_recevie(asio::buffer(mRecvBuf + mRecvBufOffset + mRecvBufDataLength,
                Message::sMaxSize - mRecvBufOffset - mRecvBufDataLength),
                boost::bind(&AsioClientImpl::onReceiveCompletion, this,
                asio::placeholders::bytes_transferred, asio::placeholder::error));
            mPendingRecvRequestCount++;
            return true;
        }

        void asyncWrite(const Message& msg)
        {
            uint32_t length = msg.getLen();
            if (length < MsgHdr::sSize) 
            {
                std::cout << "Invalid message contents" << std::endl;
                return;
            }

            void* buffer = malloc(length);
            assert(buffer);
            memcpy(buffer, msg.get(), length);
            asio::async_write(*(asio::ip::tcp::socket *)mSocket.get(), 
                asio::buffer(buffer, length),
                boost::bind(&AsioClientImpl::onWriteCompletion, this, buffer, length, 
                asio::placeholders::error));
            mPendingSendRequestCount++;
        }

        void onConnectCompletion(const std::string& host, uint16_t port, 
            const asio::error_code& error)
        {
            if (!error)
            {
                mWrapper->mOnConnected();
                asyncReceive(MsgHdr::sSize);
            }
            else if (error.value() > 10054)
            {
                mWrapper->mOnConnectFailed();
            }
            else
            {
                mWrapper->mOnConnectBroken();
            }
        }

        void onReceiveCompletion(int32_t nbytes, const asio::error_code& error)
        {
            mPendingRecvRequestCount--;
            if (error.value() != 0)
            {
                if (mPendingRecvRequestCount > 0 || mPendingSendRequestCount > 0)
                    return;
                mWrapper->mOnConnectionLost();
            }
            else
            {
                mRecvBufDataLength += nbytes;
                if (mRecvBufDataLength < mNextToReadBytes)
                {
                    asyncReceive(client, mNextToReadBytes - mRecvBufDataLength);
                    return;
                }

                // Process data 
                if (mRecvBufDataLength < MsgHdr::sSize) // ???
                    return;

                MsgHdr* msgHdr = (MsgHdr *)(mRecvBuf + mRecvBufOffset);
                if (mRecvBufDataLength < msgHdr->mLength)
                {
                    asyncReceive(client, msgHdr->mLength - mRecvBufDataLength);
                    return;
                }

                Message msg(mRecvBuf + mRecvBufOffset, mMsgHdr->mLength); 
                if (!mWrapper->mHandlers->handleMsg(msg, NULL))
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

        void onWriteCompletion(void* buffer, uint32_t len, const asio::error_code& error)
        {
            mPendingWriteRequestCount--;
            if (error.value() != 0)
            {
                if (!mPendingReadRequestCount && !mPendingWriteRequestCount)
                    close();
                free(buffer);
            } 
            else 
            {
                Message msg(buffer, len);  
                if (mWrapper->mHandlers->handleMsg(msg, NULL))
                {
                    std::cout << "The message can't match any registered id" << std::endl;
                }
                free(buffer);
            }
        } 

    private:
        AsioClient* mWrapper;
        std::auto_ptr<asio::ip::tcp::socket> mSocket; 
        AsioService& mService;
        uint32_t mPendingRecvRequestCount;
        uint32_t mPendingSendRequestCount;
        char mRecvBuf[Message::sMaxSize]; 
        uint32_t mRecvBufOffset;
        uint32_t mRecvBufDataLength;
    };
}

INet::AsioClient::AsioClient(AsioService& service)
{
    mImpl = new AsioClientImpl(service);
}

INet::AsioClient::~AsioClient()
{
    delete mImpl;
}

void 
INet::AsioClient::update()
{

}
 
void 
INet::AsioClient::connect(const int8_t* host, uint16_t port, bool ssl)
{
    assert(mImpl);
    mImpl->connect(host, port, ssl);
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
INet::AsioClient::send(msg& msg)
{
    assert(mImpl);
    mImpl->asyncWrite(msg);
}

void 
INet::AsioClient::sentto(const int8_t* ip, uint16_t port, msg& msg)
{
    assert(mImpl);
}


