/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#ifndef IGAME_NETWORK_TCPLISTENER_H
#define IGAME_NETWORK_TCPLISTENER_H

#include "network/include/TcpConnection.h"

namespace IGame
{
    typedef Delegate<void (TcpConnection*, StreamBuffer&/*istream*/, StreamBuffer&/*ostream*/)> AcceptEvent;
    typedef Delegate<void (TcpConnection*)> AcceptErrorEvent;
    class TcpListenerImpl;
    class TcpListener
    {
    public:
        TcpListener(Service& service, _UInt16 port, const _Int8* ip = 0);
        virtual ~TcpListener();

        friend class TcpConnection;
        friend class TcpListenerImpl;
        
        void Close();
        void AsyncAccept(TcpConnection* conn);
        Service& GetService() { return m_Service; }
        _UInt32 GetLastError() { return m_Errno; }
        std::string& GetBindIp() { return m_BindIp; }
        _UInt16 GetBindPort() { return m_BindPort; }

        _UInt32 m_Id;
        _UInt32 m_MaxMsgLength;

        AcceptEvent m_OnAccepted;
        AcceptErrorEvent m_OnAcceptError;
        _UInt16 m_Errno;
    protected:
        Service& m_Service;
        std::string m_BindIp;
        _UInt16 m_BindPort;
    private:
        TcpListenerImpl* m_Impl;
    };
} // namespace IGame

#define IGAME_REGISTER_ACCEPTED(l, o, f) (l)->m_OnAccepted += std::make_pair(o, f)
#define IGAME_DEREGISTER_ACCEPTED(l, o, f) (l)->m_OnAccepted -= std::make_pair(o, f)

#endif // #ifndef IGAME_NETWORK_TCPLISTENER_H

