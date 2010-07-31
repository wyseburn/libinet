/*
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "network/include/Timer.h"

using namespace boost;
using namespace boost::asio;
using namespace boost::system;

namespace IGame
{
    class TimerImpl 
    {
    public:
        TimerImpl(Timer* wrapper, Service& service, bool resetFlag) 
            : m_Wrapper(wrapper), m_Service(service), m_Timer(*(io_service *)service.Get())
            , m_ResetFlag(resetFlag)
        {
        }

        virtual ~TimerImpl() 
        { 
        }

        Service& GetService() { return m_Service; }

        void Set(_UInt64 duration)
        {
            m_Duration = duration;
            m_Timer.expires_from_now(deadline_timer::duration_type(0, 0, 0, m_Duration * 1000));
            m_Timer.async_wait(boost::bind(&TimerImpl::Handler, this, _1));
        }

        void Cancel()
        {
            m_ResetFlag = false;
            m_Timer.cancel();
        }

        void Handler(const error_code& error)
        {
            m_Wrapper->m_Callback(m_Wrapper->m_Args);
            if (m_ResetFlag)
            {
                m_Timer.expires_from_now(deadline_timer::duration_type(0, 0, 0, m_Duration * 1000));
                m_Timer.async_wait(boost::bind(&TimerImpl::Handler, this, _1));
            }
        }

    private:
        Timer* m_Wrapper;
        Service& m_Service;
        deadline_timer m_Timer;
        _UInt64 m_Duration;
        bool m_ResetFlag;
    };
}

IGame::Timer::Timer(Service& service, bool resetFlag) : m_Args(0)
{
    m_Impl = XNEW(TimerImpl)(this, service, resetFlag);
    assert(m_Impl);
}

IGame::Timer::~Timer()
{
    SAFE_DELETE(m_Impl);
}

IGame::Service& 
IGame::Timer::GetService()
{
    assert(m_Impl);
    return m_Impl->GetService();
}

void 
IGame::Timer::Set(_UInt64 duration, const TimerEvent& cb, void* args)
{
    assert(m_Impl);
    if (!m_Callback.Exist(cb)) 
        m_Callback = cb;
    m_Args = args;
    m_Impl->Set(duration);
}

void 
IGame::Timer::Cancel()
{
    assert(m_Impl);
    m_Impl->Cancel();
}

