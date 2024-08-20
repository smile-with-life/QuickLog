#pragma once

#include <quicklog/details/registry.h>
#include <quicklog/logger.h>
#include <quicklog/logformat.h>
#include <quicklog/appender/file_appender.h>
#include <quicklog/appender/stdout_appender.h>

namespace quicklog{

inline Registry::Registry()
{
    std::string default_logger_name = "quicklog";
    m_default_logger = std::make_shared<Logger>(default_logger_name);
    m_default_logger->addAppender(std::make_shared<stdout_mt>());
    enroll(m_default_logger);
}

inline void Registry::enroll(Logger_ptr new_logger)
{  
    std::string logger_name = new_logger->getName();
    if (check_logger(logger_name))
    {
        throw(log_exception("logger with name " + logger_name + " already exists"));
        return;
    }
    std::lock_guard<std::mutex> lock(m_mutex);
    m_logger_map[logger_name] = std::move(new_logger);

}

inline Logger_ptr Registry::init(const std::string& logger_name)
{         
    auto new_logger = std::make_shared<Logger>(logger_name);
    new_logger->addAppender(std::make_shared<stdout_mt>());
    enroll(new_logger);
    return new_logger;
}

template<typename Type,typename... Args>
inline Logger_ptr Registry::init(const std::string& logger_name,Args&&... args)
{    
    auto new_logger = std::make_shared<Logger>(logger_name);
    new_logger->addAppender(std::make_shared<Type>(args...));   
    enroll(new_logger);
    return new_logger;
}

inline Logger_ptr Registry::init_async(std::weak_ptr<ThreadPool> thread_pool, const std::string& logger_name)
{
    auto new_logger = std::make_shared<AsyncLogger>(thread_pool, logger_name);
    new_logger->addAppender(std::make_shared<stdout_mt>());
    enroll(new_logger);
    return new_logger;
}

template<typename Type, typename... Args>
inline Logger_ptr Registry::init_async(std::weak_ptr<ThreadPool> thread_pool, const std::string& logger_name, Args&&... args)
{
    auto new_logger = std::make_shared<AsyncLogger>(thread_pool, logger_name);
    new_logger->addAppender(std::make_shared<Type>(args...));
    enroll(new_logger);
    return new_logger;
}

inline Logger_ptr Registry::get(const std::string& logger_name) noexcept
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_logger_map.find(logger_name);
    return found == m_logger_map.end() ? nullptr : found->second;
}

inline Logger_ptr Registry::default_ptr() noexcept
{
    return m_default_logger;
}

inline Logger* Registry::default_raw() noexcept
{
    return m_default_logger.get();
}

inline void Registry::setLevel(const std::string& logger_name, LogLevel new_level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_logger_map.find(logger_name);
    if (found == m_logger_map.end())
    {
        throw(log_exception("not found logger"));
    }
    found->second->setLevel(new_level);
}

inline void Registry::setAutoLevel(const std::string& logger_name, LogLevel new_level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_logger_map.find(logger_name);
    if (found == m_logger_map.end())
    {
        throw(log_exception("not found logger"));
    }
    found->second->setAutoLevel(new_level);
}

inline void Registry::addAppender(const std::string& logger_name, Appender::ptr appender)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_logger_map.find(logger_name);
    if (found == m_logger_map.end())
    {
        throw(log_exception("not found logger"));
        
    }
    found->second->addAppender(appender);
}

inline void Registry::clearAppender(const std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_logger_map.find(logger_name);
    if (found == m_logger_map.end())
    {
        throw(log_exception("not found logger"));
        
    }
    found->second->clearAppender();
}

inline LogLevel Registry::getLevel(const std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_logger_map.find(logger_name);
    if (found == m_logger_map.end())
    {
        throw(log_exception("not found logger"));
    }
    return found->second->getLevel();
}

inline LogLevel Registry::getAutoLevel(const std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_logger_map.find(logger_name);
    if (found == m_logger_map.end())
    {
        throw(log_exception("not found logger"));
    }
    return found->second->getAutoLevel();
}

inline void Registry::setDefaultLevel(LogLevel new_level)
{  
    setLevel("quicklog", new_level);
}

inline void Registry::setDefaultAutoLevel(LogLevel new_auto_level)
{
    setAutoLevel("quicklog", new_auto_level);
}

inline void Registry::addDefaultAppender(Appender::ptr appender)
{
    addAppender("quicklog", appender);
}

inline void Registry::clearDefaultAppender()
{
    clearAppender("quicklog");
}

inline LogLevel Registry::getDefaultLevel()
{
    return getLevel("quicklog");
}

inline LogLevel Registry::getDefaultAutoLevel()
{
    return getAutoLevel("quicklog");
}

inline void Registry::setAllLevel(LogLevel new_level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_logger_map)
    {
        iter.second->setLevel(new_level);
    }
}

inline void Registry::setAllAutoLevel(LogLevel new_auto_level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_logger_map)
    {
        iter.second->setAutoLevel(new_auto_level);
    }
}

inline void Registry::addAllAppender(Appender::ptr appender)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_logger_map)
    {
        iter.second->addAppender(appender);
    }
}

inline void Registry::clearAllAppender()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_logger_map)
    {
        iter.second->clearAppender();
    }
}

#ifdef USE_RUN_FORMAT
inline void Registry::setPattern(const std::string& logger_name, const std::string& new_pattern)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_loggerMap.find(logger_name);
    if (found == m_loggerMap.end())
    {
        throw(log_exception("not found logger"));
    }
    found->second->setPattern(new_pattern);
}


inline void Registry::setDefaultPattern(const std::string& new_pattern)
{
    setPattern("quicklog", new_pattern);
}

inline void Registry::setAllPattern(const std::string& new_pattern)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_loggerMap)
    {
        iter.second->setPattern(new_pattern);
    }
}

inline std::vector<std::string> Registry::getPattern(const std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_loggerMap.find(logger_name);
    if (found == m_loggerMap.end())
    {
        throw(log_exception("not found logger"));
    }
    return found->second->getPattern();
}

inline std::vector<std::string> Registry::getDefaultPattern()
{
    return getPattern("quicklog");
}
#endif

inline void Registry::flush_all()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_logger_map)
    {
        iter.second->flush();
    }
}

inline void Registry::drop(const std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto is_default_logger = (m_default_logger->getName() == logger_name);
    m_logger_map.erase(logger_name);
    if (is_default_logger)
    {
        m_default_logger = nullptr;
    }
}

inline void Registry::drop_all()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_logger_map.clear();
    m_default_logger = nullptr;
}

inline bool Registry::check_logger(std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_logger_map.find(logger_name) == m_logger_map.end())
    {
        return false;
    }
    return true;
}
}//namespace quicklog