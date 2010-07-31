/*
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include "network/include/Errno.h"
#include "network/include/UdpBinder.h"

using namespace boost;
using namespace boost::asio;
using namespace boost::system;

namespace IGame 
{
    class UdpBinderImpl 
    {
    public:
        UdpBinderImpl(UdpBinder* wrapper) 
            : m_Wrapper(wrapper), m_Service(wrapper->m_Service)
            , m_Socket(*(io_service *)m_Wrapper->m_Service.Get())
        {
        }
            
        virtual ~UdpBinderImpl() 
        {
        }

        void AsyncSend(_UInt32 dstIp, _UInt16 dstPort, const char* data, _UInt32 len)
        {
        }

        void AsyncReceive()
        {
        }
    private:
        UdpBinder* m_Wrapper;
        ip::udp::socket m_Socket; 
        Service& m_Service;
    };
}

IGame::UdpBinder::UdpBinder(Service& service)
    : Socket(service, Socket::UDP)
{
    m_Impl = XNEW(UdpBinderImpl)(this);
    assert(m_Impl);
}

IGame::UdpBinder::~UdpBinder()
{
    SAFE_DELETE(m_Impl);
}

void 
IGame::UdpBinder::AsyncSend(_UInt32 dstIp, _UInt16 dstPort, const char* data, _UInt32 len)
{
    assert(m_Impl);
    m_Impl->AsyncSend(dstIp, dstPort, data, len);
}
        
void 
IGame::UdpBinder::AsyncReceive()
{
    assert(m_Impl);
    m_Impl->AsyncReceive();
}



