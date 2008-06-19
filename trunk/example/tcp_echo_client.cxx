#include <iostream>
#include "session.hxx"

#if defined (_WIN32)
# include <winsock2.h>
#endif

class client : public inet::session
{
public:
    client(inet::service& service) : inet::session(service, inet::tcp), echoseq_(0)
    {
        INET_REGISTER_CONNECTED(this, this, &client::on_connected);
        INET_REGISTER_CONNECT_FAILED(this, this, &client::on_connect_failed);
        INET_REGISTER_CONNECT_BROKEN(this, this, &client::on_connect_broken);
        INET_REGISTER_RECEIVED(this, this, &client::on_received);
    }

    ~client() {}

    void connect(const char* ip, unsigned short port)
    {
        async_connect(ip, port);
    }

    void on_connected(inet::session*, inet::buffer& istream, inet::buffer& ostream)
    {
        std::cout << "Connect to server success" << std::endl;
        ostream << echoseq_; 
        ostream << "tcp echo test";
        async_send();
    }

    void on_connect_failed(inet::session*, inet::buffer& istream, inet::buffer& ostream)
    {
        std::cout << "Failed to connect server" << std::endl;
    }

    void on_connect_broken(inet::session*, inet::buffer& istream, inet::buffer& ostream)
    {
        std::cout << "Connect broken" << std::endl;
    }

    void on_received(inet::session*, inet::buffer& istream, inet::buffer& ostream)
    {
        unsigned int id; 
        std::string str;
     
        istream >> id; 
        istream >> str;
        std::cout << id << " " << str.c_str() << std::endl;
#if defined (_WIN32)
        Sleep(1000);
#elif defined (__linux__) || defined (__FreeBSD__)
        usleep(1000);
#endif
        echoseq_++;

        ostream << echoseq_;
        ostream << "tcp echo test";
        async_send();
    }

private:
    unsigned int echoseq_;
};

int main()
{
    inet::service service;

    client cli(service);

    cli.connect("127.0.0.1", 12345);

    service.run();

    return 0;
}

