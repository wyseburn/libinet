#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include "serialize.hxx"

typedef std::pair <int, std::string> mypair;
typedef std::map<std::string, int> mymap;

struct object
{
     int a;
     std::string b;
     std::wstring c;
     char d;
     std::vector<int> e;
     mypair f;
     mymap g;
};

INET_S11N_TRAITS_6(object, int, a, std::string, b, std::wstring, c, char, d,
                   std::vector<int>, e, mypair, f);

int main(int argc, char* argv[])
{
    object obj, obj1;
    inet::buffer buf;
    
    obj.a = -10;
    obj.b = "test for object serialize";
    obj.c = L"对象序列化测试";
    obj.d = 'a';
    obj.e.push_back(1);
    obj.e.push_back(2);
    obj.e.push_back(3);
    obj.e.push_back(4);
    obj.e.push_back(5);
    obj.f = std::make_pair(1, "1111");
    obj.g["1"] = 100;
    obj.g["2"] = 200;


    inet::serialize(obj, buf);

    inet::unserialize(obj1, buf);

    printf("obj.a = %d, obj.b = %s\n", obj1.a, obj1.b.c_str());

    exit(0);
}
