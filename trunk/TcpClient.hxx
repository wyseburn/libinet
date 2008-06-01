/**
 *  Version:     @(#)TcpClient.hxx    0.0.1 29/05/2008
 *  Authors:     Hailong Xia <hlxxxx@gmail.com> 
 *  Brief  :     TcpClient interface class 
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

#ifndef __INET_TCP_CLIENT_H__
#define __INET_TCP_CLIENT_H__

#include "Session.hxx"

namespace INet
{
    template <>
    class TcpClient : public Session 
    {
    public:
        TcpClient(Service& service) : Session(service, TCP)
        {
            mOnReceived += EventCallback(this, &TcpClient::onReceviced);
            mOnSent += EventCallback(this, &TcpClient::onSent);
            //mOnSendFailed += EventCallback(this, &TcpClient::onSendFailed);
        }

        virtual ~TcpClient();

        template <class MsgType>
        void asyncSend(const MsgType& msg)
        {
            //serialize(msg, buf);
            //Session::asyncSend(buf.mData, buf.mWriteLength);
        }

        Int32 onReceived(char* data, UInt32 dataLen)
        {
            assert(session);
            assert(data && dataLen > 0);

            if (dataLen < MsgHdr::sSize) // ???
            {
                std::cout << "Message length isn't enough" << std::endl;
                close();
                return 0; 
            }

            MsgHdr* msgHdr = (MsgHdr *)(data);
            if (dataLen < msgHdr->mLength)
            {
                asyncReceive(msgHdr->mLength - dataLen);
                return 0;
            }

            Message msg(data, dataLen);
            if (!mHandlers.handleMsg(msg, NULL))
            {
                std::cout << "The message can't match any registered id" << std::endl;
            }
            return msgHdr->mLength;
        }

        Int32 onSent(char* data, UInt32 dataLen)
        {
            assert(data && dataLen > 0);
            free(data);
        }
    };
}

#endif // #ifndef __INET_TCP_CLIENT_H__

