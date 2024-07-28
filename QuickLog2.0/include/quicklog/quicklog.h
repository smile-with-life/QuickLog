#pragma once

#include<quicklog/details/registry.h>

namespace quicklog {
static Registry registry;
template<typename T>
void Trace(const T& message \
    , unsigned int threadId = getThreadId() \
    , std::source_location location = std::source_location::current())
{
    registry.default_logger()->Trace(message, threadId, std::move(location));
}
template<typename T>
void Debug(const T& message \
    , unsigned int threadId = getThreadId() \
    , std::source_location location = std::source_location::current())
{
    registry.default_logger()->Debug(message, threadId,std::move(location));
}
template<typename T>
void Info(const T& message \
    , unsigned int threadId = getThreadId() \
    , std::source_location location = std::source_location::current())
{
    registry.default_logger()->Info(message, threadId, std::move(location));
}
template<typename T>
void Warning(const T& message \
    , unsigned int threadId = getThreadId() \
    , std::source_location location = std::source_location::current())
{
    registry.default_logger()->Warning(message, threadId, std::move(location));
}
template<typename T>
void Error(const T& message \
    , unsigned int threadId = getThreadId() \
    , std::source_location location = std::source_location::current())
{
    registry.default_logger()->Error(message, threadId, std::move(location));
}
template<typename T>
void Fatal(const T& message \
    , unsigned int threadId = getThreadId() \
    , std::source_location location = std::source_location::current())
{
    registry.default_logger()->Fatal(message, threadId, std::move(location));
}
template<typename... Args>
void Trace(std::format_string<Args...> format, Args &&...args)
{
    registry.default_logger()->Trace(format, std::forward<Args>(args)...);
}
template<typename... Args>
void Debug(std::format_string<Args...> format, Args &&...args)
{
    registry.default_logger()->Debug(format, std::forward<Args>(args)...);
}
template<typename... Args>
void Info(std::format_string<Args...> format, Args &&...args)
{
    registry.default_logger()->Info(format, std::forward<Args>(args)...);
}
template<typename... Args>
void Warning(std::format_string<Args...> format, Args &&...args)
{
    registry.default_logger()->Warning(format, std::forward<Args>(args)...);
}
template<typename... Args>
void Error(std::format_string<Args...> format, Args &&...args)
{
    registry.default_logger()->Error(format, std::forward<Args>(args)...);
}
template<typename... Args>
void Fatal(std::format_string<Args...> format, Args &&...args)
{
    registry.default_logger()->Fatal(format, std::forward<Args>(args)...);
}

template<typename Type,typename... Args>
Appender::ptr initAppender(Args&&... args)
{
    return std::make_shared<Type>(args...);
}

}//namespace quicklog