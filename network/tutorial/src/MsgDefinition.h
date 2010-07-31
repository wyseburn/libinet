#ifndef IGAME_NETWORK_MSGDEFINITAION_H
#define IGAME_NETWORK_MSGDEFINITAION_H

#include "os/include/Compat.h"
#include "network/include/MsgHandler.h"

struct DemoData
{
    std::string m_Content;
    IGAME_S11N_1(DemoData, std::string, m_Content);
};

struct DemoReq
{
    _UInt32 m_Seq;
    DemoData m_Data;
    IGAME_MESSAGE_2(1, DemoReq, _UInt32, m_Seq, DemoData, m_Data);
};

struct DemoResp
{
    bool m_Ok;
    _UInt32 m_Seq;
    IGAME_MESSAGE_2(2, DemoResp, bool, m_Ok, _UInt32, m_Seq);
};

#endif // #ifndef IGAME_NETWORK_MSGDEFINITAION_H


