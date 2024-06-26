#pragma once
#include<memory>
#include <string>
// 日志格式
// {0} 时间   
// {1} 文件名 
// {2} 线程Id
// {3} 日志级别
// {4} 行号
// {5} 消息
// {6} 程序运行时间
// 默认:"{0} {1} thread[{2}] [{3}] line[{4}] message[{5}] elapse[{6}]"
// 2024-04-14 15:24:57  QuickLog.h  thread[23904] [Debug] line[72] message[hello] elapse[1500]
class LogFormat
{
public:
    using ptr = std::shared_ptr<LogFormat>;
private:
    std::string m_pattern;//格式模板
public:
    LogFormat() :m_pattern("{0} {1} thread[{2}] [{3}] line[{4}] message[{5}] elapse[{6}]\n") 
    {};
    LogFormat(const std::string pattern) :m_pattern(pattern+"\n") 
    {};
    ~LogFormat() {};
    LogFormat(const LogFormat&) = delete;
    LogFormat& operator=(const LogFormat&) = delete;
    LogFormat(LogFormat&&) = delete;
    LogFormat& operator=(LogFormat&&) = delete;
public:
    std::string& getPattern();

    void setPattern(const std::string pattern);
};

//实现
std::string& LogFormat::getPattern()
{
    return m_pattern;
}
void LogFormat::setPattern(const std::string pattern)
{
    m_pattern = pattern + "\n";
}
