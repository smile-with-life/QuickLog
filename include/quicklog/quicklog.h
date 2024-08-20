#pragma once

#include<quicklog/details/registry.h>

namespace quicklog {
static Registry registry;


template<typename T>
void Trace(const T& message, std::source_location location = std::source_location::current())
{
    registry.default_raw()->Trace(message, std::move(location));
}

template<typename T>
void Debug(const T& message, std::source_location location = std::source_location::current())
{
    registry.default_raw()->Debug(message,std::move(location));
}

template<typename T>
void Info(const T& message, std::source_location location = std::source_location::current())
{
    registry.default_raw()->Info(message, std::move(location));
}

template<typename T>
void Warning(const T& message, std::source_location location = std::source_location::current())
{
    registry.default_raw()->Warning(message, std::move(location));
}

template<typename T>
void Error(const T& message, std::source_location location = std::source_location::current())
{
    registry.default_raw()->Error(message, std::move(location));
}

template<typename T>
void Fatal(const T& message, std::source_location location = std::source_location::current())
{
    registry.default_raw()->Fatal(message, std::move(location));
}

template<typename... Args>
void Trace(with_source_location<std::format_string<Args...>> with_format, Args &&...args)
{
    registry.default_raw()->Trace(with_format, std::forward<Args>(args)...);
}

template<typename... Args>
void Debug(with_source_location<std::format_string<Args...>> with_format, Args &&...args)
{
    registry.default_raw()->Debug(with_format, std::forward<Args>(args)...);
}

template<typename... Args>
void Info(with_source_location<std::format_string<Args...>> with_format, Args &&...args)
{
    registry.default_raw()->Info(with_format, std::forward<Args>(args)...);
}

template<typename... Args>
void Warning(with_source_location<std::format_string<Args...>> with_format, Args &&...args)
{
    registry.default_raw()->Warning(with_format, std::forward<Args>(args)...);
}

template<typename... Args>
void Error(with_source_location<std::format_string<Args...>> with_format, Args &&...args)
{
    registry.default_raw()->Error(with_format, std::forward<Args>(args)...);
}

template<typename... Args>
void Fatal(with_source_location<std::format_string<Args...>> with_format, Args &&...args)
{
    registry.default_raw()->Fatal(with_format, std::forward<Args>(args)...);
}

Logger_ptr create(const std::string& logger_name)
{
    return registry.init(logger_name);
}

template<typename Type, typename... Args>
inline Logger_ptr create(const std::string& logger_name, Args&&... args)
{
    return registry.init<Type>(logger_name, std::forward<Args>(args)...);
}

Logger_ptr create_async(std::weak_ptr<ThreadPool> thread_pool, const std::string& logger_name)
{
    return registry.init_async(thread_pool, logger_name);
}

template<typename Type, typename... Args>
inline Logger_ptr create_async(std::weak_ptr<ThreadPool> thread_pool, const std::string& logger_name, Args&&... args)
{
    return registry.init_async<Type>(thread_pool, logger_name, std::forward<Args>(args)...);
}

void enroll(Logger_ptr new_logger)
{
    registry.enroll(new_logger);
}

Logger_ptr default_ptr() noexcept
{
    return registry.default_ptr();
}

Logger* default_raw() noexcept
{
    return registry.default_raw();
}

template<typename Type,typename... Args>
Appender_ptr appender(Args&&... args) noexcept
{
    return std::make_shared<Type>(args...);
}
std::shared_ptr<ThreadPool> thread_pool(unsigned int num = 4) noexcept
{
    return ThreadPool::get_ptr(num);
}

}//namespace quicklog