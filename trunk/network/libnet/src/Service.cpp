/*
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#include <boost/asio.hpp>

#include "network/include/Service.h"
#include "os/include/SingletonObject.h"

namespace IGame 
{
    class ServiceImpl
    {
    public:
        boost::asio::io_service m_Service;
    };
} // namespace IGame

IGame::Service::Service()
{
    m_Impl = XNEW(ServiceImpl)();
    assert(m_Impl);
}

IGame::Service::~Service()
{
    SAFE_DELETE(m_Impl);
}

void*
IGame::Service::Get()
{
    assert(m_Impl);
    return &m_Impl->m_Service;
}
    
_UInt32
IGame::Service::Poll()
{
    assert(m_Impl);
    return (_UInt32)m_Impl->m_Service.poll();
}

_UInt32
IGame::Service::PollOne()
{
    assert(m_Impl);
    return (_UInt32)m_Impl->m_Service.poll_one();
}

_UInt32
IGame::Service::Run()
{
    assert(m_Impl);
    return (_UInt32)m_Impl->m_Service.run();
}

_UInt32
IGame::Service::RunOne()
{
    assert(m_Impl);
    return (_UInt32)m_Impl->m_Service.run_one();
}

void
IGame::Service::Stop()
{
    assert(m_Impl);
    m_Impl->m_Service.stop();
}


