#pragma once
#include <windows.h>
#include <string>
#include <iostream>
//*set the level of logging via this file
#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

#if RELEASE==1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif
typedef enum log_level
{
    LOG_LEVEL_FATAL=0,
    LOG_LEVEL_ERROR=1,
    LOG_LEVEL_WARN=2,
    LOG_LEVEL_INFO=3,
    LOG_LEVEL_DEBUG=4,
    LOG_LEVEL_TRACE=5,
} log_level;

bool init_logging();
void shutdown_logging();

template<typename... Args>
void log_output(log_level level, Args&&... message)
{
    SetConsoleOutputCP(CP_UTF8);
    std::string levelStrings[6]={"{FATAL 💀}: ", "{ERROR 😵}: ", "{WARN ⚠️}: ",
                                 "{INFO ℹ️}: ", "{DEBUG 🐛}: ", "{TRACE}: "};
    UINT8 color[6]={65,4,6,2,5,8};
    HANDLE cHandle= GetStdHandle(STD_OUTPUT_HANDLE);
    
    //save prv attribute so I can get the prv settings back
    CONSOLE_SCREEN_BUFFER_INFO ci;
    GetConsoleScreenBufferInfo(cHandle, &ci);
    WORD originalAttrib=ci.wAttributes;

    SetConsoleTextAttribute(cHandle, color[level]);
    std::cout<<levelStrings[level];
    (std::cout<<...<<message)<<std::endl;
    //reset to original settings
    SetConsoleTextAttribute(cHandle, originalAttrib);
}

//always compiled
#define LOG_FATAL(...) log_output(LOG_LEVEL_FATAL, __VA_ARGS__);
#define LOG_ERROR(...) log_output(LOG_LEVEL_ERROR, __VA_ARGS__);

#if LOG_WARN_ENABLED==1
    #define LOG_WARN(...) log_output(LOG_LEVEL_WARN, __VA_ARGS__);
#else
    #define LOG_WARN(...)
#endif
#if LOG_INFO_ENABLED==1
    #define LOG_INFO(...) log_output(LOG_LEVEL_INFO, __VA_ARGS__);
#else
    #define LOG_INFO(...)
#endif
#if LOG_DEBUG_ENABLED==1
    #define LOG_DEBUG(...) log_output(LOG_LEVEL_DEBUG, __VA_ARGS__);
#else
    #define LOG_DEBUG(...)
#endif
#if LOG_TRACE_ENABLED==1
    #define LOG_TRACE(...) log_output(LOG_LEVEL_TRACE, __VA_ARGS__);
#else
    #define LOG_TRACE(...)
#endif



