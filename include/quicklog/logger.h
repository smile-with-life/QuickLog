#pragma once

#include <quicklog/common.h>
#include <quicklog/details/logmsg.h>

#include <vector>
#include <format>

namespace quicklog {
// 日志器
class Logger
{
public:
    explicit Logger(std::string name);
    Logger(std::string name, LogLevel level);
    Logger(std::string name, LogLevel level, LogLevel auto_level);

    virtual ~Logger() = default;

    Logger(const Logger& other);
    Logger(Logger&& other) noexcept;
    Logger& operator=(const Logger other) noexcept;

    void swap(Logger& other) noexcept;

public://常规日志打印函数
    template<class T>//进行编译时期检查，确保T类型能被format格式化
    void Trace(const T& message, std::source_location location = std::source_location::current())
    {
        if (LogLevel::Trace >= m_level)
        {
            auto log_msg = logMsg(logTime(m_time_type), location, getThreadId(), LogLevel::Trace, message);
            log(log_msg);
        }
    }
    template<class T>
    void Debug(const T& message, std::source_location location = std::source_location::current())
    {
        if (LogLevel::Debug >= m_level)
        {
            auto log_msg = logMsg(logTime(m_time_type), location, getThreadId(), LogLevel::Debug, message);
            log(log_msg);
        }
    }
    template<class T>
    void Info(const T& message, std::source_location location = std::source_location::current())
    {
        if (LogLevel::Info >= m_level)
        {
            auto log_msg = logMsg(logTime(m_time_type), location, getThreadId(), LogLevel::Info, message);
            log(log_msg);
        }
    }
    template<class T>
    void Warning(const T& message, std::source_location location = std::source_location::current())
    {
        if (LogLevel::Warning >= m_level)
        {
            auto log_msg = logMsg(logTime(m_time_type), location, getThreadId(), LogLevel::Warning, message);
            log(log_msg);
        }
    }
    template<class T>
    void Error(const T& message, std::source_location location = std::source_location::current())
    {
        if (LogLevel::Error >= m_level)
        {
            auto log_msg = logMsg(logTime(m_time_type), location, getThreadId(), LogLevel::Error, message);
            log(log_msg);
        }
    }
    template<class T>
    void Fatal(const T& message, std::source_location location = std::source_location::current())
    {
        if (LogLevel::Fatal >= m_level)
        {
            auto log_msg = logMsg(logTime(m_time_type), location, getThreadId(), LogLevel::Fatal, message);
            log(log_msg);
        }
    }
public://可变参数日志打印函数
    template <typename... Args>
    void Trace(with_source_location<std::format_string<Args...>> with_format, Args &&...args)
    {
        if (LogLevel::Trace >= m_level)
        {
            std::string message;
            std::format_to(std::back_inserter(message), with_format.format(), std::forward<Args>(args)...);
            auto log_msg = logMsg(logTime(m_time_type), with_format.location(), getThreadId(), LogLevel::Trace, message);
            log(log_msg);
        }       
    }

    template <typename... Args>
    void Debug(with_source_location<std::format_string<Args...>> with_format, Args &&...args)
    {
        if (LogLevel::Debug >= m_level)
        {
            std::string message;
            std::format_to(std::back_inserter(message), with_format.format(), std::forward<Args>(args)...);
            auto log_msg = logMsg(logTime(m_time_type), with_format.location(), getThreadId(), LogLevel::Debug, message);
            log(log_msg);
        }
    }

    template <typename... Args>
    void Info(with_source_location<std::format_string<Args...>> with_format, Args &&...args)
    {
        if (LogLevel::Info >= m_level)
        {
            std::string message;
            std::format_to(std::back_inserter(message), with_format.format(), std::forward<Args>(args)...);
            auto log_msg = logMsg(logTime(m_time_type), with_format.location(), getThreadId(), LogLevel::Info, message);
            log(log_msg);
        }
    }

    template <typename... Args>
    void Warning(with_source_location<std::format_string<Args...>> with_format, Args &&...args)
    {
        if (LogLevel::Warning >= m_level)
        {
            std::string message;
            std::format_to(std::back_inserter(message), with_format.format(), std::forward<Args>(args)...);
            auto log_msg = logMsg(logTime(m_time_type), with_format.location(), getThreadId(), LogLevel::Warning, message);
            log(log_msg);
        }
    }

    template <typename... Args>
    void Error(with_source_location<std::format_string<Args...>> with_format, Args &&...args)
    {
        if (LogLevel::Error >= m_level)
        {
            std::string message;
            std::format_to(std::back_inserter(message), with_format.format(), std::forward<Args>(args)...);
            auto log_msg = logMsg(logTime(m_time_type), with_format.location(), getThreadId(), LogLevel::Error, message);
            log(log_msg);
        }
    }

    template <typename... Args>
    void Fatal(with_source_location<std::format_string<Args...>> with_format, Args &&...args)
    {
        if (LogLevel::Fatal >= m_level)
        {
            std::string message;
            std::format_to(std::back_inserter(message), with_format.format(), std::forward<Args>(args)...);
            auto log_msg = logMsg(logTime(m_time_type), with_format.location(), getThreadId(), LogLevel::Fatal, message);
            log(log_msg);
        }
    }
public://日志类的基本操作
    void log(const logMsg& log_msg);

    void flush();

    const std::string getName() const  noexcept;
         
    const LogLevel getLevel() const  noexcept;

    const LogLevel getAutoLevel() const  noexcept;

#ifdef USE_RUN_FORMAT
    const std::vector<std::string> getPattern() const;

    void setPattern(const std::string& pattern);
#endif 
    void setLevel(LogLevel level) noexcept;

    void setAutoLevel(LogLevel level) noexcept;

    void setTimeType(TimeType time_type) noexcept;

    void addAppender(Appender_ptr appender);

    void clearAppender() noexcept;
protected:
    virtual void native_log(const logMsg& log_msg);

    virtual void native_flush();

    bool should_flush(const LogLevel level) const noexcept;

protected:
    std::string m_name;//日志名称
    LogLevel m_level;//日志级别
    LogLevel m_auto_level;//自动刷新的级别
    TimeType m_time_type{ TimeType::Local };
    std::vector<Appender_ptr> m_appender_list;//日志输出列表
};
using Logger_ptr = std::shared_ptr<Logger>;
}

#include "logger-inl.h"