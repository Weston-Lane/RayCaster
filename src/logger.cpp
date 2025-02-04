
#include <iostream>
#include <string>
#include <windows.h>
#include "logger.h"
    // LOG_DEBUG("this is debug");
    // LOG_ERROR("this is an error");
    // LOG_FATAL("THIS IS FATAL");
    // LOG_INFO("information");
    // LOG_TRACE("tracing");
bool init_logging()
{
    return true;
}
void shutdown_logging()
{
    
}
void log_output(log_level level, std::string message)
{
    SetConsoleOutputCP(CP_UTF8);
    std::string levelStrings[6]={"{FATAL 💀}: ", "{ERROR 😵}: ", "{WARN ⚠️}: ",
                                 "{INFO ℹ️}: ", "{DEBUG 🐛}: ", "{TRACE}: "};
    uint8_t color[6]={65,4,6,2,1,8};
    HANDLE cHandle= GetStdHandle(STD_OUTPUT_HANDLE);
    
    //save prv attribute so I can get the prv settings back
    CONSOLE_SCREEN_BUFFER_INFO ci;
    GetConsoleScreenBufferInfo(cHandle, &ci);
    WORD originalAttrib=ci.wAttributes;

    SetConsoleTextAttribute(cHandle, color[level]);
    std::cout<<levelStrings[level]<<message<<std::endl;
    //reset to original settings
    SetConsoleTextAttribute(cHandle, originalAttrib);
}