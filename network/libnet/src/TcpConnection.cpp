/*
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include "network/include/Errno.h"
#include "network/include/TcpConnection.h"

using namespace boost;
using namespace boost::asio;
using namespace boost::system;

namespace IGame 
{
    class TcpConnectionImpl 
    {
    public:
        TcpConnectionImpl(TcpConnection* wrapper) 
            : m_Wrapper(wrapper), m_Service(wrapper->m_Service)
            , m_Socket(*(ip::tcp::socket *)wrapper->GetSocket())
            , m_PendingRecvRequestCount(0), m_RequestSendRequestCount(0)
        {
        }
            
        virtual ~TcpConnectionImpl() 
        {
        }

        bool AsyncConnect(const _Int8* remote, _UInt16 port)
        {
            m_Wrapper->m_OStream.Clear();  // Hack by hailong for secure
            m_Wrapper->m_IStream.Clear();  // Hack by hailong for secure
            ip::tcp::resolver resolver(*(io_service *)(m_Service.Get()));
            ip::tcp::resolver::iterator endpoint_iterator(resolver.resolve(ip::tcp::resolver::query((const char *)remote, 
                boost::lexical_cast<std::string>(port))));
            ip::tcp::resolver::iterator last;
            assert(last != endpoint_iterator);

            m_Socket.async_connect(*endpoint_iterator, boost::bind(&TcpConnectionImpl::OnConnected,
                this, remote, port, asio::placeholders::error));

            ip::tcp::endpoint endpoint = *endpoint_iterator;
            m_Wrapper->m_FromIp = endpoint.address().to_string();
            m_Wrapper->m_FromPort = endpoint.port();
            return true;
        }

        bool CloseLinger()
        {
            _Int32 sock = (_Int32)m_Socket.native();
            assert(sock > 0);

#if defined (_WIN32)
            LINGER linger;
#elif defined (__linux__) || defined (__FreeBSD__)
            struct linger linger;
#endif
            linger.l_onoff = 1;
            linger.l_linger = 0;

            if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char *)&linger, sizeof(linger)) < 0)
            {
                return false;
            }
            return true;
        }

        bool SetKeepalive(_UInt32 keepIdle, _UInt32 keepIntvl, _UInt32 keepCount)
        {
            _Int32 sock = (_Int32)m_Socket.native();
            assert(sock > 0);

            if (keepIdle <= 0 || keepIntvl <= 0 || keepCount <= 0) 
                return false;

            int keepAlive = 1;

#ifdef _WIN32
            struct {
                unsigned long onOff;
                unsigned long keepIdle;
                unsigned long keepIntvl;
            } inKeepAlive, outKeepAlive;

            unsigned long inLen, outLen, retBytes;

            inLen = sizeof(inKeepAlive);
            outLen = sizeof(outKeepAlive);
            retBytes = 0;

            memset(&inKeepAlive, 0, sizeof(inKeepAlive));
            memset(&outKeepAlive, 0, sizeof(outKeepAlive));

            inKeepAlive.onOff = keepAlive;
            inKeepAlive.keepIntvl = keepIntvl;
            inKeepAlive.keepIdle = keepIdle;

#define SIO_KEEPALIVE_VALS _WSAIOW(IOC_VENDOR, 4)

            /* hl, keepcnt default is 3 times on windows system. */
            if (WSAIoctl((unsigned int)sock, SIO_KEEPALIVE_VALS, 
                (LPVOID)&inKeepAlive, inLen, (LPVOID)&outKeepAlive, 
                outLen, &retBytes, NULL, NULL) == -1) {
                    return false;
            }
#elif defined (__linux__)
            if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAlive, sizeof(keepAlive)) < 0)
                return false;

            if (setsockopt(sock, SOL_TCP, TCP_KEEPIDLE, (char *)&keepIdle, sizeof(keepIdle)) < 0)
                return false;

            if (setsockopt(sock, SOL_TCP, TCP_KEEPINTVL, (char *)&keepIntvl, sizeof(keepIntvl)) < 0) 
                return false;

            if (setsockopt(sock, SOL_TCP, TCP_KEEPCNT, (char *)&keepCount, sizeof(keepCount)) < 0)
                return false;
