#include "network/include/MsgHandler.h"
#include "network/include/Timer.h"
#include "os/include/Time.h"
#include "MsgDefinition.h"

class Client : public IGame::TcpConnection
{
public:
    Client(IGame::Service& service) : IGame::TcpConnection(service)
    {
        m_UserPointer = &m_MsgHdr;
        IGAME_REGISTER_CONNECTED(this, this, &Client::OnConnected);
        IGAME_REGISTER_CONNECT_FAILED(this, this, &Client::OnConnectFailed);
        IGAME_REGISTER_CONNECT_BROKEN(this, this, &Client::OnConnectBroken);
        m_MsgHandler.Initialize(this);
        m_MsgHandler.RegisterHandler<Client, DemoResp>(XNEW(IGame::Delegate<void (Client&, const DemoResp&)>)(this, &Client::OnDemoResp));
    }

    ~Client() {}

    void Connect(const char* ip, unsigned short port)
    {
        AsyncConnect(ip, port);
    }

    void OnConnected(IGame::TcpConnection* conn, IGame::StreamBuffer& istream, IGame::StreamBuffer& ostream)
    {
        std::cout << "Connect to server success" << std::endl;
        std::cout << "Remote Ip: " << GetFromIp().c_str() << " Remote Port: " << GetFromPort() << std::endl;

        DemoReq req;
        req.m_Seq = 1;
        req.m_Data.m_Content = "Test Message";
        m_MsgHandler.SendMsg(*conn, req);
        std::cout << "Send demo req" << " " << req.m_Seq << std::endl;

        m_SendTime = IGame::Time::GetTimeMilliSec();
    }

    void OnConnectFailed(IGame::TcpConnection*, IGame::StreamBuffer& istream, IGame::StreamBuffer& ostream)
    {
        std::cout << "Failed to connect server, errno: " << GetLastError() << std::endl;
    }

    void OnConnectBroken(IGame::TcpConnection*, IGame::StreamBuffer& istream, IGame::StreamBuffer& ostream)
    {
        std::cout << "Connect broken, errno: " << GetLastError() << std::endl;
    }

    void OnDemoResp(Client& conn, const DemoResp& data)
    {
        
        _UInt64 currTime = IGame::Time::GetTimeMilliSec();

        std::cout << "Received demo resp" << " " << data.m_Seq << " rtt: " << currTime - m_SendTime << std::endl;

        DemoReq req;
        req.m_Seq = data.m_Seq + 1;
        req.m_Data.m_Content = "Test Message";
        
        m_MsgHandler.SendMsg(conn, req);

        std::cout << "Send demo req" << " " << req.m_Seq << std::endl;
        m_SendTime = IGame::Time::GetTimeMilliSec();
    }

private:
    IGame::MsgHandler m_MsgHandler; 
    IGame::MsgHdr m_MsgHdr;
    _UInt64 m_SendTime;
};

int main()
{
    LOG_INIT("", true, true);
    IGame::Service service;
    Client cli(service);
    cli.Connect("127.0.0.1", 12345);
    service.Run();
    return 0;
}

