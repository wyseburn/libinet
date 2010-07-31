#include <cassert>
#include <iostream>
#include "os/include/Delegate.h"

static void StaticFunc(const char* str)
{
    std::cout << "Static Function invoked by " << str << std::endl;
}

class Object 
{
public:
    Object(int n) : m_Count(n) {}
    
    void Func(const char* str)
    {
        std::cout << "Member Function invoked by " << str << std::endl;
        m_Count++;
    }

    int m_Count;
};

class Functor
{
public:
    void operator () (const char* str)
    {
        std::cout << "Functor invoked by " << str << std::endl;
    }

    friend bool operator == (const Functor& a, const Functor& b)
    {
        return true;
    }
};

int main(int argc, char* argv[])
{
   // Create delegate a that references a static Function:
   IGame::Delegate<void (const char *)> a;
   a += &StaticFunc;
   assert(a.Exist(&StaticFunc));
   std::cout << "Invoking delegate a: ";
   a("a");
   std::cout << std::endl;

   a -= &StaticFunc;
   assert(!a.Exist(&StaticFunc));

   // Create delegate b that references an instance Function:
   Object obj(0);
   IGame::Delegate<void (const char*)> b(&obj, &Object::Func);
   b += std::make_pair(&obj, &Object::Func);
   assert(b.Exist(std::make_pair(&obj, &Object::Func)));

   std::cout << "Invoking delegate b: ";
   b("b");
   assert(obj.m_Count == 1);
   std::cout << std::endl;

   // Create delegate c that references a Function Object:
   Functor Func; 
   IGame::Delegate<void (const char*)> c;
   c += Func;

   std::cout << "Invoking delegate c: ";
   c("c");
   std::cout << std::endl;

   // Add an instance Function and a Functor to delegate a
   a += std::make_pair(&obj, &Object::Func);
   a += Func;

   std::cout << "Invoking deleage a: ";
   a("a");
   std::cout << std::endl;

   // Remove the static Function from delegate a
   a -= &StaticFunc;

   std::cout << "Invoking deleage a: ";
   a("a");
   std::cout << std::endl;

   return 0;
}

