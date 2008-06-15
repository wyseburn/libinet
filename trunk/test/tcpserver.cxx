#include <iostream>
#include "listener.hxx"
#include "msg_handler.hxx"
#include "msg.hxx"

class client : public inet::session
{
public:
    client(inet::service& service) : inet::session(service, inet::tcp)
    {
        msg_handler_.init(this);
        INET_REGISTER_MSG(msg_handler_, echo_req, this, &client::on_echo_req);
    }

    ~client() {}

    bool on_echo_req(const echo_req& req)
    {
        msg_handler_.send_msg(req);
    }

private:
    inet::msg_handler<1000> msg_handler_;
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
        accept();
        INET_REGISTER_CONNECT_BROKEN(session, this, &client_manager::on_disconnected);
    }

    void on_disconnected(inet::session* session, inet::buffer& istream, inet::buffer& ostream)
    {
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

