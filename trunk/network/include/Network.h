/*
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#ifndef IGAME_NETWORK_NETWORK_H
#define IGAME_NETWORK_NETWORK_H

#include "os/include/Compat.h"
#include "os/include/Delegate.h"
#include "os/include/Fifo.h"
#include "os/include/Container.h"
#include "os/include/Thread.h"
#include "network/include/Timer.h"
#include "network/include/TcpConnection.h"
#include "network/include/TcpListener.h"
#include "network/include/MsgHdr.h"

namespace IGame
{
    typedef Delegate<void (_UInt16/*netId*/, _UInt16/*rootNetId*/, const std::string&/*ip*/, _UInt16/*port*/)> NetworkEvent;
    class Network : public Thread
    {
    public:
        Network() : m_IsShutdown(true) 
        {
            for (_Int32 i = 0; i < IGAME_MAX_MSG_ID; i++)
            {
                m_HandlerWrapperArray[i] = NULL;
            }

            // Initialize default send driver timer.
            m_Timer.reset(XNEW(Timer)(m_Service));
            assert(m_Timer.get());
            m_Timer->Set(10, TimerEvent(this, &Network::OnTimer), NULL);
        }

        virtual ~Network() 
        {
            for (_Int32 i = 0; i < IGAME_MAX_MSG_ID; i++)
            {
                SAFE_DELETE(m_HandlerWrapperArray[i]);
            }
        }

        struct MsgWrapper
        {
            _UInt16 m_MsgId;
            _UInt16 m_NetId;
            void* m_Payload;
            MsgWrapper() : m_MsgId(0), m_NetId(0), m_Payload(0) {}
        };

        struct Event
        {
            _UInt16 m_Type;
            _UInt16 m_NetId;
            _UInt16 m_RootNetId;
            std::string m_Ip;
            _UInt16 m_Port;

            enum {
                EVENT_NONE                         = 0,
                EVENT_STOP                         = 1,
                EVENT_SOCKET_CLOSE                 = 2,
                EVENT_SOCKET_ACCEPT                = 3,
                EVENT_SOCKET_ON_ACCEPTED           = 4,
                EVENT_SOCKET_ON_ACCEPT_BROKEN      = 5,
                EVENT_SOCKET_CONNECT               = 6,
                EVENT_SOCKET_ON_CONNECTED          = 7,
                EVENT_SOCKET_ON_CONNECT_FAILED     = 8,
                EVENT_SOCKET_ON_CONNECT_BROKEN     = 9,
            };

            Event() : m_Type(EVENT_NONE), m_NetId(0), m_RootNetId(0), m_Ip(""), m_Port(0) {}
        };

        void Start();
        void Stop();
        void Update(_UInt32 ms = 0);

        void AsyncAccept(_UInt16 bindPort);
        void AsyncConnect(const std::string& remoteIp, _UInt16 remotePort);
        void AsyncClose(_UInt16 netId);

        NetworkEvent m_OnAccepted;
        NetworkEvent m_OnAcceptBroken;
        NetworkEvent m_OnConnected;
        NetworkEvent m_OnConnectFailed;
        NetworkEvent m_OnConnectBroken;

        void RegisterDefaultMsg(Delegate<void (Network&, const StreamBuffer&)>* func)
        {
            if (m_DefaultHandler.m_Func && m_DefaultHandler.m_IsStatic == false)
            {
                XDELETE(m_DefaultHandler.m_Func);
                m_DefaultHandler.m_Func = NULL;
            }
            m_DefaultHandler.m_IsStatic = false;
            m_DefaultHandler.m_Func = (char *)func;
        }

        void RegisterDefaultMsg(void (*func)(Network&, const StreamBuffer&))
        {
            if (m_DefaultHandler.m_Func && m_DefaultHandler.m_IsStatic == false)
            {
                XDELETE(m_DefaultHandler.m_Func);
                m_DefaultHandler.m_Func = NULL;
            }
            m_DefaultHandler.m_IsStatic = true;
            m_DefaultHandler.m_Func = (char *)func;
        }

        template <typename MsgType >
        void RegisterMsg(Delegate<void (Network&, _UInt16/*netId*/, const MsgType&)>* func)
        {
            HandlerWrapper* handlerWrapper = XNEW(HandlerWrapper)();
            assert(handlerWrapper);
            handlerWrapper->m_IsStatic = false;
            handlerWrapper->m_RecvWrapper = &RecvWrapper<MsgType >;
            handlerWrapper->m_DispatchWrapper = &DispatchWrapper<MsgType >;
            handlerWrapper->m_Func = (char *)func;
            _UInt32 msgId = MsgId<MsgType >::s_MsgId;
            assert(msgId > 0);
            assert(msgId < 0xFFFF);
            assert(msgId < IGAME_MAX_MSG_ID);
            assert(!m_HandlerWrapperArray[msgId]);
            m_HandlerWrapperArray[msgId] = handlerWrapper;
        }

        template <typename MsgType >
        void RegisterMsg(void (*func)(Network&, _UInt16/*netId*/, const MsgType&))
        {
            HandlerWrapper* handlerWrapper = XNEW(HandlerWrapper)();
            assert(handlerWrapper);
            handlerWrapper->m_IsStatic = true;
            handlerWrapper->m_RecvWrapper = &RecvWrapper<MsgType >;
            handlerWrapper->m_DispatchWrapper = &DispatchWrapper<MsgType >;
            handlerWrapper->m_Func = (char *)func;
            _UInt32 msgId = MsgId<MsgType >::s_MsgId;
            assert(msgId > 0);
            assert(msgId < 0xFFFF);
            assert(msgId < IGAME_MAX_MSG_ID);
            assert(!m_HandlerWrapperArray[msgId]);
            m_HandlerWrapperArray[msgId] = handlerWrapper;
        }

        template <typename MsgType >
        void SendMsg(const MsgType& msg, _UInt16 netId)
        {
            StreamBuffer* buf = XNEW(StreamBuffer)(1004, 0);
            MsgHdr hdr;
            hdr.m_Id = MsgId<MsgType >::s_MsgId;
            hdr.m_BodyLen = SerializeSize(msg, *buf);
            Serialize(hdr, *buf);
            Serialize(msg, *buf, MsgId<MsgType >::s_MsgId);

            MsgWrapper* msgWrapper = XNEW(MsgWrapper)();
            assert(msgWrapper);
            msgWrapper->m_MsgId = MsgId<MsgType >::s_MsgId;
            msgWrapper->m_NetId = netId;
            msgWrapper->m_Payload = buf;
            m_OFifo.Push(msgWrapper);
        }

    private:
        virtual void ThreadFunc();
        void OnTimer(void* args);
        void OnAccepted(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
        void OnConnected(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
        void OnConnectFailed(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
        void OnConnectBroken(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);
        void OnReceived(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream);

        template <typename MsgType >
        static bool RecvWrapper(Network& network, _UInt32 msgId, TcpConnection& conn)
        {
            MsgType* msg = XNEW(MsgType)();
            assert(msg);

            if (!Unserialize(*msg, conn.m_IStream, msgId))
            {
                SAFE_DELETE(msg);
                return false;
            }

            MsgWrapper* msgWrapper = XNEW(MsgWrapper)();
            assert(msgWrapper);
            msgWrapper->m_MsgId = msgId;
            msgWrapper->m_NetId = conn.m_Id;
            msgWrapper->m_Payload = msg;
            network.m_IFifo.Push(msgWrapper);
            return true;
        }

        template <typename MsgType >
        static void DispatchWrapper(Network& network, MsgWrapper& msgWrapper, char* func, bool isStatic)
        {
            MsgType* msg = (MsgType *)msgWrapper.m_Payload;
            if (isStatic)
            {
                ((void (*)(Network&, _UInt16, const MsgType&))func)(network, msgWrapper.m_NetId, *msg);
            }
            else
            {
                ((Delegate<void (Network&, _UInt16, const MsgType&)>*)func)->operator () (network, msgWrapper.m_NetId, *msg);
            }
            XDELETE(msg);
        }

        struct HandlerWrapper
        {
            bool m_IsStatic;
            bool (*m_RecvWrapper)(Network&, _UInt32 msgId, TcpConnection&);
            void (*m_DispatchWrapper)(Network&, MsgWrapper&, char*, bool);
            char* m_Func;
            HandlerWrapper() : m_RecvWrapper(0), m_DispatchWrapper(0), m_Func(0), m_IsStatic(false) {}
        } m_DefaultHandler, *m_HandlerWrapperArray[IGAME_MAX_MSG_ID + 1];

        Service m_Service;
        Fifo<MsgWrapper*> m_IFifo;
        Fifo<MsgWrapper*> m_OFifo;
        PointerArray<void*> m_NetObjectArray;
        std::auto_ptr<Timer> m_Timer;
        bool m_IsShutdown;
        std::vector<TcpListener*> m_TcpListenerList;
    };
} // namespace IGame

#endif // #ifndef IGAME_NETWORK_NETWORK_H


