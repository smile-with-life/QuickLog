#pragma once

#include <quicklog/config.h>
#include <exception>
#include <string>
#include <source_location>
#include <thread>
#include <chrono>
#include <type_traits>

/* 版本号 */
#define QUICKLOG_VER_MAJOR 2 //主版本号
#define QUICKLOG_VER_MINOR 1 //次版本号
#define QUICKLOG_VER_PATCH 0 //补丁版本号
#define QUICKLOG_TO_VERSION(major, minor, patch) (major * 10000 + minor * 100 + patch)
//QUICKLOG_VERSION为最终版本号
#define QUICKLOG_VERSION QUICKLOG_TO_VERSION(SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH)


/* 日志级别数量 */ 
#define QUICKLOG_LEVEL_TOTAL    8 

namespace quicklog{
/* 日志级别 */
enum class LogLevel : int
{
    All,      //所有：所有日志级别
    Trace,    //跟踪：指明程序运行轨迹
    Debug,    //调试：指明调试过程中的事件信息
    Info,     //信息：指明运行过程中的重要信息
    Warning,  //警告：指明可能潜在的危险状况
    Error,    //错误：指明错误事件
    Fatal,    //致命：指明非常严重的可能会导致应用终止执行错误事件
    Off       //关闭：最高级别，不打印日志
};

/* 控制台彩色打印模式 */
enum class ColorMode 
{ 
    always, 
    automatic, 
    never 
};

/* 时间类型 */
enum class TimeType 
{
    Local,  // 本地时间
    UTC     // UTC时间
};

struct logTime
{
    logTime() = default;
    logTime(TimeType time_type)
        : time(std::chrono::system_clock::now())
        , type(time_type)
    {}

    logTime(const logTime& other) = default;
    logTime& operator=(const logTime& other) = default;

    std::chrono::time_point<std::chrono::system_clock> time;
    TimeType type{TimeType::Local};
};

/* 日志异常处理 */
class log_exception : public std::exception {
public:
    explicit log_exception(std::string message)
        :m_message(std::move(message))
    {}

    const char* what() const noexcept override
    {
        return m_message.c_str();
    }

private:
    std::string m_message;
};

/* 模板参数检查 */
template<class T>
concept VaildString = requires(T value)
{
    { std::string_view(std::forward<T>(value)) } -> std::convertible_to<std::string_view>;
};

/* 包装代码位置信息 */
template<class T>
class with_source_location {
public:
    template<class U> requires std::convertible_to <const U&, std::basic_string_view<char>>
    consteval with_source_location(U&& inner, std::source_location location = std::source_location::current())
        : m_inner(std::forward<U>(inner))
        , m_location(location)
    {}

    constexpr with_source_location(const with_source_location& other)
        : m_inner(other.m_inner)
        , m_location(other.m_location)
    {}

    template<class U>
        requires std::convertible_to <const U&, std::basic_string_view<char>>
    with_source_location& operator=(const with_source_location<U>& other) {
        m_inner = other.m_inner;
        m_location = other.m_location;
        return *this;
    }

    constexpr T const& format() const {
        return m_inner;
    }

    constexpr std::source_location const& location() const {
        return m_location;
    }

private:
    T m_inner;
    std::source_location m_location;
};

/* 获取线程ID */
unsigned int getThreadId()
{
    thread_local auto threadId = std::this_thread::get_id();
    return *(unsigned int*)&threadId;
}
/* 前向声明 */
class LogFormat;
class Appender;
using Appender_ptr = std::shared_ptr<Appender>;
using LogFormat_ptr = std::unique_ptr<LogFormat>;
}//namespace quicklog


