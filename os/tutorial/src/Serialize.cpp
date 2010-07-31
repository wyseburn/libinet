#include "os/include/StreamBufferSerializeImpls.h"
#include "os/include/TinyXmlSerializeImpls.h"
#include "os/include/TinyJsonSerializeImpls.h"

#include <map>
#include <set>
#include <vector>
#include <string>
#include <list>
#include <deque>

//-----------------------------Simple object---------------------------------------------------


struct SimpleObject
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

    SimpleObject() : a(false)
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

    void PrintToXml()
    {
        TiXmlElement obj("SimpleObject");
        TiXmlPrinter ptr;
        IGame::Serialize(*this, obj);
        obj.Accept(&ptr);
        printf("%s\n", ptr.CStr());
    }

    void PrintToJson()
    {
        IGame::TiJsonNode tj1, tj2;
        std::string str1, str2;

        // serialize from c++ object to json node.
        IGame::Serialize(*this, tj1);
        // serialize from json node to string.
        tj1.ToString(str1);

        // unserialize from string to json node
        IGame::TinyJsonParser::Parse(tj2, str1);
        // unserialize from json node to string
        tj2.ToString(str2);

        SimpleObject assertObj;
        assert(IGame::Unserialize(assertObj, tj2));

        printf("%s\n", str2.c_str());
    }
};

IGAME_S11N_TRAITS_11(SimpleObject, 
                    bool,               a, 
                    char,               b, 
                    wchar_t,            c, 
                    short,              d,
                    unsigned short,     e, 
                    int,                f, 
                    unsigned int,       g, 
                    long,               h, 
                    unsigned long,      i, 
                    float,              j, 
                    double,             k); 

//-----------------------------Complex object---------------------------------------------------

// Define some test cell.

struct TestObj1
{
    std::string m_Name;
    int m_Age;
    IGAME_S11N_2(TestObj1, std::string, m_Name, int, m_Age);
};

struct TestObj2
{
    bool m_Flag;
    TestObj1 m_Obj1;
    IGAME_S11N_2(TestObj2, bool, m_Flag, TestObj1, m_Obj1);
};

// Define some custom data type for ',';

typedef std::pair <int, std::string> MyPair;  // NOTE: Key only is int or string
typedef std::map<std::string, std::list<TestObj2> > MyMap; // NOTE: Key only is int or string
typedef std::multimap<int, std::string> MyMultimap; // NOTE:: key only is int or string
typedef std::set<int> MySet;
typedef std::multiset<std::string> MyMultiset;
typedef IGame::Map<int, IGame::Map<int, TestObj1> > MyIGameMap;

struct ComplexObject
{
    std::string                   a;
    //std::wstring                b;  // no support wstring tempraily.
    std::vector<int>              c;
    std::list<std::string>        d;
    std::deque<std::vector<int> > e;
    MyPair                        f;
    MyMap                         g;
    MyMultimap                    h;
    MySet                         i;
    MyMultiset                    j;
    IGame::Blob                   k;
    MyIGameMap                    l;
    IGame::StreamBuffer           m;
    char                          n[100];
    IGame::List<std::string>      o;
    IGame::Set<std::string>       p;
    TestObj1                      q;
    TestObj2                      r;

    ComplexObject()
    {
        a.clear();
        //b.clear();
        c.clear();
        d.clear();
        e.clear();
        g.clear();
        h.clear();
        i.clear();
        j.clear();
        k.m_Data = n;
        l.Clear();
        m.Clear();
        memset(n, 0, 100);
        o.Clear();
        p.Clear();
    }

    void PrintToXml()
    {
        TiXmlElement obj("ComplexObject");
        TiXmlPrinter ptr;
        IGame::Serialize(*this, obj);
        obj.Accept(&ptr);
        printf("%s\n", ptr.CStr());
    }

    void PrintToJson()
    {
        IGame::TiJsonNode tj1, tj2;
        std::string str1, str2;

        // serialize from c++ object to json node.
        IGame::Serialize(*this, tj1);
        // serialize from json node to string.
        tj1.ToString(str1);

        // unserialize from string to json node
        IGame::TinyJsonParser::Parse(tj2, str1);
        // unserialize from json node to string
        tj2.ToString(str2);

        ComplexObject assertObj;
        assert(IGame::Unserialize(assertObj, tj2));

        printf("%s\n", str2.c_str());
    }
};

