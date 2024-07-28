#pragma once

#include"logformat.h"

namespace quicklog {
#ifdef USE_RUN_FORMAT
inline LogFormat::LogFormat(TimeType timeType)
    : m_pattern("{0} {1} thread[{3}] [{4}] line[{5}] message[{6}]\n")
    , m_timeType(timeType)
    , m_lastTime(std::chrono::system_clock::now())
{
    set_strTime(TimePoint());
}

inline const std::string& LogFormat::getPattern()
{
    return m_pattern;
}

inline void LogFormat::setPattern(const std::string& pattern)
{
    m_pattern = pattern;
}

inline void LogFormat::format(const log_msg& logMessage, std::string& content)
{
    set_strTime(logMessage.m_logTime);
    std::string strLevel(get_strLevel(logMessage.m_level));
    std::vformat_to(std::back_inserter(content), std::string_view(m_pattern)
        , std::make_format_args(m_strTime
            , logMessage.m_fileName
            , logMessage.m_functionName
            , logMessage.m_threadId
            , strLevel
            , logMessage.m_line
            , logMessage.m_message));

}

inline void LogFormat::format(const log_msg& logMessage, std::string& content, ColorMode mode)
{
    set_strTime(logMessage.m_logTime);
    std::string_view strLevel(get_strLevel(logMessage.m_level));
    switch (mode)
    {
    case ColorMode::never:
    {
        std::vformat_to(std::back_inserter(content), std::string_view(m_pattern)
            , std::make_format_args(m_strTime
                , logMessage.m_fileName
                , logMessage.m_functionName
                , logMessage.m_threadId
                , strLevel
                , logMessage.m_line
                , logMessage.m_message));
        break;
    }
    case ColorMode::always:
    {
              
        std::vformat_to(std::back_inserter(content), std::string_view(m_pattern)
            , std::make_format_args(m_strTime
                , logMessage.m_fileName
                , logMessage.m_functionName
                , logMessage.m_threadId
                , strLevel
                , logMessage.m_line
                , logMessage.m_message));
        get_color(logMessage.m_level, content);
        break;
    }
    case ColorMode::automatic:
        std::string strLevel=get_strColorLevel(logMessage.m_level);
        std::vformat_to(std::back_inserter(content), std::string_view(m_pattern)
            , std::make_format_args(m_strTime
                , logMessage.m_fileName
                , logMessage.m_functionName
                , logMessage.m_threadId
                , strLevel
                , logMessage.m_line
                , logMessage.m_message));
        break;
    }

}
#else
inline LogFormat::LogFormat(TimeType timeType)
    : m_timeType(timeType)
    , m_lastTime(std::chrono::system_clock::now())
{
    set_strTime(TimePoint());
}

inline void LogFormat::format(const log_msg& logMessage, std::string& content)
{
    set_strTime(logMessage.m_logTime);
    std::string_view strLevel(get_strLevel(logMessage.m_level));
    std::format_to(std::back_inserter(content), Global_Pattern
        , m_strTime
            , logMessage.m_fileName
            , logMessage.m_functionName
            , logMessage.m_threadId
            , strLevel
            , logMessage.m_line
            , logMessage.m_message);

}

inline void LogFormat::format(const log_msg& logMessage, std::string& content, ColorMode mode)
{
    set_strTime(logMessage.m_logTime);
    std::string_view strLevel(get_strLevel(logMessage.m_level));
    switch (mode)
    {
    case ColorMode::never:
    {
        std::format_to(std::back_inserter(content), Global_Pattern
            , m_strTime
                , logMessage.m_fileName
                , logMessage.m_functionName
                , logMessage.m_threadId
                , strLevel
                , logMessage.m_line
                , logMessage.m_message);
        break;
    }
    case ColorMode::always:
    {
        std::format_to(std::back_inserter(content), Global_Pattern
            , m_strTime
                , logMessage.m_fileName
                , logMessage.m_functionName
                , logMessage.m_threadId
                , strLevel
                , logMessage.m_line
                , logMessage.m_message);
        get_color(logMessage.m_level, content);
        break;
    }
    case ColorMode::automatic:
    {
        std::string_view strLevel = get_strColorLevel(logMessage.m_level);
        std::format_to(std::back_inserter(content), Global_Pattern
            , m_strTime
                , logMessage.m_fileName
                , logMessage.m_functionName
                , logMessage.m_threadId
                , strLevel
                , logMessage.m_line
                , logMessage.m_message);
        break;
    }
    }

}
#endif

inline void LogFormat::setTimeType(TimeType timeType)
{
    m_timeType = timeType;
}

inline void LogFormat::set_strTime(const TimePoint& timePoint)
{
    auto nowTime = std::chrono::system_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(nowTime - m_lastTime).count() >= 1)
    {        
        m_lastTime = nowTime;
        if (m_timeType == TimeType::Local)
        {
            m_strTime = timePoint.strLocal_time();
        }
        else
        {
            m_strTime = timePoint.strUTC_time();
        }
    }

}
inline const char* LogFormat::get_strLevel(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Trace:
    {
        return "Trace";
        break;
    }
    case LogLevel::Debug:
    {
        return "Debug";
        break;
    }
    case LogLevel::Info:
    {
        return "Info";
        break;
    }
    case LogLevel::Warning:
    {
        return "Warning";
        break;
    }
    case LogLevel::Error:
    {
        return "Error";
        break;
    }
    case LogLevel::Fatal:
    {
        return "Fatal";
        break;
    }
    default:
    {
        throw(log_exception("传入了一个错误的日志等级——logger.h文件被修改"));
    }
    }
}
inline const char* LogFormat::get_strColorLevel(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Trace:
    {
        return "\033[37mTrace\033[0m";
        break;
    }
    case LogLevel::Debug:
    {
        return "\033[32mDebug\033[0m";
        break;
    }
    case LogLevel::Info:
    {
        return "\033[34mInfo\033[0m";
        break;
    }
    case LogLevel::Warning:
    {
        return "\033[33mWarning\033[0m";
        break;
    }
    case LogLevel::Error:
    {
        return "\033[31mError\033[0m";
        break;
    }
    case LogLevel::Fatal:
    {
        return "\033[35mFatal\033[0m";
        break;
    }
    default:
    {
        throw(log_exception("传入了一个错误的日志等级——logger.h文件被修改"));
    }
    }
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