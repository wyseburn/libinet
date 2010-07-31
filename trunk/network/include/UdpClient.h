/*
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#ifndef IGAME_NETWORK_UDPCLIENT_H
#define IGAME_NETWORK_UDPCLIENT_H

#include "network/include/MsgHdr.h"
#include "network/include/UdpBinder.h"

namespace IGame
{
    class UdpClient : public UdpBinder
    {
    public:
        UdpClient(Service& service) : UdpBinder(service)
        {
        }

        virtual ~UdpClient() {}

        void Init()
        {
        }

        template <typename MsgType >
        void SendMsg(const MsgType& msg, MsgHdr& hdr)
        {

        }

    public:
        MsgHdr m_MsgHdr;
    };
} // namespace IGame

#endif // #ifndef IGAME_NETWORK_UDPCLIENT_H


