#include "os/include/Container.h"
#include "os/include/StreamBufferSerializeImpls.h"
#include "os/include/TinyXmlSerializeImpls.h"
#include "os/include/ObjectSync.h"

using namespace IGame;

struct SubObject
{
    _UInt32 m_Id;
    std::string m_Name;
    IGAME_S11N_2(SubObject, _UInt32, m_Id, std::string, m_Name);
    SubObject() : m_Id(0), m_Name("") {}
};

typedef IGame::Map<_UInt32, std::string> MyMap;

struct Object
{
    _UInt32 m_Id;
    MyMap m_Map;
    SubObject m_SubObject;

    Object() : m_Id(0) {}

    IGAME_OBJECT_SYNC_3(m_Id, m_Map, m_SubObject);
    IGAME_S11N_3(Object, _UInt32, m_Id, MyMap, m_Map, SubObject, m_SubObject);

    void print() 
    {
        TiXmlElement obj("Object");
        TiXmlPrinter ptr;
        IGame::Serialize(*this, obj);
        obj.Accept(&ptr);
        printf("%s\n", ptr.CStr());
    }
};

int main()
{
    Object obj1, obj2;

    IGame::StreamBuffer buf;

    obj1.m_Id = 100;
    SET_OBJECT_SYNC(obj1, m_Id, buf);

    obj1.m_SubObject.m_Id = 1;
    obj1.m_SubObject.m_Name = "obj1";
    SET_OBJECT_SYNC(obj1, m_SubObject, buf);

    std::cout << "-------------obj1--------------" << std::endl;
    obj1.print();    
    std::cout << "-------------------------------" << std::endl;

    obj2.m_Id = 200;
    obj2.m_Map[1] = "my map";
    obj2.m_SubObject.m_Id = 2;
    obj2.m_SubObject.m_Name = "obj2";

    std::cout << "-------------obj2--------------" << std::endl;
    obj2.print();    
    std::cout << "-------------------------------" << std::endl;

    IGame::ObjectSync(obj2, buf);

    std::cout << "SET_OBJECT_SYNC(obj1, m_Id, buf)" << std::endl;
    std::cout << "SET_OBJECT_SYNC(obj1, m_SubObject, buf)" << std::endl;
    std::cout << "IGame::ObjectSync(obj2, buf)" << std::endl;

    std::cout << "-------------obj2--------------" << std::endl;
    obj2.print();    
    std::cout << "-------------------------------" << std::endl;
    return 0;
}


