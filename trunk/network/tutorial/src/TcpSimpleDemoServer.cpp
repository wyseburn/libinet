#include "network/include/MsgHandler.h"
#include "network/include/TcpListener.h"
#include "os/include/Time.h"
#include "MsgDefinition.h"

class Client : public IGame::TcpConnection
{
public:
    Client(IGame::Service& service) : IGame::TcpConnection(service) 
    {
        m_UserPointer = &m_MsgHdr;
        m_MsgHandler.Initialize(this);
        m_MsgHandler.RegisterHandler<Client, DemoReq>(XNEW(IGame::Delegate<void (Client&, const DemoReq&)>)(this, &Client::OnDemoReq));
    }

    ~Client() {}

    void OnDemoReq(Client& conn, const DemoReq& data)
    {
        std::cout << data.m_Data.m_Content.c_str() << " " << data.m_Seq << std::endl;


        //IGame::Time::Sleep(100);
        DemoResp resp;
        resp.m_Ok = true;
        resp.m_Seq = data.m_Seq + 1;
        m_MsgHandler.SendMsg(conn, resp);
    }

private:
    IGame::MsgHandler m_MsgHandler;
    IGame::MsgHdr m_MsgHdr;
};

class Server : public IGame::TcpListener
{
public:
    Server(IGame::Service& service, unsigned short port, const char* ip = 0) 
        : IGame::TcpListener(service, port, ip)
    {
        IGAME_REGISTER_ACCEPTED(this, this, &Server::OnAccepted);
    }

    ~Server() {}

    void Accept()
    {
        Client* cli = XNEW(Client)(GetService());
        AsyncAccept(cli);
    }
 
    void OnAccepted(IGame::TcpConnection* conn, IGame::StreamBuffer& istream, IGame::StreamBuffer& ostream)
    {
        std::cout << "The connection establish with client" << std::endl;
        std::cout << "From Ip: " << conn->GetFromIp().c_str() 
                  << " From Port: " << conn->GetFromPort() << std::endl;
        Accept();
        IGAME_REGISTER_CONNECT_BROKEN(conn, this, &Server::OnDisconnected);
    }

    void OnDisconnected(IGame::TcpConnection* conn, IGame::StreamBuffer& istream, IGame::StreamBuffer& ostream)
    {
        std::cout << "The connection was closed by peer, errno: " << GetLastError() << std::endl;
        SAFE_DELETE(conn);
    }
};

int main()
{
    LOG_INIT("", true, true);
    IGame::Service service;
    Server server(service, 12345); 
    server.Accept();
    service.Run();

    return 0;
}

