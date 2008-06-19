#include <iostream>
#include "serialize.hxx"

struct object
{
     int a;
     char b;
     std::string c;   
};

INET_S11N_TRAITS_3(object, int, a, char, b, std::string, c);

int main(int argc, char* argv[])
{
    object obj, obj1;
    inet::buffer buf;
    
    obj.a = -10;
    obj.b = 'a';
    obj.c = "test for object serialize";

    inet::serialize(obj, buf);

    inet::unserialize(obj1, buf);

    printf("obj.a = %d, obj.b = %c, obj.c = %s\n", obj.a, obj.b, obj.c.c_str());

    exit(0);
}