IGAME_S11N_TRAITS_16(ComplexObject, 
                     std::string,                  a,
                     //std::wstring,               b,
                     std::vector<int>,             c,
                     std::list<std::string>,       d,
                     std::deque<std::vector<int> >,e,
                     MyPair,                       f,
                     MyMap,                        g,
                     MyMultimap,                   h,
                     MySet,                        i,
                     MyMultiset,                   j,
                     IGame::Blob,                  k,
                     MyIGameMap,                   l,
                     IGame::StreamBuffer,          m,
                     IGame::List<std::string>,     o,
                     IGame::Set<std::string>,      p,
                     TestObj1,                     q,
                     TestObj2,                     r);

int
main(int argc, char* argv[])
{
    // Demo for simple object serialize.
    IGame::StreamBuffer buffer;
    SimpleObject simpleobj1, simpleobj2;

    simpleobj1.a = false;
    simpleobj1.b = 'b';
    simpleobj1.c = L'c';
    simpleobj1.d = -200;
    simpleobj1.e = 200;
    simpleobj1.f = -300;
    simpleobj1.g = 300;
    simpleobj1.h = -400;
    simpleobj1.i = 400;
    simpleobj1.j = -0.0000076;
    simpleobj1.k = 123456789.0987654321;

    std::cout << "-------------simple obj1 info--------------" << std::endl;
    simpleobj1.PrintToJson();
    std::cout << "--------------------------------------------" << std::endl;

    std::cout << "execute operation IGame::Serialize(simpleobj1, buffer)" << std::endl;
    std::cout << "execute operation IGame::Unserialize(simpleobj2, buffer)" << std::endl;

    IGame::Serialize(simpleobj1, buffer);
    IGame::Unserialize(simpleobj2, buffer);

    std::cout << "-------------simple obj2 info--------------" << std::endl;
    simpleobj2.PrintToJson();
    std::cout << "--------------------------------------------" << std::endl;

    // Demo for complex object serialize.
    buffer.Clear();

    ComplexObject complexobj1, complexobj2;

    complexobj1.a = "test for complex object serialize";

    complexobj1.c.push_back(100);
    complexobj1.c.push_back(200);

    complexobj1.d.push_back("test for list serialize 1");
    complexobj1.d.push_back("test for list serialize 2");
    complexobj1.d.push_back("test for list serialize 3");

    complexobj1.e.push_back(complexobj1.c);
    complexobj1.e.push_back(complexobj1.c);

    complexobj1.f = std::make_pair(1000, "aaaa");

    std::list<TestObj2> tempList;
    TestObj2 testobj2;
    testobj2.m_Flag = true;
    testobj2.m_Obj1.m_Name = "testobj2";
    testobj2.m_Obj1.m_Age = 100;
    tempList.push_back(testobj2);
    complexobj1.g["abcd"] = tempList;

    complexobj1.h.insert(std::make_pair(1, "a"));
    complexobj1.h.insert(std::make_pair(2, "b"));
    complexobj1.h.insert(std::make_pair(1, "c"));

    complexobj1.i.insert(1234);
    complexobj1.i.insert(5678);

    complexobj1.j.insert("aaaa");
    complexobj1.j.insert("bbbb");
    complexobj1.j.insert("aaaa");

    std::string str = "test IGame::Blob object serialize";
    strncpy(complexobj1.n, str.c_str(), str.length());
    complexobj1.k.m_Len = str.length();

    IGame::Map<int, TestObj1> testMap;
    TestObj1 testobj1;
    testobj1.m_Name = "testobj1";
    testobj1.m_Age = 400;
    testMap[1] = testobj1;
    complexobj1.l[9] = testMap;

    complexobj1.m << "test INet::Buffer object serialize";

    complexobj1.o.InsertTail("xxxx");
    complexobj1.o.InsertTail("yyyy");

    complexobj1.p.Insert("aaaa");
    complexobj1.p.Insert("bbbb");

    complexobj1.q = testobj1;
    
    complexobj1.r = testobj2;

    std::cout << "-------------complex obj1 info--------------" << std::endl;
    complexobj1.PrintToXml();
    std::cout << "--------------------------------------------" << std::endl;

    std::cout << "execute operation IGame::Serialize(simpleobj1, buffer)" << std::endl;
    std::cout << "execute operation IGame::Unserialize(simpleobj2, buffer)" << std::endl;

    IGame::Serialize(complexobj1, buffer, 123);
    IGame::Unserialize(complexobj2, buffer, 123);

    std::cout << "-------------complex obj2 info--------------" << std::endl;
    complexobj2.PrintToJson();
    std::cout << "--------------------------------------------" << std::endl;

    return 0;
}

