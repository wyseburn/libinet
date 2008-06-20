#include "listener.hxx"

class client : public inet::session
{
public:
    client(inet::service& service) : inet::session(service, inet::tcp) {}
    ~client() {}
};

class client_manager : public inet::listener
{
public:
    client_manager(inet::service& service, unsigned short port, const char* ip = 0) 
        : inet::listener(service, port, ip), service_(service)
    {
        INET_REGISTER_ACCEPTED(this, this, &client_manager::on_accepted);
    }

    ~client_manager() {}

    void accept()
    {
        client* cli = new client(service_);
        async_accept(cli);
    }
 
    void on_accepted(inet::session* session, inet::buffer& istream, inet::buffer& ostream)
    {
        std::cout << "The connection establish with client" << std::endl;
        accept();
        INET_REGISTER_RECEIVED(session, this, &client_manager::on_received);
        INET_REGISTER_CONNECT_BROKEN(session, this, &client_manager::on_disconnected);
    }

    void on_received(inet::session* session, inet::buffer& istream, inet::buffer& ostream)
    {
        std::cout << "Receive a echo request from client." << std::endl;
        istream >> ostream;
        session->async_send();
    }

    void on_disconnected(inet::session* session, inet::buffer& istream, inet::buffer& ostream)
    {
        std::cout << "The connection was closed by peer" << std::endl;
        delete session;
    }
private:
    inet::service& service_;
};

int main()
{
    inet::service service;

    client_manager srv(service, 12345); 

    srv.accept();

    service.run();

    return 0;
}

