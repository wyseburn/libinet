/*
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "network/include/Errno.h"
#include "network/include/TcpListener.h"
#include "os/include/Logger.h"

using namespace boost;
using namespace boost::asio;
using namespace boost::system;

namespace IGame
{
    class TcpListenerImpl
    {
    public:
        TcpListenerImpl(TcpListener* wrapper, Service& service, _UInt16 port, const _Int8* ip)
            : m_Wrapper(wrapper), m_Service(service)
            , m_Acceptor(*(io_service *)service.Get(), ip::tcp::endpoint(ip::tcp::v4(), port))
        {}

        virtual ~TcpListenerImpl() 
        {
        }

        void Close()
        {
            m_Acceptor.close();
        }

        void AsyncAccept(TcpConnection* conn)
        {
            assert(conn->GetTransportType() == Socket::TCP);
            m_Acceptor.async_accept(*(ip::tcp::socket *)conn->GetSocket(),
                boost::bind(&TcpListenerImpl::OnAccepted, this, conn, asio::placeholders::error));
        }

        void OnAccepted(TcpConnection* conn, const error_code& error)
        {
            if (error.value() == 0)
            {
                ip::tcp::socket* sock = (ip::tcp::socket *)conn->GetSocket();
                ip::tcp::endpoint endpoint = sock->remote_endpoint();
                conn->m_FromIp = endpoint.address().to_string();
                conn->m_FromPort = endpoint.port();
                //socket_base::linger option(false, 0);
                //((ip::tcp::socket *)conn->get_socket())->set_option(option);
                conn->CloseLinger();
                conn->AsyncReceive();
                //if (!on_accept_.empty())
                m_Wrapper->m_OnAccepted(conn, conn->m_IStream, conn->m_OStream);
            } 
            else
            {
                m_Wrapper->m_Errno = IGAME_ERROR_ACCEPT;
                m_Wrapper->Close();
                if (!m_Wrapper->m_OnAcceptError.Empty()) 
                    m_Wrapper->m_OnAcceptError(conn);
                LOG_ERROR("Fatal error from Accept\n");
            }
        }

        TcpListener* m_Wrapper;
        Service& m_Service;
        ip::tcp::acceptor m_Acceptor;
    };

} // namespace

IGame::TcpListener::TcpListener(Service& service, _UInt16 port, const _Int8* ip)
: m_Service(service), m_Id(0), m_MaxMsgLength(0), m_BindPort(port)
{
    if (ip != 0)
    {
        m_BindIp = ip;
    }
    m_Impl = XNEW(TcpListenerImpl)(this, service, port, ip);
}

IGame::TcpListener::~TcpListener()
{
    SAFE_DELETE(m_Impl);
}

void
IGame::TcpListener::AsyncAccept(TcpConnection* conn)
{
    assert(m_Impl);
    m_Impl->AsyncAccept(conn);
}

void
IGame::TcpListener::Close()
{
    assert(m_Impl);
    m_Impl->Close();
}

