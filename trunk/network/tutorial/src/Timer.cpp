#include "network/include/Timer.h"
#include "os/include/Logger.h"

static void Func(void* args)
{
    std::cout << "Static timer callback" << std::endl;
}

class Object
{
public:
    Object() : m_Count(0) {}
    ~Object() {}

    void Func(void* args)
    {
        m_Count++;
        IGame::Timer* timer = (IGame::Timer *)args;
        if (m_Count >= 5)
        {
            timer->Cancel();
        }
        std::cout << "Member timer callback " << m_Count << std::endl;
    }
private:
    int m_Count;
};
     
int main(int argc, char* argv[])
{
    LOG_INIT("", true, true);
    IGame::Service service;
    IGame::Timer staticTimer(service);
    IGame::Timer memberTimer(service);
    Object obj;

    staticTimer.Set(1000, IGame::TimerEvent(&Func), &staticTimer);
    memberTimer.Set(2000, IGame::TimerEvent(&obj, &Object::Func), &memberTimer); 

    service.Run();
    exit(0);
}


