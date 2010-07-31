#include "os/include/CsvReader.h"

using namespace IGame;

/**
* example.csv
* id,name,desc
* 1,"name1","desc1"
* 2,"name2","desc2"
*/

int main()
{
    std::string filePath = "example.csv";
    std::ifstream fileStream(filePath.c_str());
    IGame::CsvReader reader(fileStream, ",");
    reader.InitTitle();
    while(reader.ReadLine())   
    {
        _UInt32 id = 0;
        std::string name;
        std::string desc;
        reader.Bind("id", id);
        reader.Bind("name", name);
        reader.Bind("desc", desc);
        std::cout << id << "," << name << "," << desc << std::endl;
    }
    fileStream.close();
    return 0;
}



