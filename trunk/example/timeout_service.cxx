#include "timeout_service.hxx"

#if defined (_WIN32)
#include <winsock2.h>
#endif

static void func(void* args)
{
    std::cout << "Static timer callback" << std::endl;
}

class object
{
public:
    void func(void* args)
    {
        std::cout << "Member timer callback" << std::endl;
    }
};
     
int main(int argc, char* argv[])
{
    inet::timeout_service<1000*1000, 1000> service;
    object obj;

    for (int i = 0; i < 20; ++i)
        service.regist(1000 * i, inet::TIMEOUT_EVENT_CB(&obj, &object::func), NULL);

 
    for (int i = 0; i < 20; ++i)
        service.regist(1000 * i, inet::TIMEOUT_EVENT_CB(&func), NULL);

    while (1)
    {
#if defined (_WIN32)
        Sleep(500);
#elif defined (__linux__) || defined (__FreeBSD__)
        usleep(500);
#endif
        service.update();
    }

    exit(0);
}

