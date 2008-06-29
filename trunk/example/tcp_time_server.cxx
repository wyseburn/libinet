#include "listener.hxx"
#include "msg_handler.hxx"
#include "msg.hxx"

#if defined (_WIN32)
#include <time.h>
#endif

class client : public inet::session
{
public:
    client(inet::service& service) : inet::session(service, inet::tcp) 
    {
        server_id_ = 1234;
        msg_handler_.init(this);
        INET_REGISTER_MSG(&msg_handler_, get_time_req, this, &client::on_get_time_req);
    }

    ~client() {}

    bool on_get_time_req(const get_time_req& req)
    {
        std::cout << "Received a get current time request from user " << req.username_.c_str() << std::endl;
        time_t t;
        time(&t);
        char buffer[64] = { 0 };
        strftime(buffer, 64, "%H:%M:%S", localtime(&t));
        
        get_time_res res;
        res.server_id_ = server_id_;
        res.time_ = buffer;
        msg_handler_.send_msg(res);
        return true;
    }

private:
    unsigned int server_id_;
    inet::msg_handler<10> msg_handler_;
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
        std::cout << "From Ip: " << session->get_remote_ip().c_str() 
                  << " From Port: " << session->get_remote_port() << std::endl;
        accept();
        INET_REGISTER_CONNECT_BROKEN(session, this, &client_manager::on_disconnected);
    }

    void on_disconnected(inet::session* session, inet::buffer& istream, inet::buffer& ostream)
    {
        std::cout << "The connection was closed by peer, errno: " << get_last_error() << std::endl;
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

