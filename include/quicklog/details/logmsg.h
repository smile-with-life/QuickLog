#pragma once

#include<quicklog/common.h>



namespace quicklog {
struct logMsg //日志消息
{
    logMsg() = default;
    logMsg(logTime log_time, std::source_location location, unsigned int threadID  \
        , LogLevel level,std::string_view message)
        : m_log_time(log_time)
        , m_location(location)
        , m_tid(threadID)
        , m_level(level)
        , m_message(std::move(message))
    {}

    virtual ~logMsg() = default;
    logMsg(const logMsg& other) = default;
    logMsg& operator=(const logMsg& other) = default;

    logTime m_log_time;//产生日志消息的时间
    std::source_location m_location;//产生日志消息的代码位置信息
    unsigned int m_tid{0};//产生日志消息的线程ID
    LogLevel m_level{LogLevel::Trace};//日志消息的级别
    std::string_view m_message;//日志消息的内容

    mutable ColorMode m_mode{ ColorMode::never };

};
}