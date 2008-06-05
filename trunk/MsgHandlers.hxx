/*
 *  Version:     @(#)MsgHandlers.hxx    0.0.1 24/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     Message handler manager class interface 
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty 
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
 *  the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General
 *  Public License along with this library; if not, write to the 
 *  Free Software Foundation, Inc., 59 Temple Place, Suite 330, 
 *  Boston, MA 02111-1307  USA
 */

#ifndef __INET_MSG_HANDLER_H__
#define __INET_MSG_HANDLER_H__

#include "Delegate.hxx"
#include "Session.hxx"
#include "Message.hxx"

#define INET_MSG_REGISTER(handler, msgtype, obj, func) \
    (handler).registerHandler<msgtype>(new INet::Delegate<bool (const msgtype&)>(obj, func))

namespace INet 
{
    template <UInt32 MaxMsgId = 64> // Message handler array size
    class MsgHandler
    {
    public:
        MsgHandler() 
        { 
            memset(mHandlers, 0, sizeof(mHandlers));
            memset(&mDefaultHandler, 0, sizeof(mDefaultHandler));
        }

        virtual ~MsgHandler() 
        { 
            for (int i = 0; i < MaxMsgId; i++) delete mHandlers[i].mFunc;
            if (mDefaultHandler.mFunc)  delete mDefaultHandler.mFunc;
        }

        void init(Session* session)
        {
            mSession = session;
            mSession->mOnReceived += RecvEventCallback(this, &MsgHandler::onReceived);
            mSession->mOnSent += SendEventCallback(this, &MsgHandler::onSent);
        }

        template <typename MsgType>
        void registerHandler(Delegate<bool (const MsgType&)>* handler)
        {
            Int32 id = MsgId<MsgType >::sMsgID;
            assert(id >= 0 && id < MaxMsgId);
            mHandlers[id].mWrapper = &handlerWrapper<MsgType>; 
            mHandlers[id].mFunc = (void *)handler;
        }

        void registerDefaultHandler(const Delegate<bool (Buffer&)>& handler)
        {
            mDefaultHandler.mFunc = (void *)&handler;
        }

        template <class MsgType>
        void sendMsg(const MsgType& msg)
        {
            assert(mSession);
            MsgHeader header;
            header.mId = MsgId<MsgType >::sMsgID;
            header.mBodyLen = serializedSize(msg);
            serialize(header, mSession->mSendBuffer);
            serialize(msg, mSession->mSendBuffer);  
            mSession->asyncSend();
        }

        void onReceived(Session* session)
        {
            assert(mSession);

            MsgHeader header;
            unserialize(header, mSession->mRecvBuffer);

            assert(header.mId > 0); ///  ???

            if (header.mBodyLen > mSession->mRecvBuffer.length())
                return;

            if (!mHandlers[header.mId].mWrapper || !mHandlers[header.mId].mFunc)
            {
                if (mDefaultHandler.mFunc)
                    ((Delegate<bool (Buffer&)>&)mDefaultHandler.mFunc)(mSession->mRecvBuffer); 
                std::cout << "Cant't match a valid message handler." << std::endl;
                return;
            }
            mHandlers[header.mId].mWrapper(mHandlers[header.mId].mFunc, mSession->mRecvBuffer);
        }

        void onSent(Session* session, const void* data, UInt32 len)
        {
        }

    private:
        template <typename MsgType>
        static bool handlerWrapper(void* func, Buffer& buf)
        {
            MsgType msg; 
            unserialize(msg, buf);
            return ((Delegate<bool (const MsgType&)>*)func)->operator () (msg);
        }

        struct Handler
        {
            bool (*mWrapper)(void*, Buffer&);
            void* mFunc; 
        };

        Handler mDefaultHandler;
        Handler mHandlers[MaxMsgId];
        Session* mSession;
    };
} // namespace

#endif // __INET_MSG_HANDLER_H__

