#include "os/include/StreamBuffer.h"

void foo1()
{
    std::cout << "----------------foo1------------------" << std::endl;
    IGame::StreamBuffer buf(10);
    buf << (int) 100;
    buf << "test for stream buffer";

    std::cout << "buf len: " << buf.Length() << std::endl;

    int n;
    std::string str;

    buf >> n;
    buf >> str;

    std::cout << "buf len: " << buf.Length() << std::endl;
    std::cout << "n: " << n << " str: " << str.c_str() << std::endl;
}

void foo2()
{
    std::cout << "----------------foo2------------------" << std::endl;
    IGame::StreamBuffer buf(10);
    buf << (int) 100;
    buf << "test for stream buffer";

    std::cout << "buf len: " << buf.Length() << std::endl;

    buf.Drain(sizeof(int) + 5/*test */);

    std::cout << "execute buf.Drain(szieof(int) + 5) operation" << std::endl;
    std::cout << "buf len: " << buf.Length() << std::endl;

    std::string str;

    buf >> str;

    std::cout << "execute buf >> str operation" << std::endl;
    std::cout << "buf len: " << buf.Length() << std::endl;
    std::cout << "str: " << str.c_str() << std::endl;
}

void foo3()
{
    std::cout << "----------------foo3------------------" << std::endl;
    IGame::StreamBuffer buf(10);
    buf.Write("test for stream buffer", strlen("test for stream buffer"));

    std::cout << "buf len: " << buf.Length() << std::endl;

    char str[1024] = { 0 };
    int len = buf.Read(str, 5);

    std::cout << "execute buf.Read(str, 5) operation" << std::endl;
    std::cout << "Read len: " << len << " Read content: " << str << std::endl;
    std::cout << "buf len: " << buf.Length() << std::endl;

    len = buf.Read(str, 100);

    std::cout << "execute buf.Read(str, 100) operation" << std::endl;
    std::cout << "Read len: " << len << " Read content: " << str << std::endl;
    std::cout << "buf len: " << buf.Length() << std::endl;
}

void foo4()
{
    std::cout << "----------------foo4------------------" << std::endl;
    IGame::StreamBuffer buf(100);

    std::cout << "buf len: " << buf.Length() << std::endl;

    IGame::StreamBuffer::Node* node = buf.AllocNode();
    node->m_Len = strlen("test for stream buffer");
    memcpy((char *)node + sizeof(IGame::StreamBuffer::Node), "test for stream buffer", node->m_Len);

    buf.PushNode(node); 
    std::cout << "execute buf.push_node(node) operation" << std::endl;
    std::cout << "buf len: " << buf.Length() << std::endl;

    char str[1024] = { 0 };
    int len = buf.Read(str, 5);

    std::cout << "execute buf.Read(str, 5) operation" << std::endl;
    std::cout << "Read len: " << len << " Read content: " << str << std::endl;
    std::cout << "buf len: " << buf.Length() << std::endl;

    len = buf.Read(str, 100);

    std::cout << "execute buf.Read(str, 100) operation" << std::endl;
    std::cout << "Read len: " << len << " Read content: " << str << std::endl;
    std::cout << "buf len: " << buf.Length() << std::endl;
}

void foo5()
{
    std::cout << "----------------foo5------------------" << std::endl;
    IGame::StreamBuffer buf(15);

    buf << "test for stream buffer";

    buf.Print();
    std::cout << "buf len: " << buf.Length() << std::endl;

    IGame::StreamBuffer::Node* node = buf.PopNode();
   
    std::cout << "execute buf.pop_node() operation" << std::endl;
    std::cout << "node content: " << ((char *)node + sizeof (IGame::StreamBuffer::Node)) << std::endl;
    std::cout << "buf len: " << buf.Length() << std::endl;

    buf.DeallocNode(node);

    node = buf.PopNode();
   
    std::cout << "execute buf.pop_node() operation" << std::endl;
    std::cout << "node content: " << ((char *)node + sizeof (IGame::StreamBuffer::Node)) << std::endl;
    std::cout << "buf len: " << buf.Length() << std::endl;

    buf.DeallocNode(node);
}

void foo6()
{
    std::cout << "----------------foo6------------------" << std::endl;
    IGame::StreamBuffer buf1(10), buf2(5), buf3;

    buf1 << "test for stream buffer"; 
    buf2 << "foo";
    buf3 << "1234";

    std::cout << "buf1 len: " << buf1.Length() << std::endl;
    std::cout << "buf2 len: " << buf2.Length() << std::endl;
    std::cout << "buf3 len: " << buf3.Length() << std::endl;

    buf2 = buf1;

    std::cout << "execute buf2 = buf1 operation" << std::endl;
    std::cout << "buf1 len: " << buf1.Length() << std::endl;
    std::cout << "buf2 len: " << buf2.Length() << std::endl;
    std::cout << "buf3 len: " << buf3.Length() << std::endl;

    buf2.Drain(buf2.Length());

    std::cout << "execute buf1.Drain(buf2.Length()) operation" << std::endl;
    std::cout << "buf1 len: " << buf1.Length() << std::endl;
    std::cout << "buf2 len: " << buf2.Length() << std::endl;
    std::cout << "buf3 len: " << buf3.Length() << std::endl;

    buf2 << "abcd";

    std::cout << "execute buf2 << \"abcd\" operation" << std::endl;
    std::cout << "buf1 len: " << buf1.Length() << std::endl;
    std::cout << "buf2 len: " << buf2.Length() << std::endl;
    std::cout << "buf3 len: " << buf3.Length() << std::endl;

    buf1 >> buf2;

    std::cout << "execute buf1 >> buf2  operation" << std::endl;
    std::cout << "buf1 len: " << buf1.Length() << std::endl;
    std::cout << "buf2 len: " << buf2.Length() << std::endl;
    std::cout << "buf3 len: " << buf3.Length() << std::endl;

    buf2 += buf3;
    std::cout << "execute buf3 += buf3  operation" << std::endl;
    std::cout << "buf1 len: " << buf1.Length() << std::endl;
    std::cout << "buf2 len: " << buf2.Length() << std::endl;
    std::cout << "buf3 len: " << buf3.Length() << std::endl;
}

void foo7()
{
    std::cout << "----------------foo7------------------" << std::endl;
    IGame::StreamBuffer buf1(15), buf2(20);

    buf1 << "test for stream buffer";
    std::cout << "buf1 len: " << buf1.Length() << std::endl;
    std::cout << "buf2 len: " << buf2.Length() << std::endl;

    int len = buf1.Read(buf2, 4);
    std::cout << "execute buf1.Read(buf2, 4) operation" << std::endl;
    std::cout << "Read len: " << len << std::endl;
    std::cout << "buf1 len: " << buf1.Length() << std::endl;
    std::cout << "buf2 len: " << buf2.Length() << std::endl;

    len = buf1.Read(buf2, 100);
    std::cout << "execute buf1.Read(buf2, 100) operation" << std::endl;
    std::cout << "Read len: " << len << std::endl;
    std::cout << "buf1 len: " << buf1.Length() << std::endl;
    std::cout << "buf2 len: " << buf2.Length() << std::endl;
}

int main()
{
    foo1();
    foo2();
    foo3();
    foo4();
    foo5();
    foo6();
    foo7();
    return 0;
}