#endif
            return true;
        }

        void AsyncReceive()
        {
            StreamBuffer::Node* node = m_Wrapper->m_IStream.AllocNode();
            m_Socket.async_receive(asio::buffer((char *)node + sizeof(StreamBuffer::Node), 
                m_Wrapper->m_IStream.m_NodeDataSize),
                boost::bind(&TcpConnectionImpl::OnReceived, this,
                asio::placeholders::bytes_transferred, node, asio::placeholders::error));
            m_PendingRecvRequestCount++;
        }

        void AsyncSend()
        {
            if (m_RequestSendRequestCount == 0)
            {
                StreamBuffer::Node* node = m_Wrapper->m_OStream.PopNode();
                if (node)
                {
                    asio::async_write(m_Socket, 
                        asio::buffer((char *)node + sizeof(StreamBuffer::Node) + node->m_Off, node->m_Len),
                        boost::bind(&TcpConnectionImpl::OnSent, this, node, asio::placeholders::error));
                    m_RequestSendRequestCount++;
                }
            }
        }

        void AsyncSend(const void* data, _UInt32 len)
        {
            m_Wrapper->m_OStream.Write((const _Int8 *)data, len);
            AsyncSend();
        }

        void AsyncSend(StreamBuffer& buffer, _UInt32 len)
        {
            buffer.Read(m_Wrapper->m_OStream, len);
            AsyncSend();
        }

        void OnConnected(const std::string& remote, _UInt16 port, const error_code& error)
        {
            if (error.value() == 0)
            {
                m_Wrapper->m_IStream.Clear();  // Hack by hailong for re-connection
                m_Wrapper->m_OStream.Clear();  
                CloseLinger();
                AsyncReceive();
                m_Wrapper->m_OnConnected(m_Wrapper, m_Wrapper->m_IStream, m_Wrapper->m_OStream);
            }
            else 
            {
                m_Wrapper->m_Errno = IGAME_ERROR_CONNECT;
                m_Socket.close();
                if (!m_Wrapper->m_OnConnectFailed.Empty())
                {
                    m_Wrapper->m_OnConnectFailed(m_Wrapper, m_Wrapper->m_IStream, m_Wrapper->m_OStream);
                }
            }
        }

        void OnReceived(_UInt32 nbytes, StreamBuffer::Node* node, const error_code& error)
        {
            m_PendingRecvRequestCount--;
            node->m_Len = nbytes;
            m_Wrapper->m_IStream.PushNode(node);
            if (error.value() != 0)
            {
                if (m_PendingRecvRequestCount == 0 && m_RequestSendRequestCount == 0)
                {
                    if (!(error.value() == 104 || error.value() == 54))
                    {
                        m_Wrapper->Close();
                    }
                    if (!m_Wrapper->m_OnConnectBroken.Empty())
                    {
                        m_Wrapper->m_OnConnectBroken(m_Wrapper, m_Wrapper->m_IStream, m_Wrapper->m_OStream);
                    }
                }
            }
            else
            {
                AsyncReceive();
                m_Wrapper->m_OnReceived(m_Wrapper, m_Wrapper->m_IStream, m_Wrapper->m_OStream);
            }
        }

        void OnSent(StreamBuffer::Node* node, const error_code& error)
        {
            m_RequestSendRequestCount--;
            if (error.value() != 0)
            {
                m_Wrapper->m_OStream.DeallocNode(node);
                if (m_PendingRecvRequestCount == 0 && m_RequestSendRequestCount == 0)
                {
                    if (!(error.value() == 104 || error.value() == 54))
                    {
                        m_Wrapper->Close(); 
                    }
                    if (!m_Wrapper->m_OnConnectBroken.Empty())
                    {
                        m_Wrapper->m_OnConnectBroken(m_Wrapper, m_Wrapper->m_IStream, m_Wrapper->m_OStream);
                    }
                }
            } 
            else 
            {
                if (!m_Wrapper->m_OnSent.Empty())
                {
                     m_Wrapper->m_OnSent(m_Wrapper, (const void *)((char *)node + sizeof(StreamBuffer::Node)), node->m_Len);
                }

                m_Wrapper->m_OStream.DeallocNode(node);
                AsyncSend();
            }
        } 

    private:
        TcpConnection* m_Wrapper;
        ip::tcp::socket& m_Socket; 
        Service& m_Service;
        _UInt32 m_PendingRecvRequestCount;
        _UInt32 m_RequestSendRequestCount;
    };
}

IGame::TcpConnection::TcpConnection(Service& service)
    : m_IStream(8172, 4), m_OStream(8172, 4), m_FromIp(""), m_FromPort(0)
    , Socket(service, Socket::TCP), m_UserPointer(NULL), m_MaxMsgLength(0)
    , m_Id(0), m_ListenerId(0), m_IsPending(false)
{
    m_Impl = XNEW(TcpConnectionImpl)(this);
    assert(m_Impl);
}

IGame::TcpConnection::~TcpConnection()
{
    SAFE_DELETE(m_Impl);
}

void
IGame::TcpConnection::Reset()
{
    SAFE_DELETE(m_Impl);
    m_Impl = XNEW(TcpConnectionImpl)(this);
    assert(m_Impl);
}

bool
IGame::TcpConnection::AsyncConnect(const _Int8* remote, _UInt16 port)
{
    assert(m_Impl);
    if (remote)
    {
        m_FromIp = remote;
    }
    m_FromPort = port;
    return m_Impl->AsyncConnect(remote, port);
}

bool 
IGame::TcpConnection::CloseLinger()
{
    assert(m_Impl);
    return m_Impl->CloseLinger();
}

bool 
IGame::TcpConnection::SetKeepalive(_UInt32 keepIdle, _UInt32 keepIntvl, _UInt32 keepCount)
{
    assert(m_Impl);
    return m_Impl->SetKeepalive(keepIdle, keepIntvl, keepCount);
}

void
IGame::TcpConnection::AsyncReceive()
{
    assert(m_Impl);
    m_Impl->AsyncReceive();
}

void
IGame::TcpConnection::AsyncSend()
{
    assert(m_Impl);
    m_Impl->AsyncSend();
}

void
IGame::TcpConnection::AsyncSend(const void* data, _UInt32 len)
{
    assert(m_Impl);
    m_Impl->AsyncSend(data, len);
}

void 
IGame::TcpConnection::AsyncSend(StreamBuffer& buffer, _UInt32 len)
{
    assert(m_Impl);
    m_Impl->AsyncSend(buffer, len);
}


