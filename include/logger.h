#pragma once
//set the level of logging via this file
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
void log_output(log_level level, std::string message);

//always compiled
//TODO: use cpp variatic templates to allow for more arguements than just a message
#define LOG_FATAL(message, ...) log_output(LOG_LEVEL_FATAL, message);
#define LOG_ERROR(message, ...) log_output(LOG_LEVEL_ERROR, message);

#if LOG_WARN_ENABLED==1
    #define LOG_WARN(message, ...) log_output(LOG_LEVEL_WARN, message);
#else
    #define LOG_WARN(message, ...)
#endif
#if LOG_INFO_ENABLED==1
    #define LOG_INFO(message, ...) log_output(LOG_LEVEL_INFO, message);
#else
    #define LOG_INFO(message, ...)
#endif
#if LOG_DEBUG_ENABLED==1
    #define LOG_DEBUG(message, ...) log_output(LOG_LEVEL_DEBUG, message);
#else
    #define LOG_DEBUG(message, ...)
#endif
#if LOG_TRACE_ENABLED==1
    #define LOG_TRACE(message, ...) log_output(LOG_LEVEL_TRACE, message);
#else
    #define LOG_TRACE(message, ...)
#endif
