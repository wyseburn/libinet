#include "network/include/TcpConnection.h"
#include "os/include/Time.h"
#include "os/include/Logger.h"

class Client : public IGame::TcpConnection
{
public:
    Client(IGame::Service& service) : IGame::TcpConnection(service), m_EchoSeq(0)
    {
        IGAME_REGISTER_CONNECTED(this, this, &Client::OnConnected);
        IGAME_REGISTER_CONNECT_FAILED(this, this, &Client::OnConnectFailed);
        IGAME_REGISTER_CONNECT_BROKEN(this, this, &Client::OnConnectBroken);
        IGAME_REGISTER_RECEIVED(this, this, &Client::OnReceived);
    }

    ~Client() {}

    void TcpConnect(const char* ip, unsigned short port)
    {
        AsyncConnect(ip, port);
    }

    void OnConnected(IGame::TcpConnection*, IGame::StreamBuffer& istream, IGame::StreamBuffer& ostream)
    {
        std::cout << "Connect to server success" << std::endl;
        std::cout << "Remote Ip: " << GetFromIp().c_str() 
                  << " Remote Port: " << GetFromPort() << std::endl;
        ostream << m_EchoSeq; 
        ostream << "tcp echo test";
        AsyncSend();
    }

    void OnConnectFailed(IGame::TcpConnection*, IGame::StreamBuffer& istream, IGame::StreamBuffer& ostream)
    {
        std::cout << "Failed to connect server, errno: " << GetLastError() << std::endl;
    }

    void OnConnectBroken(IGame::TcpConnection*, IGame::StreamBuffer& istream, IGame::StreamBuffer& ostream)
    {
        std::cout << "Connect broken, errno: " << GetLastError() << std::endl;
    }

    void OnReceived(IGame::TcpConnection*, IGame::StreamBuffer& istream, IGame::StreamBuffer& ostream)
    {
        unsigned int id; 
        std::string str;

        istream >> id; 
        istream >> str;
        std::cout << id << " " << str.c_str() << std::endl;
        IGame::Time::Sleep(1000);
        m_EchoSeq++;

        ostream << m_EchoSeq;
        ostream << "tcp echo test";
        AsyncSend();
    }

private:
    unsigned int m_EchoSeq;
};

int main()
{
    LOG_INIT("", true, true);
    IGame::Service service;
    Client cli(service);
    cli.AsyncConnect("127.0.0.1", 12345);
    service.Run();
    return 0;
}

