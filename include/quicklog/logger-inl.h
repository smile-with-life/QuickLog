#pragma once

#include <quicklog/logger.h>
#include <quicklog/logformat.h>
#include <quicklog/appender/appender.h>

namespace quicklog{
inline Logger::Logger(std::string name)
    : m_name(name)
    , m_level(LogLevel::All)
    , m_auto_level(LogLevel::Warning)
    , m_appender_list()
{}
inline Logger::Logger(std::string name, LogLevel level)
    : m_name(name)
    , m_level(level)
    , m_auto_level(LogLevel::Warning)
    , m_appender_list()
{}
inline Logger::Logger(std::string name, LogLevel level, LogLevel auto_level)
    : m_name(name)
    , m_level(level)
    , m_auto_level(auto_level)
    , m_appender_list()
{}
inline Logger::Logger(const Logger& other)
    : m_name(other.m_name)
    , m_level(other.m_level)
    , m_auto_level(other.m_auto_level)
    , m_appender_list(other.m_appender_list)
    {}

inline Logger::Logger(Logger&& other) noexcept
    : m_name(std::move(other.m_name))
    , m_level(std::move(other.m_level))
    , m_auto_level(std::move(other.m_auto_level))
    , m_appender_list(std::move(other.m_appender_list))
    {}

inline Logger& Logger::operator=(Logger other) noexcept
{
    this->swap(other);
    return *this;
}

inline void Logger::swap(Logger& other) noexcept
{
    //swap m_logName
    m_name.swap(other.m_name);

    // swap m_level
    auto other_level = other.m_level;
    other.m_level = m_level;
    m_level = other_level;

    //swap m_auto_flush_level
    auto other_auto_level = other.m_auto_level;
    other.m_auto_level = m_auto_level;
    m_auto_level = other_auto_level;

    //swap m_appenderList
    m_appender_list.swap(other.m_appender_list);
}

inline void Logger::log(const logMsg& log_msg)
{ 
    native_log(log_msg);
}

inline void Logger::flush()
{
    native_flush();
}

inline const LogLevel Logger::getLevel() const noexcept
{
    return m_level;
}
inline const LogLevel Logger::getAutoLevel() const noexcept
{
    return m_auto_level;
}

inline const std::string Logger::getName() const noexcept
{
    return m_name;
}

void Logger::setLevel(LogLevel level) noexcept
{
    m_level = level;
}
inline void Logger::setAutoLevel(LogLevel level) noexcept
{
    m_auto_level = level;
}

inline void Logger::setTimeType(TimeType time_type) noexcept
{
    m_time_type = time_type;
}

#ifdef USE_RUN_FORMAT
inline const std::vector<std::string> Logger::getPattern() const
{
    std::vector<std::string> patternList;
    for (auto& iter : m_appenderList)
    {
        patternList.push_back(iter->getPattern());
    }
    return patternList;
}

inline void Logger::setPattern(const std::string& pattern)
{
    for (auto& iter : m_appenderList)
    {
        iter->setPattern(pattern);
    }

}
#endif

inline void Logger::addAppender(Appender_ptr appender)
{
    if (m_appender_list.size() <= MaxAppenderCount)
    {
        for (auto& iter : m_appender_list)
        {
            if (iter == appender)
            {
                std::cout << "repeatedly contain the same of appender!";
                return;
            }
        }
        m_appender_list.push_back(appender);
    }
    else
    {
        std::cout << "appender list is already full!";
    }
}

inline void Logger::clearAppender() noexcept
{
    m_appender_list.clear();
}

inline void Logger::native_log(const logMsg& log_msg)
{
    for (const auto& iter : m_appender_list)
    {
        iter->log(log_msg);
    }

    if (should_flush(log_msg.m_level))
    {
        flush();
    }
}

inline void Logger::native_flush()
{
    for (auto& iter : m_appender_list)
    {
        iter->flush();
    }
}

inline bool Logger::should_flush(const LogLevel level) const noexcept
{
    return (level >= m_auto_level);
}

void swap(Logger& left, Logger& right) noexcept
{
    left.swap(right);
}

}