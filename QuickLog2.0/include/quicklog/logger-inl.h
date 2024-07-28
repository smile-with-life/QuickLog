#pragma once

#include <quicklog/logger.h>
#include <quicklog/details/log_msg.h>
#include <quicklog/details/timepoint.h>

namespace quicklog{
inline Logger::Logger(const Logger& other)
    : m_logName(other.m_logName)
    , m_level(other.m_level)
    , m_auto_flush_level(other.m_auto_flush_level)
    , m_appenderList(other.m_appenderList)  
    {}

inline Logger::Logger(Logger&& other) noexcept
    : m_logName(std::move(other.m_logName))
    , m_level(std::move(other.m_level))
    , m_auto_flush_level(std::move(other.m_auto_flush_level))
    , m_appenderList(std::move(other.m_appenderList))
    {}

inline Logger& Logger::operator=(Logger other) noexcept
{
    this->swap(other);
    return *this;
}

inline void Logger::swap(Logger& other) noexcept
{
    //swap m_logName
    m_logName.swap(other.m_logName);

    // swap m_level
    auto other_level = other.m_level;
    other.m_level = m_level;
    m_level = other_level;

    //swap m_auto_flush_level
    auto other_auto_flush_level = other.m_auto_flush_level;
    other.m_auto_flush_level = m_auto_flush_level;
    m_auto_flush_level = other_auto_flush_level;

    //swap m_appenderList
    m_appenderList.swap(other.m_appenderList);
}

inline void Logger::log(const log_msg& logMessage)
{ 

    for (const auto& iter : m_appenderList)
    {
        iter->log(logMessage);
    }
    
    if (should_flush(logMessage.m_level))
    {
        flush();
    }
    
}

inline void Logger::flush()
{
    for (auto& iter : m_appenderList)
    {
        iter->flush();
    }
}

inline const LogLevel Logger::getLevel() const
{
    return m_level;
}
inline const LogLevel Logger::getAutoLevel() const
{
    return m_auto_flush_level;
}

inline const std::string Logger::getName() const
{
    return m_logName;
}

void Logger::setLevel(LogLevel level)
{
    m_level = level;
}
inline void Logger::setAutoLevel(LogLevel level)
{
    m_auto_flush_level = level;
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
inline void Logger::addAppender(Appender::ptr appender)
{
    if (m_appenderList.size() <= MaxAppenderCount)
    {
        for (auto& iter : m_appenderList)
        {
            if (iter == appender)
            {
                std::cout << "repeatedly contain the same of appender!";
                return;
            }
        }
        m_appenderList.push_back(appender);
    }
    else
    {
        std::cout << "appender list is already full!";
    }
}
inline void Logger::setTimeType(TimeType timeType)
{
    for (auto& iter : m_appenderList)
    {
        iter->setTimeType(timeType);
    }
}
inline void Logger::clearAppender()
{
    m_appenderList.clear();
}

inline bool Logger::should_flush(const LogLevel level) const
{
    return (level >= m_auto_flush_level);
}

void swap(Logger& left, Logger& right)
{
    left.swap(right);
}

}