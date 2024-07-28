#pragma once
#include <source_location>
#include <string>
#include "timepoint.h"
#include "../common.h"
namespace quicklog {
struct log_msg //日志消息
{
    log_msg() = default;
    log_msg(TimePoint&& logTime, std::source_location& location, unsigned int logThreadId  \
        , LogLevel level,std::string_view message)
        : m_logTime(logTime)
        , m_fileName(location.file_name())
        , m_functionName(location.function_name())
        , m_threadId(logThreadId)
        , m_level(level)
        , m_line(location.line())
        , m_message(std::move(message))
    {}
    log_msg(const log_msg& other) = default;
    log_msg& operator=(const log_msg& other) = default;

    TimePoint m_logTime;//产生日志消息的时间
    std::string_view m_fileName;//文件名
    std::string_view m_functionName;//函数名   
    unsigned int m_threadId;//产生日志消息的线程ID
    LogLevel m_level;//日志消息的级别
    uint32_t m_line;//行号
    std::string_view m_message;//日志消息的内容

};
}