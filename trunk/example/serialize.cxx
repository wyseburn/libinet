#include "serialize.hxx"

#include <map>
#include <set>
#include <vector>
#include <string>
#include <list>
#include <deque>

typedef std::pair <int, std::string> mypair;
typedef std::map<std::string, int> mymap;
typedef std::multimap<int, std::string> mymultimap;
typedef std::set<int> myset;
typedef std::multiset<std::string> mymultiset;

struct simple_object
{
    bool           a;
    char           b;
    wchar_t        c;
    short          d;
    unsigned short e;
    int            f;
    unsigned int   g;
    long           h;
    unsigned long  i;
    float          j;
    double         k;

    simple_object() : a(false)
                    , b(0)
                    , c(0)
                    , d(0)
                    , e(0)
                    , f(0)
                    , g(0)
                    , h(0)
                    , i(0)
                    , j(0)
                    , k(0)
    {
    }

    void print()
    {
        a ? std::cout << "a = true" : std::cout << "a = false";
        std::cout << std::endl;
        std::cout << "b = " << b << std::endl;
        std::wcout << "c = " << c << std::endl;
        std::cout << "d = " << d << std::endl;
        std::cout << "e = " << e << std::endl;
        std::cout << "f = " << f << std::endl;
        std::cout << "g = " << g << std::endl;
        std::cout << "h = " << h << std::endl;
        std::cout << "i = " << i << std::endl;
        std::cout << "j = " << j << std::endl;
        std::cout << "k = " << k << std::endl;
    }
};

INET_S11N_TRAITS_11(simple_object, 
                    bool, a, 
                    char, b, 
                    wchar_t, c, 
                    short, d,
                    unsigned short, e, 
                    int, f, 
                    unsigned int, g, 
                    long, h, 
                    unsigned long, i, 
                    float, j, 
                    double, k); 

struct complex_object
{
    std::string                   a;
    std::wstring                  b;
    std::vector<int>              c;
    std::list<std::string>        d;
    std::deque<std::vector<int> > e;
    mypair                        f;
    mymap                         g;
    mymultimap                    h;
    myset                         i;
    mymultiset                    j;
    inet::blob                    k;
    char                          buf[100];

    complex_object()
    {
        a.clear();
        b.clear();
        c.clear();
        d.clear();
        e.clear();
        g.clear();
        h.clear();
        i.clear();
        j.clear();
        k.data_ = buf;
        memset(buf, 0, 100);
    }

