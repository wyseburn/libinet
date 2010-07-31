/*
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#include "network/include/Network.h"
#include "os/include/Logger.h"
#include "network/include/Errno.h"

using namespace IGame;

void 
Network::Start()
{
    LOG_INFO("Network engine is starting.");
    if (m_IsShutdown == false)
    {
        LOG_WARN("Network engine has started.");
        return;
    }
    m_IsShutdown = false;
    Thread::Start();
}
        
void 
Network::Stop()
{
    LOG_INFO("Network engine is stoping.");
    m_IsShutdown = true;
    Event* ev = XNEW(Event)();
    assert(ev);
    ev->m_Type = Event::EVENT_STOP;
    MsgWrapper* msgWrapper = XNEW(MsgWrapper)();
    assert(msgWrapper);
    msgWrapper->m_MsgId = 0;
    msgWrapper->m_Payload = ev;
    m_OFifo.PushHead(msgWrapper);
    Join();
}

void 
Network::ThreadFunc()
{
    LOG_INFO("Network engine has started.");
    while (!m_IsShutdown)
    {
        try
        {
            m_Service.Run();
        }
        catch (std::exception& e)
        {
            LOG_WARN("Ignore a unknown exception, %s", e.what());
        }
    }
    LOG_INFO("Network engine has stoped.");
}

void 
Network::AsyncAccept(_UInt16 bindPort)
{
    LOG_INFO("Post a async accept request at port %d", bindPort);
    Event* ev = XNEW(Event)();
    assert(ev);
    ev->m_Type = Event::EVENT_SOCKET_ACCEPT;
    ev->m_Port = bindPort;
    MsgWrapper* msgWrapper = XNEW(MsgWrapper)();
    assert(msgWrapper);
    msgWrapper->m_MsgId = 0;
    msgWrapper->m_Payload = ev;
    m_OFifo.Push(msgWrapper);
}
        
void 
Network::AsyncConnect(const std::string& remoteIp, _UInt16 remotePort)
{
    LOG_INFO("Post a async connect request at ip %s and port %d", remoteIp.c_str(), remotePort);
    Event* ev = XNEW(Event)();
    assert(ev);
    ev->m_Type = Event::EVENT_SOCKET_CONNECT;
    ev->m_Ip = remoteIp;
    ev->m_Port = remotePort;
    MsgWrapper* msgWrapper = XNEW(MsgWrapper)();
    assert(msgWrapper);
    msgWrapper->m_MsgId = 0;
    msgWrapper->m_Payload = ev;
    m_OFifo.Push(msgWrapper);
}
        
void 
Network::AsyncClose(_UInt16 netId)
{
    LOG_INFO("Post a async close request at netid %d", netId);
    Event* ev = XNEW(Event)();
    assert(ev);
    ev->m_Type = Event::EVENT_SOCKET_CLOSE;
    ev->m_NetId = netId;
    MsgWrapper* msgWrapper = XNEW(MsgWrapper)();
    assert(msgWrapper);
    msgWrapper->m_MsgId = 0;
    msgWrapper->m_Payload = ev;
    m_OFifo.Push(msgWrapper);
}

void 
Network::Update(_UInt32 ms)
{
    std::vector<MsgWrapper*> tmpVector;

    if (ms == 0)
    {
        tmpVector =  m_IFifo.Pops(100);
    }
    else
    {
        tmpVector = m_IFifo.GetNexts(ms, 100);
    }

    for (std::vector<MsgWrapper*>::iterator it = tmpVector.begin(); it != tmpVector.end(); ++it)
    {
        if ((*it) == NULL) continue;
        if ((*it)->m_Payload == NULL)
        {
            LOG_WARN("Exception for a null payload pointer.");
            XDELETE(*it);
            continue;
        }

        if ((*it)->m_MsgId == 0)
        {
            Event* ev = (Event*)(*it)->m_Payload;
            switch (ev->m_Type)
            {
            case Event::EVENT_SOCKET_ON_ACCEPTED:
                LOG_DEBUG("Receive a EVENT_SOCKET_ON_ACCEPTED event.");
                if (!m_OnAccepted.Empty())
                {
                    m_OnAccepted(ev->m_NetId, ev->m_RootNetId, ev->m_Ip, ev->m_Port);
                }
                break;
            case Event::EVENT_SOCKET_ON_ACCEPT_BROKEN:
                LOG_DEBUG("Receive a EVENT_SOCKET_ON_ACCEPT_BROKEN event.");
                if (!m_OnAcceptBroken.Empty())
                {
                    m_OnAcceptBroken(ev->m_NetId, ev->m_RootNetId, ev->m_Ip, ev->m_Port);
                }
                break;
            case Event::EVENT_SOCKET_ON_CONNECTED:
                LOG_DEBUG("Receive a EVENT_SOCKET_ON_CONNECTED event.");
                if (!m_OnConnected.Empty())
                {
                    m_OnConnected(ev->m_NetId, ev->m_RootNetId, ev->m_Ip, ev->m_Port);
                }
                break;
            case Event::EVENT_SOCKET_ON_CONNECT_FAILED:
                LOG_DEBUG("Receive a EVENT_SOCKET_ON_CONNECT_FAILED event.");
                if (!m_OnConnectFailed.Empty())
                {
                    m_OnConnectFailed(ev->m_NetId, ev->m_RootNetId, ev->m_Ip, ev->m_Port);
                }
                break;
            case Event::EVENT_SOCKET_ON_CONNECT_BROKEN:
                LOG_DEBUG("Receive a EVENT_SOCKET_ON_CONNECT_BROKEN event.");
                if (!m_OnConnectBroken.Empty())
                {
                    m_OnConnectBroken(ev->m_NetId, ev->m_RootNetId, ev->m_Ip, ev->m_Port);
                }
                break;
            }

            XDELETE(ev);
            XDELETE(*it);
            continue;
        } 
        else if ((*it)->m_MsgId == 0xFFFF) // default msg
        {
            if (m_DefaultHandler.m_Func)
            {
                if (m_DefaultHandler.m_IsStatic == true)
                {
                    ((void (*)(Network&, const StreamBuffer&))m_DefaultHandler.m_Func)
                        (*this, *(StreamBuffer*)(*it)->m_Payload);
                }
                else
                {
                    ((Delegate<void (Network&, const StreamBuffer&)>*)m_DefaultHandler.m_Func)->operator () 
                        (*this, *(StreamBuffer*)(*it)->m_Payload);
                }
            }
            XDELETE((StreamBuffer *)(*it)->m_Payload);
            XDELETE(*it);
            continue;
        }

        HandlerWrapper* handlerWrapper = m_HandlerWrapperArray[(*it)->m_MsgId];
        assert(handlerWrapper);

        handlerWrapper->m_DispatchWrapper(*this, *(*it), handlerWrapper->m_Func, handlerWrapper->m_IsStatic);
        XDELETE((*it));
    }
}

void 
Network::OnAccepted(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    IGAME_REGISTER_RECEIVED(conn, this, &Network::OnReceived);
    IGAME_REGISTER_CONNECT_BROKEN(conn, this, &Network::OnConnectBroken);

    conn->m_Id = m_NetObjectArray.Insert(conn);
    assert(conn->m_Id > 0);
    conn->m_UserPointer = (void *)XNEW(MsgHdr)();
    assert(conn->m_UserPointer);

    TcpListener* listener = (TcpListener*)m_NetObjectArray[conn->m_ListenerId];
    assert(listener);
    TcpConnection* newConn = XNEW(TcpConnection)(m_Service);
    newConn->m_ListenerId = conn->m_ListenerId;
    listener->AsyncAccept(newConn);

    Event* ev = XNEW(Event)();
    assert(ev);
    ev->m_Type = Event::EVENT_SOCKET_ON_ACCEPTED;
    ev->m_NetId = conn->m_Id;
    ev->m_RootNetId = conn->m_ListenerId;
    MsgWrapper* msgWrapper = XNEW(MsgWrapper)();
    assert(msgWrapper);
    msgWrapper->m_MsgId = 0;
    msgWrapper->m_Payload = ev;
    m_IFifo.Push(msgWrapper);
    LOG_INFO("Connection %d arrived from [%s:%d]", conn->m_Id, conn->GetFromIp().c_str(), conn->GetFromPort());
}

void 
Network::OnConnected(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    IGAME_REGISTER_RECEIVED(conn, this, &Network::OnReceived);
    IGAME_REGISTER_CONNECT_BROKEN(conn, this, &Network::OnConnectBroken);

    conn->m_Id = m_NetObjectArray.Insert(conn);
    assert(conn->m_Id > 0);
    conn->m_UserPointer = (void *)XNEW(MsgHdr)();
    assert(conn->m_UserPointer);

    Event* ev = XNEW(Event)();
    assert(ev);
    ev->m_Type = Event::EVENT_SOCKET_ON_CONNECTED;
    ev->m_NetId = conn->m_Id;
    ev->m_RootNetId = conn->m_ListenerId;
    MsgWrapper* msgWrapper = XNEW(MsgWrapper)();
    assert(msgWrapper);
    msgWrapper->m_MsgId = 0;
    msgWrapper->m_Payload = ev;
    m_IFifo.Push(msgWrapper);
    LOG_INFO("Connection established with [%s:%d]", conn->GetFromIp().c_str(), conn->GetFromPort());
}

void 
Network::OnConnectFailed(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    Event* ev = XNEW(Event)();
    assert(ev);
    ev->m_Type = Event::EVENT_SOCKET_ON_CONNECT_FAILED;
    ev->m_NetId = conn->m_Id;
    ev->m_RootNetId = conn->m_ListenerId;
    MsgWrapper* msgWrapper = XNEW(MsgWrapper)();
    assert(msgWrapper);
    msgWrapper->m_MsgId = 0;
    msgWrapper->m_Payload = ev;
    m_IFifo.Push(msgWrapper);
    LOG_INFO("Failed to connect [%s:%d]", conn->GetFromIp().c_str(), conn->GetFromPort());
    XDELETE(conn);
}

void 
Network::OnConnectBroken(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    XDELETE((MsgHdr*)conn->m_UserPointer);
    conn->m_UserPointer = NULL;

    Event* ev = XNEW(Event)();
    assert(ev);
    ev->m_Type = Event::EVENT_SOCKET_ON_CONNECT_BROKEN;
    ev->m_NetId = conn->m_Id;
    ev->m_RootNetId = conn->m_ListenerId;
    MsgWrapper* msgWrapper = XNEW(MsgWrapper)();
    assert(msgWrapper);
    msgWrapper->m_MsgId = 0;
    msgWrapper->m_Payload = ev;
    m_IFifo.Push(msgWrapper);
    LOG_INFO("Connection broken with [%s:%d]", conn->GetFromIp().c_str(), conn->GetFromPort());
    m_NetObjectArray.Erase(conn->m_Id);
    XDELETE(conn);
}

void 
Network::OnReceived(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
{
    MsgHdr* msgHdr = (MsgHdr*)conn->m_UserPointer;
    if (msgHdr == NULL)
    {
        LOG_WARN("This isn't valid msg header for received");
        conn->Close();
        return;
    }

    while (1)
    {
        if (conn->m_IsPending == false)
        {
            if (istream.Length() < 6)
                break;

            Unserialize(*msgHdr, istream);
            if (conn->m_MaxMsgLength && msgHdr->m_BodyLen > conn->m_MaxMsgLength)
            {
                LOG_WARN("Invalid msg body length: id=%d, len=%d", msgHdr->m_Id, msgHdr->m_BodyLen);
                conn->m_Errno = IGAME_ERROR_MSGLEN;
                conn->Close();
                break;
            }
        }

        if (msgHdr->m_BodyLen > istream.Length())
        {
            conn->m_IsPending = true;
            break;
        }

        conn->m_IsPending = false;

        if (msgHdr->m_Id >= IGAME_MAX_MSG_ID)
        {
            LOG_WARN("Msg id overflow: id=%d, len=%d", msgHdr->m_Id, msgHdr->m_BodyLen);
            conn->m_Errno = IGAME_ERROR_MSGID;
            conn->Close();
            break;
        }

        HandlerWrapper* handlerWrapper = m_HandlerWrapperArray[msgHdr->m_Id];
        if (handlerWrapper)
        {
            if (!handlerWrapper->m_RecvWrapper(*this, msgHdr->m_Id, *conn))
            {
                LOG_WARN("Failed to unserialize, id=%d, len=%d", msgHdr->m_Id, msgHdr->m_BodyLen);
                conn->m_Errno = IGAME_ERROR_S11N;
                conn->Close();
                break;
            }
        }
        else  // default msg 
        {
            StreamBuffer* buf = XNEW(StreamBuffer)(1004, 0);
            buf->Read(conn->m_IStream, msgHdr->m_BodyLen);
            MsgWrapper* msgWrapper = XNEW(MsgWrapper)();
            assert(msgWrapper);
            msgWrapper->m_MsgId = 0xFFFF;
            msgWrapper->m_NetId = conn->m_Id;
            msgWrapper->m_Payload = buf;
            m_IFifo.Push(msgWrapper);
        }
        OnTimer(NULL);
    }
}

void 
Network::OnTimer(void* args)
{
    std::vector<MsgWrapper*> tmpVector = m_OFifo.Pops(100);
    for (std::vector<MsgWrapper*>::iterator it = tmpVector.begin(); 
        it != tmpVector.end(); ++it)
    {
        if ((*it) == NULL) continue;

        if ((*it)->m_Payload == NULL)
        {
            LOG_WARN("Exception for a null payload pointer.");
            XDELETE((*it));
            continue;
        }

        if ((*it)->m_MsgId == 0)
        {
            Event* ev = (Event*)(*it)->m_Payload;
            switch (ev->m_Type)
            {
            case Event::EVENT_STOP:
                {
                    LOG_DEBUG("Receive a EVENT_STOP event");
                    m_Timer->Cancel();
                    m_Service.Stop();
                }
                break;
            case Event::EVENT_SOCKET_CLOSE:
                {
                    LOG_DEBUG("Receive a EVENT_SOCKET_CLOSE event.");
                    Socket* socket = (Socket *)m_NetObjectArray[ev->m_NetId];
                    if (socket == NULL) break;
                    socket->Close();
                }
                break;
            case Event::EVENT_SOCKET_ACCEPT:
                {
                    LOG_DEBUG("Receive a EVENT_SOCKET_ACCEPT event.");
                    for (std::vector<TcpListener*>::const_iterator it = m_TcpListenerList.begin();
                        it != m_TcpListenerList.end(); ++it)
                    {
                        if ((*it)->GetBindPort() == ev->m_Port)
                        {
                            assert(0);
                        }
                    }
                        
                    TcpListener* listener = XNEW(TcpListener)(m_Service, ev->m_Port);
                    assert(listener);
                    listener->m_Id = m_NetObjectArray.Insert(listener);
                    assert(listener->m_Id > 0);
                    IGAME_REGISTER_ACCEPTED(listener, this, &Network::OnAccepted);
                    TcpConnection* conn = XNEW(TcpConnection)(m_Service);
                    assert(conn);
                    conn->m_ListenerId = listener->m_Id;
                    listener->AsyncAccept(conn);
                    m_TcpListenerList.push_back(listener);
                }
                break;
            case Event::EVENT_SOCKET_CONNECT:
                {
                    LOG_DEBUG("Receive a EVENT_SOCKET_CONNECT event.");
                    TcpConnection* conn = XNEW(TcpConnection)(m_Service);
                    assert(conn);
                    conn->AsyncConnect(ev->m_Ip.c_str(), ev->m_Port);
                    IGAME_REGISTER_CONNECT_FAILED(conn, this, &Network::OnConnectFailed);
                }
                break;
            }
            XDELETE(ev);
            XDELETE(*it);
            continue;
        }

        StreamBuffer* buf = (StreamBuffer*)((*it)->m_Payload);
        TcpConnection* conn = (TcpConnection*)m_NetObjectArray[(*it)->m_NetId];
        if (conn == NULL)
        {
            LOG_DEBUG("Connection %d has missing.", (*it)->m_NetId);
            XDELETE(buf);
            XDELETE(*it);
            continue;
        }

        conn->m_OStream += *buf;
        conn->AsyncSend();

        XDELETE(buf);
        XDELETE(*it);
    }
}





