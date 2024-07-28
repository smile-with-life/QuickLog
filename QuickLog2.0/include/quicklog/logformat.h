#pragma once

#include <memory>
#include <string>
#include <format>

#include <quicklog/common.h>
#include <quicklog/details/log_msg.h>
#include <quicklog/details/timepoint.h>

// 日志格式
// {0} 时间   
// {1} 文件名 
// {2} 函数名
// {3} 线程Id
// {4} 日志级别
// {5} 行号
// {6} 消息
namespace quicklog{
class LogFormat
{
public:
    using ptr = std::unique_ptr<LogFormat>;
private:
#ifdef USE_RUN_FORMAT
    std::string m_pattern;//格式模板
#else

#endif 
    TimeType m_timeType;//时间类型
    std::chrono::time_point<std::chrono::system_clock> m_lastTime;//上次被调用的时间
    std::string m_strTime;//时间字符串
    //格式化设置
public:
    LogFormat(TimeType timeType);

    ~LogFormat() = default;
    LogFormat(const LogFormat&) = delete;
    LogFormat& operator=(const LogFormat&) = delete;
public:
#ifdef USE_RUN_FORMAT
    const std::string& getPattern();

    void setPattern(const std::string& pattern);

    void format(const log_msg& logMessage, std::string& content);

    void format(const log_msg& logMessage, std::string& content, ColorMode mode);
#else
    void format(const log_msg& logMessage, std::string& content);

    void format(const log_msg& logMessage, std::string& content, ColorMode mode);

#endif
    void setTimeType(TimeType timeType);
private:
    void set_strTime(const TimePoint& timePoint);

    void get_color(LogLevel level,std::string& str);

    const char* get_strLevel(LogLevel level);

    const char* get_strColorLevel(LogLevel level);
};
}

#include"logformat-inl.h"