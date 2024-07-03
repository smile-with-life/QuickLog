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
// 默认:"{0} {1} thread[{3}] [{4}] line[{5}] message[{6}] "
// 2024-04-14 15:24:57  QuickLog.h main（） thread[23904] [Debug] line[72] message[hello] 
namespace quicklog{
class LogFormat
{
public:
    using ptr = std::unique_ptr<LogFormat>;
private:
    std::string m_pattern;//格式模板
    std::string m_color_pattern;//带颜色的格式模板
    TimeType m_timeType;//时间类型
    std::chrono::time_point<std::chrono::system_clock> m_lastTime;//上次被调用的时间
    std::string m_strTime;//时间字符串
    //格式化设置
public:
    LogFormat();
    explicit LogFormat(const std::string& pattern);
    LogFormat(const std::string& pattern, TimeType timeType);

    ~LogFormat() = default;
    LogFormat(const LogFormat&) = delete;
    LogFormat& operator=(const LogFormat&) = delete;
public:
    LogFormat::ptr clone();

    const std::string& getPattern();

    void setPattern(const std::string& pattern);

    void format(const log_msg& logMessage, std::string& content);

    void format(const log_msg& logMessage, std::string& content, ColorMode mode);
private:

    std::string get_strTime(TimeType timeType,const TimePoint& timePoint);

    void get_color(LogLevel level,std::string& str);
};
}

#include"logformat-inl.h"