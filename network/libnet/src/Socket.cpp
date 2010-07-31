/*
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#include <boost/asio.hpp>
#include "network/include/Socket.h"

using namespace boost;
using namespace boost::asio;
using namespace boost::system;

namespace IGame 
{
    class SocketImpl 
    {
    public:
        SocketImpl(Socket* wrapper) 
            : m_Wrapper(wrapper)
            , m_Socket(*(io_service *)m_Wrapper->m_Service.Get())
        {
        }
            
        virtual ~SocketImpl() 
        {
            m_Socket.close();
        }

        void* GetSocket() { return &m_Socket; }
        _Int32 GetSocketFd() { return (_Int32)m_Socket.native();}

        void Close() 
        { 
            m_Socket.close();
        }

        void Shutdown() 
        { 
            m_Socket.shutdown(ip::tcp::socket::shutdown_both); 
        }

        void SetBlocking()
        {
        }

        void SetNonBlocking()
        {
        }
    private:
        Socket* m_Wrapper;
        ip::tcp::socket m_Socket; 
    };
}

IGame::Socket::Socket(Service& service, TransportType type)
    : m_Service(service), m_TransportType(type), m_Errno(0), m_IsPending(false)
{
    m_Impl = XNEW(SocketImpl)(this);
    assert(m_Impl);
}

IGame::Socket::~Socket()
{
    SAFE_DELETE(m_Impl);
}

void 
IGame::Socket::Close()
{
    assert(m_Impl);
    m_Impl->Close();
}

void 
IGame::Socket::Shutdown()
{
    assert(m_Impl);
    m_Impl->Shutdown();
}

void 
IGame::Socket::SetBlocking()
{

}

void 
IGame::Socket::SetNonBlocking()
{

}

void* 
IGame::Socket::GetSocket()
{
    assert(m_Impl);
    return m_Impl->GetSocket();
}

_Int32 
IGame::Socket::GetSocketFd()
{
    assert(m_Impl);
    return m_Impl->GetSocketFd();
}
