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

namespace INet 
{
    template <UInt32 MaxMsgId = 64> // Message handler array size
    class MsgHandlers
    {
        MsgHandlers()
        {
            memset(&mDefaultHandler, 0, sizeof(mDefaultHandler));
            memset(mHandlers, 0, sizeof(mHandlers));
        }

        virtual ~MsgHandlers()
        {
        }

        template <typename MsgType>
        void registerHandler(Int32 id, 
            const Delegate<bool (const MsgType&, void*)>& handler, void* args = NULL)
        {
            assert(id >= 0 && id < MaxMsgId);

            mHandlers[id].mWrapper = &handlerWrapper<MsgType>; 
            mHandlers[id].mFunc = (const void *)handler;
            mHandlers[id].mArgs = args;
        }

        void registerDefaultHandler(const Delegate<bool (const void*, UInt32, void*)>& handler, 
            void* args = NULL)
        {
            mDefaultHandler.mFunc = (const void *)handler;
            mDefaulthandler.mArgs = args;
        }

        bool handleMsg(Int32 id, void* data, UInt32 dataLen)
        {
            if (!mHandlers[id].mWrapper || !mHandlers[id].mFunc)
            {
                if (mDefaultHandler.mFunc)
                {
                    return mDefaultHandler.mFunc(data, dataLen, mDefaultHandler.mArgs); 
                }
                std::cout << "Cant't match a valid message handler." << std::endl;
                return false;
            }

            return mHandlers[id].mWrapper(mHandlers[id].mFunc, mHandlers[id].mArgs, data, dataLen);
        }

    private:
        template <typename MsgType>
        static bool handlerWrapper(const void* func, void* args, const void* data, UInt32 dataLen)
        {
            const MsgHandler& handler = const Delegate<bool (const MsgType&)>& func;

            MsgType msg; 
            Buffer buf(data, dataLen);
            unserialize(msg, buf);
            return handler(msg, args);
        }

        struct MsgHandler
        {
            bool (*mWrapper)(const void*, void*, const void*, UInt32);
            const void* mFunc; 
            void* mArgs;
        };

        MsgHandler mDefaultHandler;
        MsgHandler mHandlers[MaxMsgId];
    };
} // namespace

#endif // __INET_MSG_HANDLER_H__

