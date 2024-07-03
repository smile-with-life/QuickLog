#pragma once

#include"logformat.h"

namespace quicklog {
inline LogFormat::LogFormat()
    : m_pattern("{0} {1} thread[{3}] [{4}] line[{5}] message[{6}]")
    , m_color_pattern("{0} {1} thread[{3}] [{4}] line[{5}] message[{6}]")
    , m_timeType(TimeType::Local)
    , m_lastTime(std::chrono::system_clock::now())
    , m_strTime(get_strTime(m_timeType, TimePoint()))
{
    m_pattern += '\n';
}
inline LogFormat::LogFormat(const std::string& pattern)
    : m_pattern(pattern)
    , m_color_pattern(pattern)
    , m_timeType(TimeType::Local)
    , m_lastTime(std::chrono::system_clock::now())
    , m_strTime(get_strTime(m_timeType, TimePoint()))
{
    m_pattern += '\n';
}
 inline LogFormat::LogFormat(const std::string& pattern, TimeType timeType)
     : m_pattern(pattern)
     , m_color_pattern(pattern)
     , m_timeType(timeType)
     , m_lastTime(std::chrono::system_clock::now())
     , m_strTime(get_strTime(m_timeType, TimePoint()))
{
    m_pattern += '\n';
}
inline LogFormat::ptr LogFormat::clone()
{
    return std::make_unique<LogFormat>(m_pattern, m_timeType);
}
inline const std::string& LogFormat::getPattern()
{
    return m_pattern.data();
}

inline void LogFormat::setPattern(const std::string& pattern)
{
    m_pattern = pattern;
    m_pattern += '\n';
}

inline void LogFormat::format(const log_msg& logMessage, std::string& content)
{
    auto nowTime = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - m_lastTime).count() >= 1)
    {
        m_strTime = get_strTime(m_timeType, logMessage.m_logTime);
        m_lastTime = nowTime;
    }
    std::vformat_to(std::back_inserter(content), std::string_view(m_pattern)
        , std::make_format_args(m_strTime
            , logMessage.m_location.file_name()
            , logMessage.m_location.function_name()
            , logMessage.m_threadId
            , strLogLevel(logMessage.m_level)
            , logMessage.m_location.line()
            , logMessage.m_message.data()));   
     
}
inline void LogFormat::format(const log_msg& logMessage, std::string& content, ColorMode mode)
{
    auto nowTime = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - m_lastTime).count() >= 1)
    {
        m_strTime = get_strTime(m_timeType, logMessage.m_logTime);
        m_lastTime = nowTime;
    }
    switch (mode)
    {
    case ColorMode::never:
    {
        std::vformat_to(std::back_inserter(content), std::string_view(m_pattern)
            , std::make_format_args(m_strTime
                , logMessage.m_location.file_name()
                , logMessage.m_location.function_name()
                , logMessage.m_threadId
                , strLogLevel(logMessage.m_level)
                , logMessage.m_location.line()
                , logMessage.m_message.data()));
        break;
    }     
    case ColorMode::always:
    {
        get_color(logMessage.m_level,m_color_pattern);
        std::vformat_to(std::back_inserter(content), std::string_view(m_color_pattern)
            , std::make_format_args(m_strTime
                , logMessage.m_location.file_name()
                , logMessage.m_location.function_name()
                , logMessage.m_threadId
                , strLogLevel(logMessage.m_level)
                , logMessage.m_location.line()
                , logMessage.m_message.data()));
        m_color_pattern = m_pattern;
        break;
    }
    case ColorMode::automatic:
        std::vformat_to(std::back_inserter(content), std::string_view(m_pattern)
            , std::make_format_args(m_strTime
                , logMessage.m_location.file_name()
                , logMessage.m_location.function_name()
                , logMessage.m_threadId
                , strLogColorLevel(logMessage.m_level)
                , logMessage.m_location.line()
                , logMessage.m_message.data()));
        break;
    }
    
}

inline std::string LogFormat::get_strTime(TimeType timeType,const TimePoint& timePoint)
{

    if (timeType == TimeType::Local)
    {
        return timePoint.strLocal_time();
    }
    return timePoint.strUTC_time();
}
inline void LogFormat::get_color(LogLevel level, std::string& str)
{
    switch (level)
    {
    case LogLevel::Trace :
    {
        str.insert(0, "\033[37m");
        str.insert(str.size(), "\033[0m");
        break;
    }    
    case LogLevel::Debug :
    {
        str.insert(0, "\033[32m");
        str.insert(str.size(), "\033[0m");
        break;
    }
    case LogLevel::Info :
    {
        str.insert(0, "\033[34m");
        str.insert(str.size(), "\033[0m");
        break;
    }
    case LogLevel::Warning :
    {
        str.insert(0, "\033[33m");
        str.insert(str.size(), "\033[0m");
        break;
    }
    case LogLevel::Error :
    {
        str.insert(0, "\033[31m");
        str.insert(str.size(), "\033[0m");
        break;
    }
    case LogLevel::Fatal :
    {
        str.insert(0, "\033[35m");
        str.insert(str.size(), "\033[0m");
        break;
    }
    }
}
}//namespace quicklog