#include "network/include/TcpListener.h"
#include "os/include/Logger.h"

class Client : public IGame::TcpConnection
{
public:
    Client(IGame::Service& service) : IGame::TcpConnection(service) 
    {
        IGAME_REGISTER_RECEIVED(this, this, &Client::OnReceived);
    }
    ~Client() {}

    void OnReceived(IGame::TcpConnection* conn, IGame::StreamBuffer& istream, IGame::StreamBuffer& ostream)
    {
        std::cout << "Receive a echo request from client." << std::endl;
        istream >> ostream;
        conn->AsyncSend();
    }
};

class Server : public IGame::TcpListener
{
public:
    Server(IGame::Service& service, unsigned short port, const char* ip = 0) 
        : IGame::TcpListener(service, port, ip)
    {
        IGAME_REGISTER_ACCEPTED(this, this, &Server::OnAccepted);
    }

    virtual ~Server() {}

    void Accept()
    {
        Client* cli = XNEW(Client)(GetService());
        AsyncAccept(cli);
    }
 
    void OnAccepted(IGame::TcpConnection* conn, IGame::StreamBuffer& istream, IGame::StreamBuffer& ostream)
    {
        std::cout << "The connection establish with client" << std::endl;
        std::cout << "From Ip: " << conn->GetFromIp().c_str() << " From Port: " << conn->GetFromPort() << std::endl;
       
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

