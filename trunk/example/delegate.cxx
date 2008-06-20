#include <cassert>
#include <iostream>
#include "delegate.hxx"

static void static_func(const char* str)
{
    std::cout << "Static function invoked by " << str << std::endl;
}

class object 
{
public:
    object(int n) : count_(n) {}
    
    void func(const char* str)
    {
        std::cout << "Member function invoked by " << str << std::endl;
        count_++;
    }

    int count_;
};

class functor
{
public:
    void operator () (const char* str)
    {
        std::cout << "Functor invoked by " << str << std::endl;
    }

    friend bool operator == (const functor& a, const functor& b)
    {
        return true;
    }
};

int main(int argc, char* argv[])
{
   // Create delegate a that references a static function:
   inet::delegate<void (const char *)> a;
   a += &static_func;
   assert(a.exist(&static_func));
   std::cout << "Invoking delegate a: ";
   a("a");
   std::cout << std::endl;

   // Create delegate b that references an instance function:
   object obj(0);
   inet::delegate<void (const char*)> b(&obj, &object::func);
   b += std::make_pair(&obj, &object::func);
   assert(b.exist(std::make_pair(&obj, &object::func)));

   std::cout << "Invoking delegate b: ";
   b("b");
   assert(obj.count_ == 1);
   std::cout << std::endl;

   // Create delegate c that references a function object:
   functor func; 
   inet::delegate<void (const char*)> c;
   c += func;

   std::cout << "Invoking delegate c: ";
   c("c");
   std::cout << std::endl;

   // Add an instance function and a functor to delegate a
   a += std::make_pair(&obj, &object::func);
   a += func;

   std::cout << "Invoking deleage a: ";
   a("a");
   std::cout << std::endl;

   // Remove the static function from delegate a
   a -= &static_func;

   std::cout << "Invoking deleage a: ";
   a("a");
   std::cout << std::endl;

   return 0;
}

