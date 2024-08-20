#pragma once

#include <quicklog/common.h>
#include <quicklog/details/logmsg.h>


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
private:
#ifdef USE_RUN_FORMAT
    std::string m_pattern;//格式模板
#endif 
    std::chrono::seconds m_last_time;//上次被调用的时间
    std::string m_catched_time;//时间缓存
    //格式化设置
public:
    LogFormat();
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
    void format(const logMsg& log_msg, std::string& content);

#endif
private:
    void StrTime(const logTime& log_time) noexcept;
    const char* getStrLevel(LogLevel level);
    const char* getStrColorLevel(LogLevel level);
    void setColor(LogLevel level, std::string& str);
};
}//namespace quicklog

#include"logformat-inl.h"