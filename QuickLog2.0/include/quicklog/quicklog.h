#pragma once

#include<quicklog/details/registry.h>

namespace quicklog {
static Registry registry;
template<typename T>
void Trace(const T& message \
    , std::thread::id threadId = std::this_thread::get_id() \
    , std::source_location location = std::source_location::current())
{
    registry.get_default_raw()->Trace(message, threadId, location);
}
template<typename T>
void Debug(const T& message \
    , std::thread::id threadId = std::this_thread::get_id() \
    , std::source_location location = std::source_location::current())
{
    registry.get_default_raw()->Debug(message, threadId, location);
}
template<typename T>
void Info(const T& message \
    , std::thread::id threadId = std::this_thread::get_id() \
    , std::source_location location = std::source_location::current())
{
    registry.get_default_raw()->Info(message, threadId, location);
}
template<typename T>
void Warning(const T& message \
    , std::thread::id threadId = std::this_thread::get_id() \
    , std::source_location location = std::source_location::current())
{
    registry.get_default_raw()->Warning(message, threadId, location);
}
template<typename T>
void Error(const T& message \
    , std::thread::id threadId = std::this_thread::get_id() \
    , std::source_location location = std::source_location::current())
{
    registry.get_default_raw()->Error(message, threadId, location);
}
template<typename T>
void Fatal(const T& message \
    , std::thread::id threadId = std::this_thread::get_id() \
    , std::source_location location = std::source_location::current())
{
    registry.get_default_raw()->Fatal(message, threadId, location);
}
template<typename... Args>
void Trace(std::format_string<Args...> format, Args &&...args)
{
    registry.get_default_raw()->Trace(format, std::forward<Args>(args)...);
}
template<typename... Args>
void Debug(std::format_string<Args...> format, Args &&...args)
{
    registry.get_default_raw()->Debug(format, std::forward<Args>(args)...);
}
template<typename... Args>
void Info(std::format_string<Args...> format, Args &&...args)
{
    registry.get_default_raw()->Info(format, std::forward<Args>(args)...);
}
template<typename... Args>
void Warning(std::format_string<Args...> format, Args &&...args)
{
    registry.get_default_raw()->Warning(format, std::forward<Args>(args)...);
}
template<typename... Args>
void Error(std::format_string<Args...> format, Args &&...args)
{
    registry.get_default_raw()->Error(format, std::forward<Args>(args)...);
}
template<typename... Args>
void Fatal(std::format_string<Args...> format, Args &&...args)
{
    registry.get_default_raw()->Fatal(format, std::forward<Args>(args)...);
}

Logger::ptr create(const std::string& logger_name)
{
    return registry.init_logger(logger_name);
}

template<typename AppenderType>
Logger::ptr create(const std::string& logger_name,const std::string& fileName)
{
    return registry.init_logger<AppenderType>(logger_name, fileName);
}

template<typename AppenderType>
Logger::ptr create(const std::string& logger_name, ColorMode colorMode)
{
    return registry.init_logger<AppenderType>(logger_name, colorMode);
}

Logger::ptr get(const std::string& logger_name)
{
    return registry.get(logger_name);
}





}//namespace quicklog