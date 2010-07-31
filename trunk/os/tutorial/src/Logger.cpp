
#include "os/include/Logger.h"

#define DEFAULT_LOG_CONFIG_FILE "SimpleLogConfig.txt"

using namespace IGame;

int main(int argc, char *argv[])
{
    //LOG_TO_SYSLOG(); 
    LOG_SILENCE_LEVEL(0);
    //LOG_OUTPUT_FILE("./aaa");
    LOG_DEBUG("test debug");
    LOG_INFO("test info, %s", "213");
    LOG_WARN("test warn");
    LOG_ERROR("test error"); 
    LOG_PRINT("test print");
    return 0;
}



