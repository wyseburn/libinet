#include "listener.hxx"

#define CLIENT_FREELIST_SIZE 10000
#define ACCEPT_WATER_LOWER 200 

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
        : inet::listener(service, port, ip), service_(service), pending_accept_count_(0)
    {
        INET_FREELIST_INIT(&client_free_list_, CLIENT_FREELIST_SIZE); 
        INET_REGISTER_ACCEPTED(this, this, &client_manager::on_accepted);
    }

    ~client_manager() {}

    void accept()
    {
        //if (pending_accept_count_ > 50)
        //    return;

        //for (int i = 0; i < ACCEPT_WATER_LOWER; i++)
        {
            client* cli;
            if (!INET_FREELIST_EMPTY(&client_free_list_))
            {
                cli = (client *)INET_FREELIST_POP(&client_free_list_);
                assert(cli);
                cli->reset();
            }
            else
            {
                cli = new client(service_);
                assert(cli);
            }
            async_accept(cli);
        }
        //pending_accept_count_ += ACCEPT_WATER_LOWER;
    }
 
    void on_accepted(inet::session* session, inet::buffer& istream, inet::buffer& ostream)
    {
        //std::cout << "The connection establish with client" << std::endl;
        //pending_accept_count_--;
        this->accept();
        INET_REGISTER_RECEIVED(session, this, &client_manager::on_received);
        INET_REGISTER_CONNECT_BROKEN(session, this, &client_manager::on_disconnected);
    }

    void on_received(inet::session* session, inet::buffer& istream, inet::buffer& ostream)
    {
        //std::cout << "Receive a echo request from client." << std::endl;
        inet::buffer::node* node;
        while (node = istream.pop_node())
        {
            ostream.push_node(node);
        }
        session->async_send();
    }

    void on_disconnected(inet::session* session, inet::buffer& istream, inet::buffer& ostream)
    {
        //std::cout << "The connection was closed by peer" << std::endl;
        if (!INET_FREELIST_PUSH(&client_free_list_, session)) 
        {
            delete (client *)session;
        }
    }
private:
    inet::service& service_;
    INET_FREELIST_HEAD(client_free_list, CLIENT_FREELIST_SIZE) client_free_list_; 
    unsigned int pending_accept_count_;
};

int main()
{
    inet::service service;

    client_manager srv(service, 12345); 

    srv.accept();

    while (1)
    {
        try 
        {
            service.run();
        }
        catch (std::exception& e)
        {
            std::cout << "there a exception" << std::endl;
        }
    }

    return 0;
}