    void print()
    {
        std::cout << "a = " << a.c_str() << std::endl;
        std::wcout << "b = " << b.c_str() << std::endl;
        std::cout << "c = { ";
        for (std::vector<int>::iterator it = c.begin(); 
            it != c.end(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << "}" << std::endl;

        std::cout << "d = { ";
        for (std::list<std::string>::iterator it = d.begin();
            it != d.end(); ++it)
        {
            std::cout << "\"" << it->c_str() << "\" ";
        }
        std::cout << "}" << std::endl;

        std::cout << "e = { ";
        for (std::deque<std::vector<int> >::iterator it = e.begin();
            it != e.end(); ++it)
        {
            std::cout << "{ ";
            for (std::vector<int>::iterator jt = it->begin(); 
                jt != it->end(); ++jt)
            {
                std::cout << *jt << " ";
            }
            std::cout << "} ";
        }
        std::cout << "}" << std::endl;

        std::cout << "f = { ";
        std::cout << f.first << "=>" << f.second.c_str() << " }" << std::endl;

        std::cout << "g = { ";
        for (mymap::iterator it = g.begin(); it != g.end(); ++it)
        {
            std::cout << it->first.c_str() << "=>" << it->second << " ";
        }
        std::cout << "}" << std::endl;

        std::cout << "h = { ";
        for (mymultimap::iterator it = h.begin(); it != h.end(); ++it)
        {
            std::cout << it->first << "=>" << it->second.c_str() << " ";
        }
        std::cout << "}" << std::endl;

        std::cout << "i = { ";
        for (myset::iterator it = i.begin(); it != i.end(); ++it)
        {
            std::cout << *it << " ";
        }
        std::cout << "}" << std::endl;
  
        std::cout << "j = { ";
        for (mymultiset::iterator it = j.begin(); it != j.end(); ++it)
        {
            std::cout << it->c_str() << " ";
        }
        std::cout << "}" << std::endl;

        std::cout << "k = { ";
        std::cout << "len = " << k.len_ << " data = " << k.data_ << " }" << std::endl;
    }
};

INET_S11N_TRAITS_11(complex_object, 
                    std::string, a, 
                    std::wstring, b, 
                    std::vector<int>, c, 
                    std::list<std::string>, d, 
                    std::deque<std::vector<int> >, e, 
                    mypair, f, 
                    mymap, g, 
                    mymultimap, h, 
                    myset, i, 
                    mymultiset, j, 
                    inet::blob, k);


int main(int argc, char* argv[])
{
    inet::buffer buffer;
    simple_object simpleobj1, simpleobj2;
    complex_object complexobj1, complexobj2;

    simpleobj1.a = false;
    simpleobj1.b = 'b';
    simpleobj1.c = L'c';
    simpleobj1.d = -200;
    simpleobj1.e = 200;
    simpleobj1.f = -300;
    simpleobj1.g = 300;
    simpleobj1.h = -400;
    simpleobj1.i = 400;
    simpleobj1.j = 1234.5678;
    simpleobj1.k = 123456789.0987654321;

    std::cout << "-------------simple obj1 info--------------" << std::endl;
    simpleobj1.print();
    std::cout << "--------------------------------------------" << std::endl;

    std::cout << "execute operation inet::serialize(simpleobj1, buffer)" << std::endl;
    std::cout << "execute operation inet::unserialize(simpleobj2, buffer)" << std::endl;

    inet::serialize(simpleobj1, buffer);
    inet::unserialize(simpleobj2, buffer);

    std::cout << "-------------simple obj2 info--------------" << std::endl;
    simpleobj2.print();
    std::cout << "--------------------------------------------" << std::endl;

    std::vector<int> tmpvector;
    tmpvector.push_back(100);
    tmpvector.push_back(200);

    complexobj1.a = "test std::string serialize";
    complexobj1.b = L"test std::wstring serialize";
    complexobj1.c.push_back(0);
    complexobj1.c.push_back(1);
    complexobj1.c.push_back(2);
    complexobj1.d.push_back("test std::list serialize 0");
    complexobj1.d.push_back("test std::list serialize 1");
    complexobj1.d.push_back("test std::list serialize 2");
    complexobj1.e.push_back(tmpvector);
    complexobj1.e.push_back(complexobj1.c);
    complexobj1.f = std::make_pair(1000, "aaaa");
    complexobj1.g["aa"] = 100;
    complexobj1.g["bb"] = 200;
    complexobj1.g["cc"] = 300;
    complexobj1.h.insert(std::make_pair(1, "a"));
    complexobj1.h.insert(std::make_pair(2, "b"));
    complexobj1.h.insert(std::make_pair(1, "c"));
    complexobj1.i.insert(1234);
    complexobj1.i.insert(5678);
    complexobj1.j.insert("aaaa");
    complexobj1.j.insert("bbbb");
    complexobj1.j.insert("aaaa");
    std::string str = "test inet::blob object serialize";
    strncpy(complexobj1.buf, str.c_str(), str.length());
    complexobj1.k.len_ = str.length();

    std::cout << "-------------complex obj1 info--------------" << std::endl;
    complexobj1.print();
    std::cout << "--------------------------------------------" << std::endl;

    std::cout << "execute operation inet::serialize(simpleobj1, buffer)" << std::endl;
    std::cout << "execute operation inet::unserialize(simpleobj2, buffer)" << std::endl;

    inet::serialize(complexobj1, buffer);
    inet::unserialize(complexobj2, buffer);

    std::cout << "-------------complex obj2 info--------------" << std::endl;
    complexobj2.print();
    std::cout << "--------------------------------------------" << std::endl;

    exit(0);
}
