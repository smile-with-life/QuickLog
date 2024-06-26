#pragma once
#include<memory>
#include<thread>
#include <sstream>
#include<ctime>
#include"LogLevel.h"

#include<iostream>
class Loger;
// 日志事件
class LogEvent
{
public:
    using ptr = std::shared_ptr<LogEvent>;
private:
    std::string m_Format;//日志格式

    time_t m_Time{};//时间戳
    std::string m_Filename{};//文件名
    std::thread::id m_ThreadId{};//线程Id
    LogLevel m_Level = LogLevel::Debug;// 日志等级
    uint32_t m_Line{};//行号
    uint64_t m_Elapse{};//程序运行时间   
    std::string m_Content{};// 日志内容  

public:
    LogEvent(std::string format, time_t time \
        , std::string filename, std::thread::id threadId, LogLevel level \
        , uint32_t line, uint32_t elapse, std::string content)
        : m_Format(format), m_Time(time), m_Filename(filename), m_ThreadId(threadId)
        , m_Level(level), m_Line(line), m_Elapse(elapse), m_Content(content)
    {};
    ~LogEvent() {};
    LogEvent(const LogEvent&) = delete;
    LogEvent& operator=(const LogEvent&) = delete;
    LogEvent(LogEvent&&) = delete;
    LogEvent& operator=(LogEvent&&) = delete;
public:
    std::string getTime();
    std::string &getFile(); 
    std::string getThreadId();
    std::string getLevel();
    uint32_t &getLine();
    uint64_t &getElapse();
    std::string &getContent();
    std::string& getFormat();
};

std::string LogEvent::getTime()
{
    struct tm Tm;
    localtime_s(&Tm, &m_Time);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &Tm);
    return buf;
}
std::string &LogEvent::getFile() 
{
    return m_Filename;
}
std::string LogEvent::getThreadId() 
{
    std::stringstream ss;
    ss << m_ThreadId;
    return ss.str();
}
std::string LogEvent::getLevel() 
{
    switch (m_Level)
    {
    case LogLevel::All:
    {
        return "All";
        break;
    }
    case LogLevel::Trace:
    {
        return "Trace";
        break;
    }
    case LogLevel::Debug:
    {
        return "Debug";
        break;
    }
    case LogLevel::Info:
    {
        return "Info";
        break;
    }
    case LogLevel::Warning:
    {
        return "Warning";
        break;
    }
    case LogLevel::Error:
    {
        return "Error";
        break;
    }
    case LogLevel::Fatal:
    {
        return "Fatal";
        break;
    }
    case LogLevel::Off:
    {
        return "Off";
        break;
    }
    default:
    {
        //抛出异常
        return "None";
    }
    }

}
uint32_t &LogEvent::getLine() 
{
    return m_Line;
}
std::string &LogEvent::getContent() 
{
    return m_Content;
}
uint64_t &LogEvent::getElapse() 
{
    return m_Elapse;
}
std::string& LogEvent::getFormat() 
{
    return m_Format;
}


