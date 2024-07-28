#pragma once

#include"registry.h"
#include<type_traits>
#include<iostream>

namespace quicklog{

inline Registry::Registry()
{
    std::string default_logger_name = "quicklog";
    m_default_logger = std::make_shared<Logger>(default_logger_name);
    m_default_logger->addAppender(std::make_shared<console_mt>());
    register_logger(m_default_logger);
}
inline Registry::~Registry() = default;
inline void Registry::register_logger(Logger::ptr new_logger)
{  
    auto logger_name = new_logger->getName();

    if (check_logger(logger_name))
    {
        std::cout << "logger with name " + logger_name + " already exists" << std::endl;
        return;
    }
    std::lock_guard<std::mutex> lock(m_mutex);
    m_loggerMap[logger_name] = std::move(new_logger);

}

inline Logger::ptr Registry::init_logger(const std::string& logger_name)
{         
    auto new_logger = std::make_shared<Logger>(logger_name);
    m_default_logger->addAppender(std::make_shared<console_mt>());
    register_logger(new_logger);
    return new_logger;
}
template<typename Type,typename... Args>
inline Logger::ptr Registry::init_logger(const std::string& logger_name,Args&&... args)
{    
    auto new_logger = std::make_shared<Logger>(logger_name);

    new_logger->addAppender(std::make_shared<Type>(args...));

    
    register_logger(new_logger);
    return new_logger;
}

inline Logger::ptr Registry::get(const std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_loggerMap.find(logger_name);
    return found == m_loggerMap.end() ? nullptr : found->second;
}

inline Logger::ptr Registry::default_logger()
{
    return m_default_logger;
}

inline void Registry::setLevel(const std::string& logger_name, LogLevel new_level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_loggerMap.find(logger_name);
    if (found == m_loggerMap.end())
    {
        return;
    }
    found->second->setLevel(new_level);
}
inline void Registry::setAutoLevel(const std::string& logger_name, LogLevel new_level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_loggerMap.find(logger_name);
    if (found == m_loggerMap.end())
    {
        return;
    }
    found->second->setAutoLevel(new_level);
}
#ifdef USE_RUN_FORMAT
inline void Registry::setPattern(const std::string& logger_name,const std::string& new_pattern)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_loggerMap.find(logger_name);
    if (found == m_loggerMap.end())
    {
        return;
    }
    found->second->setPattern(new_pattern);
}
#endif
inline void Registry::addAppender(const std::string& logger_name, Appender::ptr appender)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_loggerMap.find(logger_name);
    if (found == m_loggerMap.end())
    {
        return;
    }
    found->second->addAppender(appender);
}
inline void Registry::clearAppender(const std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_loggerMap.find(logger_name);
    if (found == m_loggerMap.end())
    {
        return;
    }
    found->second->clearAppender();
}
inline LogLevel Registry::getLevel(const std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_loggerMap.find(logger_name);
    if (found == m_loggerMap.end())
    {
        std::cout << "not found logger" << std::endl;
        return LogLevel::Null;
    }
    return found->second->getLevel();
}

inline LogLevel Registry::getAutoLevel(const std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_loggerMap.find(logger_name);
    if (found == m_loggerMap.end())
    {
        std::cout << "not found logger" << std::endl;
        return LogLevel::Null;
    }
    return found->second->getAutoLevel();
}
#ifdef USE_RUN_FORMAT
inline std::vector<std::string> Registry::getPattern(const std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_loggerMap.find(logger_name);
    if (found == m_loggerMap.end())
    {
        std::cout << "not found logger" << std::endl;
        return std::vector<std::string>();
    }
    return found->second->getPattern();
}
#endif
inline void Registry::setDefaultLevel(LogLevel new_level)
{  
    setLevel("quicklog", new_level);
}

inline void Registry::setDefaultAutoLevel(LogLevel new_auto_level)
{
    setAutoLevel("quicklog", new_auto_level);
}
#ifdef USE_RUN_FORMAT
inline void Registry::setDefaultPattern(const std::string& new_pattern)
{
    setPattern("quicklog", new_pattern);
}
#endif
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
#ifdef USE_RUN_FORMAT
inline std::vector<std::string> Registry::getDefaultPattern()
{
    return getPattern("quicklog");
}
#endif
inline void Registry::setAllLevel(LogLevel new_level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_loggerMap)
    {
        iter.second->setLevel(new_level);
    }
}

inline void Registry::setAllAutoLevel(LogLevel new_auto_level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_loggerMap)
    {
        iter.second->setAutoLevel(new_auto_level);
    }
}
#ifdef USE_RUN_FORMAT
inline void Registry::setAllPattern(const std::string& new_pattern)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_loggerMap)
    {
        iter.second->setPattern(new_pattern);
    }
}
#endif
inline void Registry::addAllAppender(Appender::ptr appender)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_loggerMap)
    {
        iter.second->addAppender(appender);
    }
}
inline void Registry::clearAllAppender()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_loggerMap)
    {
        iter.second->clearAppender();
    }
}

inline void Registry::flush_all()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_loggerMap)
    {
        iter.second->flush();
    }
}

inline void Registry::drop(const std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto is_default_logger = (m_default_logger->getName() == logger_name);
    m_loggerMap.erase(logger_name);
    if (is_default_logger)
    {
        m_default_logger = nullptr;
    }
}

inline void Registry::drop_all()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_loggerMap.clear();
    m_default_logger = nullptr;
}

inline bool Registry::check_logger(std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_loggerMap.find(logger_name) == m_loggerMap.end())
    {
        return false;
    }
    return true;
}
}//namespace quicklog