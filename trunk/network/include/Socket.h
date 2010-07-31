/*
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#ifndef IGAME_NETWORK_EVENT_SOCKET_H
#define IGAME_NETWORK_EVENT_SOCKET_H

#include "network/include/Service.h"
#include "os/include/SingletonObject.h"

namespace IGame
{
    class SocketImpl;
    class Socket
    {
    public:
        typedef enum { UDP = 1, TCP = 2 } TransportType;
        Socket(Service& service, TransportType type);
        virtual ~Socket();

        friend class SocketImpl;

        void* GetSocket();
        _UInt32 GetLastError() { return m_Errno; };
        Service& GetService() { return m_Service; }
        TransportType GetTransportType() { return m_TransportType; }
        _Int32 GetSocketFd();

        void Close();
        void Shutdown();
        void SetBlocking();
        void SetNonBlocking();

        _UInt16 m_Errno;
        bool m_IsPending;
    protected:
        Service& m_Service;
        TransportType m_TransportType;
    private:
        SocketImpl* m_Impl;
    };
} // namespace IGame

#endif // #ifndef IGAME_NETWORK_EVENT_SOCKET_H



