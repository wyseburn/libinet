#include <iostream>
#include "buffer.hxx"

void foo1()
{
    std::cout << "----------------foo1------------------" << std::endl;
    inet::buffer buf(10);
    buf << (int) 100;
    buf << "test for stream buffer";

    std::cout << "buf len: " << buf.length() << std::endl;

    int n;
    std::string str;

    buf >> n;
    buf >> str;

    std::cout << "buf len: " << buf.length() << std::endl;
    std::cout << "n: " << n << " str: " << str.c_str() << std::endl;
}

void foo2()
{
    std::cout << "----------------foo2------------------" << std::endl;
    inet::buffer buf(10);
    buf << (int) 100;
    buf << "test for stream buffer";

    std::cout << "buf len: " << buf.length() << std::endl;

    buf.drain(sizeof(int) + 5/*test */);

    std::cout << "execute buf.drain(szieof(int) + 5) operation" << std::endl;
    std::cout << "buf len: " << buf.length() << std::endl;

    std::string str;

    buf >> str;

    std::cout << "execute buf >> str operation" << std::endl;
    std::cout << "buf len: " << buf.length() << std::endl;
    std::cout << "str: " << str.c_str() << std::endl;
}

void foo3()
{
    std::cout << "----------------foo3------------------" << std::endl;
    inet::buffer buf(10);
    buf.write("test for stream buffer", strlen("test for stream buffer"));

    std::cout << "buf len: " << buf.length() << std::endl;

    char str[1024] = { 0 };
    int len = buf.read(str, 5);

    std::cout << "execute buf.read(str, 5) operation" << std::endl;
    std::cout << "read len: " << len << " read content: " << str << std::endl;
    std::cout << "buf len: " << buf.length() << std::endl;

    len = buf.read(str, 100);

    std::cout << "execute buf.read(str, 100) operation" << std::endl;
    std::cout << "read len: " << len << " read content: " << str << std::endl;
    std::cout << "buf len: " << buf.length() << std::endl;
}

void foo4()
{
    std::cout << "----------------foo4------------------" << std::endl;
    inet::buffer buf(100);

    std::cout << "buf len: " << buf.length() << std::endl;

    inet::buffer::node* node = buf.alloc_node();
    node->len_ = strlen("test for stream buffer");
    memcpy((char *)node + sizeof(inet::buffer::node), "test for stream buffer", node->len_);

    buf.push_node(node); 
   
    std::cout << "execute buf.push_node(node) operation" << std::endl;
    std::cout << "buf len: " << buf.length() << std::endl;

    char str[1024] = { 0 };
    int len = buf.read(str, 5);

    std::cout << "execute buf.read(str, 5) operation" << std::endl;
    std::cout << "read len: " << len << " read content: " << str << std::endl;
    std::cout << "buf len: " << buf.length() << std::endl;

    len = buf.read(str, 100);

    std::cout << "execute buf.read(str, 100) operation" << std::endl;
    std::cout << "read len: " << len << " read content: " << str << std::endl;
    std::cout << "buf len: " << buf.length() << std::endl;
}

void foo5()
{
    std::cout << "----------------foo5------------------" << std::endl;
    inet::buffer buf(15);

    buf << "test for stream buffer";

    std::cout << "buf len: " << buf.length() << std::endl;

    inet::buffer::node* node = buf.pop_node();
   
    std::cout << "execute buf.pop_node() operation" << std::endl;
    std::cout << "node content: " << ((char *)node + sizeof (inet::buffer::node)) << std::endl;
    std::cout << "buf len: " << buf.length() << std::endl;

    buf.dealloc_node(node);

    
    node = buf.pop_node();
   
    std::cout << "execute buf.pop_node() operation" << std::endl;
    std::cout << "node content: " << ((char *)node + sizeof (inet::buffer::node)) << std::endl;
    std::cout << "buf len: " << buf.length() << std::endl;

    buf.dealloc_node(node);
}

void foo6()
{
    std::cout << "----------------foo6------------------" << std::endl;
    inet::buffer buf1(10), buf2(5), buf3;

    buf1 << "test for stream buffer"; 
    buf2 << "foo";
    buf3 << "1234";

    std::cout << "buf1 len: " << buf1.length() << std::endl;
    std::cout << "buf2 len: " << buf2.length() << std::endl;
    std::cout << "buf3 len: " << buf3.length() << std::endl;

    buf2 = buf1;

    std::cout << "execute buf2 = buf1 operation" << std::endl;
    std::cout << "buf1 len: " << buf1.length() << std::endl;
    std::cout << "buf2 len: " << buf2.length() << std::endl;
    std::cout << "buf3 len: " << buf3.length() << std::endl;

    buf2.drain(buf2.length());

    std::cout << "execute buf1.drain(buf2.length()) operation" << std::endl;
    std::cout << "buf1 len: " << buf1.length() << std::endl;
    std::cout << "buf2 len: " << buf2.length() << std::endl;
    std::cout << "buf3 len: " << buf3.length() << std::endl;

    buf2 << "abcd";

    std::cout << "execute buf2 << \"abcd\" operation" << std::endl;
    std::cout << "buf1 len: " << buf1.length() << std::endl;
    std::cout << "buf2 len: " << buf2.length() << std::endl;
    std::cout << "buf3 len: " << buf3.length() << std::endl;

    buf1 >> buf2;

    std::cout << "execute buf1 >> buf2  operation" << std::endl;
    std::cout << "buf1 len: " << buf1.length() << std::endl;
    std::cout << "buf2 len: " << buf2.length() << std::endl;
    std::cout << "buf3 len: " << buf3.length() << std::endl;

    buf2 += buf3;
    std::cout << "execute buf3 += buf3  operation" << std::endl;
    std::cout << "buf1 len: " << buf1.length() << std::endl;
    std::cout << "buf2 len: " << buf2.length() << std::endl;
    std::cout << "buf3 len: " << buf3.length() << std::endl;
}

void foo7()
{
    std::cout << "----------------foo7------------------" << std::endl;
    inet::buffer buf1(15), buf2(20);

    buf1 << "test for stream buffer";
    std::cout << "buf1 len: " << buf1.length() << std::endl;
    std::cout << "buf2 len: " << buf2.length() << std::endl;

    int len = buf1.read(buf2, 4);
    std::cout << "execute buf1.read(buf2, 4) operation" << std::endl;
    std::cout << "read len: " << len << std::endl;
    std::cout << "buf1 len: " << buf1.length() << std::endl;
    std::cout << "buf2 len: " << buf2.length() << std::endl;

    len = buf1.read(buf2, 100);
    std::cout << "execute buf1.read(buf2, 100) operation" << std::endl;
    std::cout << "read len: " << len << std::endl;
    std::cout << "buf1 len: " << buf1.length() << std::endl;
    std::cout << "buf2 len: " << buf2.length() << std::endl;
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

    exit(0);
}

