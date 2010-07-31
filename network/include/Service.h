/**
 * Author: Hailong.Xia (hlxxxx@gmail.com)
 * Copyright @ 2008-2009 IGameStudio All Rights Reserved
 */

#ifndef IGAME_NETWORK_SERVICE_H
#define IGAME_NETWORK_SERVICE_H

#include "os/include/Compat.h"

namespace IGame
{
    class ServiceImpl;
    class Service
    {
    public:
        Service();
        virtual ~Service();

        void* Get();
        _UInt32 Poll();
        _UInt32 PollOne();
        _UInt32 Run();
        _UInt32 RunOne();
        void Stop();

    private:
        ServiceImpl* m_Impl;
    };
} // namespace IGame

#endif // #ifndef IGAME_NETWORK_SERVICE_H


