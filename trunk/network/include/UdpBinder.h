/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#ifndef IGAME_NETWORK_DATAGRAM_H
#define IGAME_NETWORK_DATAGRAM_H

#include "network/include/Socket.h"
#include "os/include/Delegate.h"

namespace IGame
{
    class UdpBinder;
    typedef Delegate<void (UdpBinder*, char* /*recvbuf*/, _UInt32 /*recvlen*/)> UdpRecvEvent;

    class UdpBinderImpl;
    class UdpBinder : public Socket
    {
    public:
        UdpBinder(Service& service);
        virtual ~UdpBinder();

        friend class UdpBinderImpl;

        void SetBindAddress(_UInt32 bindIp, _UInt16 bindPort);
        void SetConnectAddress(_UInt32 destIp, _UInt16 destPort);

        void AsyncSend(const char* data, _UInt32 len);
        void AsyncSend(_UInt32 dstIp, _UInt16 dstPort, const char* data, _UInt32 len);
        void AsyncReceive();

        UdpRecvEvent m_OnReceived;

    protected:
        _UInt32 m_BindIp;
        _UInt32 m_BindPort;
        _UInt32 m_FromIp;
        _UInt16 m_FromPort;
    private:
        UdpBinderImpl* m_Impl;
    };
} // namespace IGame

#endif // #ifndef IGAME_NETWORK_DATAGRAM_H


