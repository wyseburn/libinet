#include "timer.hxx"

static void func(void* args)
{
    std::cout << "Static timer callback" << std::endl;
}

class object
{
public:
    object() : count_(0) {}
    ~object() {}

    void func(void* args)
    {
        count_++;
        inet::timer* timer = (inet::timer *)args;
        if (count_ >= 5)
        {
            timer->cancel();
        }
        std::cout << "Member timer callback " << count_ << std::endl;
    }
private:
    int count_;
};
     
int main(int argc, char* argv[])
{
    inet::service service;
    inet::timer static_timer(service);
    inet::timer member_timer(service);
    object obj;

    static_timer.set(1000, inet::TIMER_EVENT_CB(&func), &static_timer);
    member_timer.set(2000, inet::TIMER_EVENT_CB(&obj, &object::func), &member_timer); 

    service.run();
    exit(0);
}


