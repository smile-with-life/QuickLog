#pragma once
#include "base_appender.h"

namespace quicklog{
template<typename Mutex>
inline Base_Appender<Mutex>::Base_Appender(TimeType timeType)
    : m_format(std::make_unique<LogFormat>(timeType))
{}

template<typename Mutex>
inline void Base_Appender<Mutex>::log(const log_msg& logMessage)
{
    std::lock_guard<Mutex> lock(m_mutex);   
    print(logMessage);
}

template<typename Mutex>
inline void Base_Appender<Mutex>::log(const std::string& content)
{
    std::lock_guard<Mutex> lock(m_mutex);
    print(content);
}

template<typename Mutex>
inline void Base_Appender<Mutex>::flush()
{
    std::lock_guard<Mutex> lock(m_mutex);
    native_flush();
}
template<typename Mutex>
inline void Base_Appender<Mutex>::setTimeType(TimeType timeType)
{
    std::lock_guard<Mutex> lock(m_mutex);
    m_format->setTimeType(timeType);
}

#ifdef USE_RUN_FORMAT
template<typename Mutex>
inline void Base_Appender<Mutex>::setPattern(const std::string& pattern)
{
    std::lock_guard<Mutex> lock(m_mutex);
    m_format->setPattern(pattern);
}

template<typename Mutex>
inline std::string Base_Appender<Mutex>::getPattern()
{
    std::lock_guard<Mutex> lock(m_mutex);
    return m_format->getPattern();
}
#endif

}//namespace quicklog
