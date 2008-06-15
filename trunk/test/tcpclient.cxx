#include <iostream>
#include "session.hxx"
#include "msg_handler.hxx"
#include "msg.hxx"

class client : public inet::session
{
public:
    client(inet::service& service) : inet::session(service, inet::tcp), echoseq_(0)
    {
        INET_REGISTER_CONNECTED(this, this, &client::on_connected);
        INET_REGISTER_CONNECT_FAILED(this, this, &client::on_connect_failed);
        INET_REGISTER_CONNECT_BROKEN(this, this, &client::on_connect_broken);

        msg_handler_.init(this);
        INET_REGISTER_MSG(msg_handler_, echo_req, this, &client::on_echo_req);
    }

    ~client() {}

    void connect(const char* ip, unsigned short port)
    {
        async_connect(ip, port);
    }

    void on_connected(inet::session*, inet::buffer&, inet::buffer&)
    {
        std::cout << "connect to server success" << std::endl;

        echo_req req;
        req.id_ = echoseq_;
        req.msg_ = "test tcp msg echo"; 
        msg_handler_.send_msg(req);
    }

    void on_connect_failed(inet::session*, inet::buffer&, inet::buffer&)
    {
        std::cout << "failed to connect server" << std::endl;
    }

    void on_connect_broken(inet::session*, inet::buffer&, inet::buffer&)
    {
        std::cout << "connect broken" << std::endl;
    }

    bool on_echo_req(const echo_req& req)
    {
        std::cout << "recevied msg id " << echoseq_ << " content " << req.msg_ << std::endl;
        echoseq_++;
        sleep(1);
        msg_handler_.send_msg(req);
    }

private:
    unsigned int echoseq_;
    inet::msg_handler<1000> msg_handler_; 
};

int main()
{
    inet::service service;

    client cli(service);

    cli.connect("127.0.0.1", 12345);

    service.run();

    return 0;
}

