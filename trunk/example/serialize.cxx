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

    simple_object() : a(false), b(0), c(0), d(0), e(0), f(0), g(0), h(0), i(0), j(0), k(0)
    {
    }
};

INET_S11N_TRAITS_11(simple_object, bool, a, char, b, wchar_t, c, short, d,
                    unsigned short, e, int, f, unsigned int, g, long, h, 
                    unsigned long, i, float, j, double, k); 

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
};

INET_S11N_TRAITS_11(complex_object, std::string, a, std::wstring, b, 
                    std::vector<int>, c, std::list<std::string>, d, 
                    std::deque<std::vector<int> >, e, mypair, f, mymap, g, 
                    mymultimap, h, myset, i, mymultiset, j, inet::blob, k);


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

    inet::serialize(simpleobj1, buffer);
    inet::unserialize(simpleobj2, buffer);

    complexobj1.a = "test std::string serialize";
    complexobj1.b = L"test std::wstring serialize";
    complexobj1.c.push_back(0);
    complexobj1.c.push_back(1);
    complexobj1.c.push_back(2);
    complexobj1.d.push_back("test std::list serialize 0");
    complexobj1.d.push_back("test std::list serialize 1");
    complexobj1.d.push_back("test std::list serialize 2");
    complexobj1.e.push_back(complexobj1.c);
    complexobj1.f = std::make_pair(1000, "1000");
    complexobj1.g["100"] = 100;
    complexobj1.g["200"] = 200;
    complexobj1.g["300"] = 300;
    complexobj1.h.insert(std::make_pair(1, "1"));
    complexobj1.h.insert(std::make_pair(2, "2"));
    complexobj1.h.insert(std::make_pair(1, "3"));
    complexobj1.i.insert(1234);
    complexobj1.i.insert(5678);
    complexobj1.j.insert("aaaa");
    complexobj1.j.insert("bbbb");
    complexobj1.j.insert("aaaa");
    std::string str = "test inet::blob object serialize";
    strncpy(complexobj1.buf, str.c_str(), str.length());
    complexobj1.k.len_ = str.length();

    inet::serialize(complexobj1, buffer);
    inet::unserialize(complexobj2, buffer);

    exit(0);
}
