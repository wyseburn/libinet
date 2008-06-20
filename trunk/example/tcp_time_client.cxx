#include "msg_handler.hxx"
#include "msg.hxx"

class client : public inet::session
{
public:
    client(inet::service& service) : inet::session(service, inet::tcp)
    {
        INET_REGISTER_CONNECTED(this, this, &client::on_connected);
        INET_REGISTER_CONNECT_FAILED(this, this, &client::on_connect_failed);
        INET_REGISTER_CONNECT_BROKEN(this, this, &client::on_connect_broken);

        msg_handler_.init(this);
        INET_REGISTER_MSG(&msg_handler_, get_time_res, this, &client::on_get_time_res);
    }

    ~client() {}

    void connect(const char* ip, unsigned short port)
    {
        async_connect(ip, port);
    }

    void on_connected(inet::session*, inet::buffer& istream, inet::buffer& ostream)
    {
        std::cout << "Connect to server success" << std::endl;
        get_time_req req;
        req.username_ = "tcp_time_client";
        msg_handler_.send_msg(req);
    }

    void on_connect_failed(inet::session*, inet::buffer& istream, inet::buffer& ostream)
    {
        std::cout << "Failed to connect server" << std::endl;
    }

    void on_connect_broken(inet::session*, inet::buffer& istream, inet::buffer& ostream)
    {
        std::cout << "Connect broken" << std::endl;
    }

    bool on_get_time_res(const get_time_res& res)
    {
        std::cout << "Current time " << res.time_.c_str() << " from server " << res.server_id_ << std::endl;
        return true;
    }

private:
    inet::msg_handler<10> msg_handler_; 
};

int main()
{
    inet::service service;

    client cli(service);

    cli.connect("127.0.0.1", 12345);

    service.run();

    return 0;
}

