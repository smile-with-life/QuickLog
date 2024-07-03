#pragma once

#include"registry.h"

namespace quicklog{

inline Registry::Registry()
    : m_default_format(std::make_unique<LogFormat>())
    , m_default_level(LogLevel::All)
    , m_default_auto_level(LogLevel::Warning)
{
    const char* default_logger_name = "quicklog";
    m_default_logger = std::make_shared<Logger>(default_logger_name);
    m_default_logger->addAppender(std::make_shared<console_mt>());
    m_default_logger->setFormat(m_default_format->clone());
    m_default_logger->setLevel(m_default_level);
    m_default_logger->setAutoLevel(m_default_auto_level);
    register_logger(m_default_logger);
}
inline Registry::~Registry() = default;
inline void Registry::register_logger(Logger::ptr new_logger)
{  
    auto logger_name = new_logger->getName();
    try 
    {
        check_logger(logger_name);
        std::lock_guard<std::mutex> lock(m_mutex);
        m_loggerMap[logger_name] = std::move(new_logger);
    }
    catch (log_exception& excption)
    {
        std::cout << excption.what() << std::endl;
    }
    catch (...)
    {
        throw;
    }
}
inline void Registry::check_logger(std::string& logger_name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_loggerMap.find(logger_name) != m_loggerMap.end())
    {
        throw(log_exception("logger with name '" + logger_name + "' already exists"));
    }
}
inline Logger::ptr Registry::init_logger(const std::string& logger_name)
{         
    auto new_logger = std::make_shared<Logger>(logger_name);
    new_logger->addAppender(std::make_shared<console_mt>());
    new_logger->setFormat(m_default_format->clone());
    new_logger->setLevel(m_default_level);
    new_logger->setAutoLevel(m_default_auto_level);   
    register_logger(new_logger);
    return new_logger;
}
template<typename Type>
inline Logger::ptr Registry::init_logger(const std::string& logger_name,const std::string& fileName)
{           
    auto new_logger = std::make_shared<Logger>(logger_name);
    new_logger->addAppender(std::make_shared<Type>(fileName));
    new_logger->setFormat(m_default_format->clone());
    new_logger->setLevel(m_default_level);
    new_logger->setAutoLevel(m_default_auto_level);
    
    register_logger(new_logger);
    return new_logger;
}
template<typename Type>
inline Logger::ptr Registry::init_logger(const std::string& logger_name, ColorMode colorMode)
{   
    auto new_logger = std::make_shared<Logger>(logger_name);
    new_logger->addAppender(std::make_shared<Type>(colorMode));
    new_logger->setFormat(m_default_format->clone());
    new_logger->setLevel(m_default_level);
    new_logger->setAutoLevel(m_default_auto_level);
    
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
inline Logger* Registry::get_default_raw()
{
    return m_default_logger.get();
}
inline void Registry::setFormat(const std::string& logger_name, LogFormat::ptr new_format)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto found = m_loggerMap.find(logger_name);
    if (found == m_loggerMap.end())
    {
        return;
    }
    found->second->setFormat(std::move(new_format));
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
inline void Registry::setDefaultLogger(Logger::ptr new_default_logger)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (new_default_logger != nullptr) 
    {
        m_loggerMap[new_default_logger->getName()] = new_default_logger;
    }
    m_default_logger = std::move(new_default_logger);
}
inline void Registry::setDefaultFormat(LogFormat::ptr new_default_format)
{
    m_default_format = std::move(new_default_format);
    setFormat("quicklog", m_default_format->clone());

}
inline void Registry::setDefaultLevel(LogLevel new_default_level)
{
    m_default_level = new_default_level;   
    setLevel("quicklog", m_default_level);
}
inline void Registry::setDefaultAutoLevel(LogLevel new_default_auto_level)
{
    m_default_auto_level = new_default_auto_level;
    setAutoLevel("quicklog", m_default_auto_level);
}
inline void Registry::setDefaultPattern(const std::string pattern)
{
    setDefaultFormat(std::make_unique<LogFormat>(pattern));
}
inline void Registry::setAllFormat(LogFormat::ptr new_all_format)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_loggerMap) 
    {
        iter.second->setFormat(new_all_format->clone());
    }
}
inline void Registry::setAllLevel(LogLevel new_all_level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_loggerMap)
    {
        iter.second->setLevel(new_all_level);
    }
}
inline void Registry::setAllAutoLevel(LogLevel new_all_auto_level)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    for (auto& iter : m_loggerMap)
    {
        iter.second->setAutoLevel(new_all_auto_level);
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

}//namespace quicklog