#pragma once
#include "base_appender.h"

namespace quicklog{
template<typename Mutex>
inline Base_Appender<Mutex>::Base_Appender()
    : m_format(std::make_unique<LogFormat>())
{}

template<typename Mutex>
inline Base_Appender<Mutex>::Base_Appender(LogFormat::ptr format)
    : m_format(std::move(format))
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
inline void Base_Appender<Mutex>::setFormat(LogFormat::ptr format)
{
    std::lock_guard<Mutex> lock(m_mutex);
    m_format = std::move(format);
}

template<typename Mutex>
void Base_Appender<Mutex>::setPattern(const std::string& pattern)
{
    std::lock_guard<Mutex> lock(m_mutex);
    setFormat(std::make_unique<LogFormat>(pattern));
}
}//namespace quicklog
