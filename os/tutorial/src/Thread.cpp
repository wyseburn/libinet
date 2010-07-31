#include "os/include/Thread.h"
#include "os/include/Time.h"

using namespace IGame;

class UserThread : public Thread
{
public:
    UserThread() {}
    virtual ~UserThread() {}

private:
    virtual void ThreadFunc()
    {
        while (!IsShutdown())
        {
            IGame::Time::Sleep(1000);
            printf("test for user thread...\n");
        }
    }
};

int main()
{
    UserThread userThread;
    userThread.Start();

    IGame::Time::Sleep(1000 * 20);

    userThread.Stop();
    userThread.Join();
    return 0;
}


