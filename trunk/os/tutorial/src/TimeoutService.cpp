#include "os/include/TimeoutService.h"
#include "os/include/Time.h"

void* timerHandler;

static bool Func(void* args)
{
    std::cout << "Static timer callback" << std::endl;
    IGame::TimeoutService<1000*1000, 1000>* service = (IGame::TimeoutService<1000*1000, 1000>*) args;
    service->Cancel(timerHandler);
    return false;
}

class Object
{
public:
    bool Func(void* args)
    {
        std::cout << "Member timer callback" << std::endl;
        return true;
    }
};
     
int main(int argc, char* argv[])
{
    IGame::TimeoutService<1000*1000, 1000> service;
    Object obj;

    timerHandler = service.Register(1000, IGame::TimeoutEvent(&obj, &Object::Func), NULL);
    assert(timerHandler);
    service.Register(5000, IGame::TimeoutEvent(&Func), &service);

    while (1)
    {
        IGame::Time::Sleep(500);
        service.Update();
    }

    exit(0);
}



