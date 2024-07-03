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
        , m_location(location)
        , m_threadId(logThreadId)
        , m_level(level)
        , m_message(message)
    {}
    log_msg(const log_msg& other) = default;
    log_msg& operator=(const log_msg& other) = default;

    TimePoint m_logTime;//产生日志消息的时间
    std::source_location m_location;
    unsigned int m_threadId;//产生日志消息的线程ID
    LogLevel m_level;//日志消息的级别
    std::string_view m_message;//日志消息的内容

    // wrapping the formatted text with color (updated by pattern_formatter).用颜色包装格式化的文本（由 pattern_formatter 更新）。
    //用于在控制台输出时，指定消息中某一部分的颜色范围
    //mutable size_t color_range_start{ 0 };
    ///mutable size_t color_range_end{ 0 };

};
}