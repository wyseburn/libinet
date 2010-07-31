
#include "os/include/TinyJsonParser.h"

using namespace IGame;

int main()
{
    TiJsonNode tj1, tj2;

    TiJsonNode* subTj = tj1.LinkChildToTail(IGAME_JSON_TYPE_OBJECT, "member1", "");
    tj1.LinkChildToTail(IGAME_JSON_TYPE_INTEGER, "member2", "200");

    subTj->LinkChildToTail(IGAME_JSON_TYPE_STRING, "submember1", "sub1");
    subTj->LinkChildToTail(IGAME_JSON_TYPE_STRING, "submember2", "sub2");

    TiJsonNode* arrayTj = tj1.LinkChildToTail(IGAME_JSON_TYPE_ARRAY, "member3", "");

    arrayTj->LinkChildToTail(IGAME_JSON_TYPE_INTEGER, "", "100");
    arrayTj->LinkChildToTail(IGAME_JSON_TYPE_INTEGER, "", "200");

    TiJsonNode* objArrayTj = tj1.LinkChildToTail(IGAME_JSON_TYPE_ARRAY, "member4", "");
    TiJsonNode* subObj1 = objArrayTj->LinkChildToTail(IGAME_JSON_TYPE_OBJECT, "submember1", "");
    subObj1->LinkChildToTail(IGAME_JSON_TYPE_TRUE, "member1", "");
    TiJsonNode* subObj2 = objArrayTj->LinkChildToTail(IGAME_JSON_TYPE_OBJECT, "submember2", "");
    subObj2->LinkChildToTail(IGAME_JSON_TYPE_FALSE, "member2", "");

    tj1.LinkChildToTail(IGAME_JSON_TYPE_STRING, "member5", "member5");
    tj1.LinkChildToHead(IGAME_JSON_TYPE_FLOAT, "member0", "1000.1102");

    std::string str1, str2;
    tj1.ToString(str1);
    
    TinyJsonParser::Parse(tj2, str1);
    tj2.ToString(str2);

    printf("str1:%s\n", str1.c_str());
    printf("str2:%s\n", str2.c_str());

    return 0;
}


// {"member0":1000.1102,"member1":{"submember1":"sub1","submember2":"sub2"},"member2":200,"member3":[{"array1":100},{"array2":200}],"member4":[{"submember1":{"member1":true}},{"submember2":{"member2":false}}],"member5":"member5"}
// {"member0":1000.1102,"member1":{"submember1":"sub1","submember2":"sub2"},"member2":200,"member3":[{"array1":100},{"array2":200}],"member4":[{"submember1":{"member1":true}},{"submember2":{"member2":false}}],"member5":"member5"}



