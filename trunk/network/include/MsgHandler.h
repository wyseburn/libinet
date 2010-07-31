/*
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#ifndef IGAME_NETWORK_MsgHandler_H
#define IGAME_NETWORK_MsgHandler_H

#include "os/include/Logger.h"
#include "os/include/Delegate.h"
#include "network/include/MsgHdr.h"
#include "network/include/TcpConnection.h"
#include "network/include/Errno.h"

namespace IGame
{
    class MsgHandler
    {
    public:
        MsgHandler() : m_DefaultHandler(NULL)
        {
            for (_Int32 i = 0; i < IGAME_MAX_MSG_ID; i++)
            {
                m_HandlerWrapperArray[i] = NULL;
            }
        }

        virtual ~MsgHandler() 
        { 
            SAFE_DELETE(m_DefaultHandler);
            for (_Int32 i = 0; i < IGAME_MAX_MSG_ID; i++)
            {
                SAFE_DELETE(m_HandlerWrapperArray[i]);
            }
        }

        void Clean()
        {
            SAFE_DELETE(m_DefaultHandler);
            for (_Int32 i = 0; i < IGAME_MAX_MSG_ID; i++)
            {
                SAFE_DELETE(m_HandlerWrapperArray[i]);
            }
        }

        void Initialize(TcpConnection* conn, _UInt32 maxMsgLength = 0)
        {
            assert(conn);
            IGAME_REGISTER_RECEIVED(conn, this, &MsgHandler::OnReceived);
            conn->m_MaxMsgLength = maxMsgLength;
        }

        template <typename ConnType >
        void RegisterDefaultHandler(Delegate<void (ConnType&, MsgHdr&, StreamBuffer&)>* func)
        {
            if (m_DefaultHandler)
            {
                SAFE_DELETE(m_DefaultHandler);
            }

            m_DefaultHandler = XNEW(HandlerWrapper)();
            assert(m_DefaultHandler);
            m_DefaultHandler->m_Func = (char *)func;
            m_DefaultHandler->m_RecvWrapper = &DefaultHandlerWrapper<ConnType >;
        }

        template <typename ConnType, typename MsgType >
        void RegisterHandler(Delegate<void (ConnType&, const MsgType&)>* func)
        {
            HandlerWrapper* handler = XNEW(HandlerWrapper)();
            assert(handler);
            handler->m_Func = (char *)func;
            handler->m_IsStatic = false;
            handler->m_RecvWrapper = &RecvWrapper<ConnType, MsgType >;
            _UInt32 msgId = MsgId<MsgType >::s_MsgId;
            assert(msgId < IGAME_MAX_MSG_ID);
            assert(!m_HandlerWrapperArray[msgId]);
            m_HandlerWrapperArray[msgId] = handler;
        }

        template <typename ConnType, typename MsgType >
        void RegisterHandler(void (*func)(ConnType&, MsgType&))
        {
            HandlerWrapper* handler = XNEW(HandlerWrapper)();
            assert(handler);
            handler->m_Func = (char *)func;
            handler->m_IsStatic = true;
            handler->m_RecvWrapper = &RecvWrapper<ConnType, MsgType >;
            _UInt32 msgId = MsgId<MsgType >::s_MsgId;
            assert(msgId < IGAME_MAX_MSG_ID);
            assert(!m_HandlerWrapperArray[msgId]);
            m_HandlerWrapperArray[msgId] = handler;
        }

        template <class MsgType >
        void SendMsg(TcpConnection& conn, const MsgType& msg)
        {
            MsgHdr hdr;
            hdr.m_Id = MsgId<MsgType >::s_MsgId;
            hdr.m_BodyLen = SerializeSize(msg, conn.m_OStream);

            if (conn.m_MaxMsgLength && hdr.m_BodyLen >= conn.m_MaxMsgLength)
            {
                LOG_WARN("Invalid msg body len: id=%d, len=%d", hdr.m_Id, hdr.m_BodyLen);
                return;
            }

            Serialize(hdr, conn.m_OStream);
            Serialize(msg, conn.m_OStream, hdr.m_Id);  
            conn.AsyncSend();
        }

        void OnReceived(TcpConnection* conn, StreamBuffer& istream, StreamBuffer& ostream)
        {
            MsgHdr* hdr = (MsgHdr*)conn->m_UserPointer;
            if (hdr == NULL)
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

                    Unserialize(*hdr, istream);

                    if (conn->m_MaxMsgLength && hdr->m_BodyLen > conn->m_MaxMsgLength)
                    {
                        LOG_WARN("Invalidmsg body length: id=%d, len=%d", hdr->m_Id, hdr->m_BodyLen);
                        conn->m_Errno = IGAME_ERROR_MSGLEN;
                        conn->Close();
                        break;
                    }
                }

                if (hdr->m_BodyLen > istream.Length())
                {
                    conn->m_IsPending = true;
                    break;
                }

                conn->m_IsPending = false;

                if (hdr->m_Id >= IGAME_MAX_MSG_ID)
                {
                    LOG_WARN("Msg id overflow: id=%d, len=%d", hdr->m_Id, hdr->m_BodyLen);
                    conn->m_Errno = IGAME_ERROR_MSGID;
                    conn->Close();
                    break;
                }

                HandlerWrapper* handler = m_HandlerWrapperArray[hdr->m_Id];
                if (handler)
                {
                    if (!handler->m_RecvWrapper(handler->m_IsStatic, handler->m_Func, *conn, *hdr))
                    {
                        LOG_WARN("Failed to unserialize, id=%d, len=%d", hdr->m_Id, hdr->m_BodyLen);
                        conn->m_Errno = IGAME_ERROR_S11N;
                        conn->Close();
                        break;
                    }
                }
                else
                {
                    if (m_DefaultHandler)
                    {
                        m_DefaultHandler->m_RecvWrapper(m_DefaultHandler->m_IsStatic, m_DefaultHandler->m_Func, *conn, *hdr);
                    }
                    else
                    {
                        LOG_WARN("Don't match this message, id=%d, len=%d", hdr->m_Id, hdr->m_BodyLen);
                        conn->m_Errno = IGAME_ERROR_MSGHDR;
                        conn->Close();
                        break;
                    }
                }
            }
        }

    protected:
        template <typename ConnType >
        static bool DefaultHandlerWrapper(bool isStatic, char* func, TcpConnection& conn, MsgHdr& hdr)
        {
            if (isStatic)
            {
                ((void (*)(ConnType&, MsgHdr&, StreamBuffer&))func)((ConnType&)conn, hdr, conn.m_IStream);
            }
            else
            {
                ((Delegate<void (ConnType&, MsgHdr&, StreamBuffer&)>*)func)->operator () ((ConnType&)conn, hdr, conn.m_IStream);
            }
            return true;
        }

        template <typename ConnType, typename MsgType >
        static bool RecvWrapper(bool isStatic, char* func, TcpConnection& conn, MsgHdr& hdr)
        {
            MsgType msg; 
            if (!Unserialize(msg, conn.m_IStream, hdr.m_Id)) 
                return false;
            if (isStatic)
            {
                ((void (*)(ConnType&, MsgType&))func)((ConnType&)conn, msg);
            }
            else
            {
                ((Delegate<void (ConnType&, const MsgType&)>*)func)->operator () ((ConnType&)conn, msg);
            }
            return true;
        }

        struct HandlerWrapper
        {
            bool m_IsStatic;
            bool (*m_RecvWrapper)(bool isStatic, char*, TcpConnection&, MsgHdr&);
            char *m_Func;
            HandlerWrapper() : m_RecvWrapper(NULL), m_IsStatic(false), m_Func(NULL) {}
        };

        HandlerWrapper* m_DefaultHandler;
        HandlerWrapper* m_HandlerWrapperArray[IGAME_MAX_MSG_ID+1];
    };
} // namespace IGame

#endif // #ifndef IGAME_NETWORK_MsgHandler_H
