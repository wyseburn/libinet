#include "network/include/Network.h"
#include "MsgDefinition.h"

struct ConnectionManager
{
    static void OnAccepted(_UInt16 netId, _UInt16 rootNetId, const std::string& ip, _UInt16 port)
    {
        std::cout << "Connection " << netId << " arrived." << std::endl;
    }

    static void OnConnectBroken(_UInt16 netId, _UInt16 rootNetId, const std::string& ip, _UInt16 port)
    {
        std::cout << "Connection " << netId << " leaved." << std::endl;
    }
};

struct MsgHandler
{
    static void OnDemoReq(IGame::Network& network, _UInt16 netId, const DemoReq& data)
    {
        std::cout << data.m_Data.m_Content.c_str() << " " << data.m_Seq << std::endl;

        DemoResp resp;
        resp.m_Ok = true;
        resp.m_Seq = data.m_Seq + 1;
        network.SendMsg(resp, netId);
    }
};

int main()
{
    LOG_INIT("", true, true);

    IGame::Network network;
    network.m_OnAccepted = &ConnectionManager::OnAccepted;
    network.m_OnConnectBroken = &ConnectionManager::OnConnectBroken;
    network.RegisterMsg(&MsgHandler::OnDemoReq);
    network.Start();
    network.AsyncAccept(12345);

    while (1)
    {
        network.Update();
        IGame::Time::Sleep(10);
    }

    network.Stop();
    IGame::Time::Sleep(1000000);
    
    return 0;
}

