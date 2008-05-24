/**
 *  Version:     @(#)MsgHanders.hxx    0.0.1 23/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     Message handler define. Here, I use array instead
 *               of map to improve MsgHander lookup efficiency.
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

#ifndef __INET_MSG_HANDLERS_H__
#define __INET_MSG_HANDLERS_H__

#include "Delegate.hxx"

namespace INet 
{
    typedef Delegate<void (const Message& void*)> MsgHandler;
    static const int sMsgHandlerMaxIndex = 1024;

    class MsgHandlers
    {
    public:
        MsgHandlers()
        {
            for (int32_t i = 0; i < sMsgHandlerMaxIndex; i++)
            {
                mHandlers[i] = NULL;
            }
            mDefaultHandler = NULL;
        }

        virtual ~MsgHandlers()
        {
            for (int32_t i = 0; i < sMsgHandlerMaxIndex; i++)
            {
                if (mHandlers[i]) delete mHandlers[i];
            }
        }

        void registerHandler(int32_t id, MsgHandler* handler)
        {
            assert(id < sMsgHandlerMaxIndex);
            mHandlers[id] = handler;
        }

        void registerDefaultHandler(MsgHandler* handler)
        {
            if (mDefaultHandler) delete mDefaultHandler;
            mDefaultHandler = handler;
        }

        MsgHandler* deregisterHandler(int32_t id)
        { 
            assert(id < sMsgHandlerMaxIndex);
            if (mHandlers[id]) 
            {
                MsgHandler* handler = mHandlers[id]; 
                mHandlers[id] = NULL;
                return handler;
            } 
            return NULL; 
        } 

        MsgHandler* getHandler(int32_t id)
        {
            assert(id < sMsgHandlerMaxIndex);
            if (mHandlers[id] == NULL) return mDefaultHandler;
            return mHandlers[id];
        }

        bool handleMsg(const Message& msg, void* args)
        {
            assert(msg);
            MsgHandler* handler = getHandler(msg.getId());
            if (!handler || handler->IsEmpty()) 
                return false;
            handler->operator () (msg, args);
            return true;
        }

    private:
        MsgHandler* mDefaultHandler;
        MsgHandler* mHandlers[sMsgHandlerMaxIndex];
    };
}

#endif // #ifndef __INET_MSG_HANDLERS_H__
