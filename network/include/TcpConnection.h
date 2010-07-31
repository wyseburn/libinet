/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#ifndef IGAME_NETWORK_CONNECTION_H
#define IGAME_NETWORK_CONNECTION_H

#include "os/include/Delegate.h"
#include "os/include/StreamBuffer.h"
#include "network/include/Socket.h"

namespace IGame
{
    class TcpConnection;
    typedef Delegate<void (TcpConnection*, StreamBuffer&/*istream*/, StreamBuffer&/*ostream*/)> TcpConnEvent;
    typedef Delegate<void (TcpConnection*, StreamBuffer&/*istream*/, StreamBuffer&/*ostream*/)> TcpRecvEvent;
    typedef Delegate<void (TcpConnection*, const void* /*buffer*/, _UInt32/*buffer length*/)> TcpSendEvent;

    class TcpConnectionImpl;
    class TcpConnection : public Socket
    {
    public:
        TcpConnection(Service& service);
        virtual ~TcpConnection();

        friend class TcpConnectionImpl;
        friend class TcpListenerImpl;

        void Reset();
        bool AsyncConnect(const _Int8* remote, _UInt16 port);
        bool CloseLinger();
        bool SetKeepalive(_UInt32 keepIdle, _UInt32 keepIntvl, _UInt32 keepCount);

        void AsyncSend();
        void AsyncSend(StreamBuffer& buffer, _UInt32 len);
        void AsyncSend(const void* data, _UInt32 len);
        void AsyncReceive();
        _UInt32 Read(void* data, _UInt32 len) 
        {
            return m_IStream.Read(data, len); 
        }

        std::string& GetFromIp() { return m_FromIp; }
        _UInt16 GetFromPort() { return m_FromPort; }


        _UInt32 m_Id;
        _UInt32 m_ListenerId;
        _UInt32 m_MaxMsgLength;
        void* m_UserPointer;
        bool m_IsPending;
        
        TcpConnEvent m_OnConnected;
        TcpConnEvent m_OnConnectFailed;
        TcpConnEvent m_OnConnectBroken;
        TcpRecvEvent m_OnReceived;
        TcpSendEvent m_OnSent;

        StreamBuffer m_IStream;
        StreamBuffer m_OStream;
        
    protected:
        std::string m_FromIp;
        _UInt16 m_FromPort;
    private:
        TcpConnectionImpl* m_Impl;
    };
} // namespace IGame

#define IGAME_REGISTER_CONNECTED(c, o, f) (c)->m_OnConnected += std::make_pair(o, f)
#define IGAME_DEREGISTER_CONNECTED(c, o, f) (c)->m_OnConnected -= std::make_pair(o, f)
#define IGAME_REGISTER_CONNECT_FAILED(c, o, f) (c)->m_OnConnectFailed += std::make_pair(o, f)
#define IGAME_DEREGISTER_CONNECT_FAILED(c, o, f) (c)->m_OnConnectFailed -= std::make_pair(o, f)
#define IGAME_REGISTER_CONNECT_BROKEN(c, o, f) (c)->m_OnConnectBroken += std::make_pair(o, f)
#define IGAME_DEREGISTER_CONNECT_BROKEN(c, o, f) (c)->m_OnConnectBroken -= std::make_pair(o, f)
#define IGAME_REGISTER_RECEIVED(c, o, f) (c)->m_OnReceived += std::make_pair(o, f)
#define IGAME_DEREGISTER_RECEIVED(c, o, f) (c)->m_OnReceived -= std::make_pair(o, f)
#define IGAME_REGISTER_SENT(c, o, f) (c)->m_OnSent += std::make_pair(o, f)
#define IGAME_DEREGISTER_SENT(c, o, f) (c)->m_OnSent -= std::make_pair(o, f)

#endif // #ifndef IGAME_NETWORK_CONNECTION_H


