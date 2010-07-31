#include "os/include/Fifo.h"
#include "os/include/Thread.h"

using namespace IGame;

struct TestData
{
    _Int32 m_Id;
    std::string m_Name;
    TestData() : m_Id(0), m_Name("") {}
    bool operator == (const TestData& rhs)
    {
        return m_Id == rhs.m_Id;
    }
};

static TestData s_NullData;
static IGame::Fifo<TestData> s_Fifo(s_NullData);

class Producer : public Thread
{
public:
    Producer() : m_Index(1) {}
    virtual ~Producer() {}

private:
    virtual void ThreadFunc()
    {
        while (!IsShutdown())
        {
            IGame::Time::Sleep(50);
            TestData data;
            data.m_Id = m_Index++;
            s_Fifo.Push(data);
        }
    }
    _UInt32 m_Index;
};

class Consumer : public Thread
{
public:
    Consumer() {}
    virtual ~Consumer() {}

private:
    virtual void ThreadFunc()
    {
        while (!IsShutdown())
        {
            TestData data = s_Fifo.GetNext(50);
            if (data == s_NullData)
            {
                continue;
            }

            printf("%d\n", data.m_Id);
        }
    }
};


int main()
{
    Producer p;
    Consumer c;

    p.Start();
    c.Start();

    IGame::Time::Sleep(1000 * 20);

    p.Stop();
    c.Stop();
    p.Join();
    c.Join();

    s_Fifo.Clear();  // explicit clear s_Fifo for static.
    return 0;
}


