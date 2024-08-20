#pragma once
#include "base_appender.h"

namespace quicklog{
template<typename Mutex>
inline Base_Appender<Mutex>::Base_Appender()
    : m_format(std::make_unique<LogFormat>())
{}

template<typename Mutex>
inline void Base_Appender<Mutex>::log(const logMsg& log_msg)
{
    std::lock_guard<Mutex> lock(m_mutex);   
    native_log(log_msg);
}

template<typename Mutex>
inline void Base_Appender<Mutex>::flush()
{
    std::lock_guard<Mutex> lock(m_mutex);
    native_flush();
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
