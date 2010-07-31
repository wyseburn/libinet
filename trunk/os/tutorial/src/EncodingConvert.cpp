
#include "os/include/EncodingConvert.h"

using namespace IGame;

int main(int argc, char *argv[])
{
    EncodingConvert conv1("EUC-CN", "UTF-8");
    EncodingConvert conv2("UTF-8", "EUC-CN");

    std::string str = "aÖÐ¹úb";

    std::string utfString = conv1.Convert(str.c_str());
    std::string gbkString = conv2.Convert(utfString.c_str());

    printf("gbkString: %s\n", gbkString.c_str());
    printf("utfString: %s\n", utfString.c_str());

    return 0;
}



