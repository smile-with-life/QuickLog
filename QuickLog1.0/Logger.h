#pragma once
#include<list>
#include<memory>
#include<mutex>
#include<thread>
#include<source_location>
#include"LogAppender.h"
#include"LogEvent.h"
#include"LogFormat.h"
#include"LogLevel.h"

// 日志器
class Logger 
{
public:
    using ptr = std::shared_ptr<Logger>;
private:
    LogLevel m_Level ;//日志级别
    std::shared_ptr<LogFormat> m_Format;//日志格式
    std::list<LogAppender::ptr> m_AppenderList;//日志输出列表
public:
    Logger()
        : m_Level(LogLevel::All)
        , m_Format(std::make_shared<LogFormat>())
    {
        m_AppenderList.push_back(ConsoleAppender::getInstance());
    }
    explicit Logger(LogLevel level)
        : m_Level(level)
        , m_Format(std::make_shared<LogFormat>())
    {}
    explicit Logger(LogFormat::ptr format)
        : m_Level(LogLevel::All)
        , m_Format(format)
    {}
    explicit Logger(LogAppender::ptr appender)
        : m_Level(LogLevel::All)
        , m_Format(std::make_shared<LogFormat>())
    {
        m_AppenderList.push_back(appender);
    }
    Logger(LogLevel level, LogFormat::ptr format,LogAppender::ptr appender)
        : m_Level(level)
        , m_Format(format)
    {
        m_AppenderList.push_back(appender);
    }
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

public:
    static Logger::ptr getInstance();
    
    LogLevel getLevel();

    std::string& getFormat();

    void setLevel(LogLevel level);    

    void setFormat(std::string format);

    void addAppender(LogAppender::ptr appender);

    void clearAppender();

    void Trace(std::string message \
        , std::thread::id threadId = std::this_thread::get_id()\
        , std::source_location location = std::source_location::current());
    
    void Debug(std::string message \
        , std::thread::id threadId = std::this_thread::get_id() \
        , std::source_location location = std::source_location::current());
    
    void Info(std::string message \
        , std::thread::id threadId = std::this_thread::get_id()\
        , std::source_location location = std::source_location::current());
    
    void Warning(std::string message \
        , std::thread::id threadId = std::this_thread::get_id()\
        , std::source_location location = std::source_location::current());
    
    void Error(std::string message \
        , std::thread::id threadId = std::this_thread::get_id()\
        , std::source_location location = std::source_location::current());
    
    void Fatal(std::string message \
        , std::thread::id threadId = std::this_thread::get_id()\
        , std::source_location location = std::source_location::current());

};


Logger::ptr Logger::getInstance()
{
    static Logger::ptr instance = std::make_shared<Logger>();
    return instance;
}

LogLevel Logger::getLevel()
{
    return m_Level;
}

void Logger::setLevel(LogLevel level)
{
    m_Level = level;
}

std::string& Logger::getFormat()
{
    return m_Format->getPattern();
}

void Logger::setFormat(std::string format)
{
    m_Format->setPattern(format);
}

void Logger::addAppender(LogAppender::ptr appender)
{
    for (auto& iter : m_AppenderList)
    {
        if (iter == appender)
        {
            std::cout << "重复包含";
            return;
        }
    }
    m_AppenderList.push_back(appender);
}

void Logger::clearAppender()
{
    m_AppenderList.clear();
}

void Logger::Trace(std::string message, std::thread::id threadId, std::source_location location)
{
    if (LogLevel::Trace >= m_Level)
    {
        //封装事件
        LogEvent::ptr event(std::make_shared<LogEvent>(m_Format->getPattern(), time(0), location.file_name(), threadId, LogLevel::Trace, location.line(), clock(), message));
        for (auto& iter : m_AppenderList)
        {
            iter->log(event);
        }
    }

}

void Logger::Debug(std::string message, std::thread::id threadId, std::source_location location)
{
    if (LogLevel::Debug >= m_Level)
    {
        //封装事件
        LogEvent::ptr event(std::make_shared<LogEvent>(m_Format->getPattern(), time(0), location.file_name(), threadId, LogLevel::Debug, location.line(), clock(), message));
        for (auto& iter : m_AppenderList)
        {
            iter->log(event);
        }
    }

}

void Logger::Info(std::string message, std::thread::id threadId, std::source_location location)
{
    if (LogLevel::Info >= m_Level)
    {
        //封装事件
        LogEvent::ptr event(std::make_shared<LogEvent>(m_Format->getPattern(), time(0), location.file_name(), threadId, LogLevel::Info, location.line(), clock(), message));
        for (auto& iter : m_AppenderList)
        {
            iter->log(event);
        }
    }
}

void Logger::Warning(std::string message, std::thread::id threadId, std::source_location location)
{
    if (LogLevel::Warning >= m_Level)
    {
        //封装事件
        LogEvent::ptr event(std::make_shared<LogEvent>(m_Format->getPattern(), time(0), location.file_name(), threadId, LogLevel::Warning, location.line(), clock(), message));
        for (auto& iter : m_AppenderList)
        {
            iter->log(event);
        }
    }
}

void Logger::Error(std::string message, std::thread::id threadId, std::source_location location)
{
    if (LogLevel::Error >= m_Level)
    {
        //封装事件
        LogEvent::ptr event(std::make_shared<LogEvent>(m_Format->getPattern(), time(0), location.file_name(), threadId, LogLevel::Error, location.line(), clock(), message));
        for (auto& iter : m_AppenderList)
        {
            iter->log(event);
        }
    }

}

void Logger::Fatal(std::string message, std::thread::id threadId, std::source_location location)
{
    if (LogLevel::Fatal >= m_Level)
    {
        //封装事件
        LogEvent::ptr event(std::make_shared<LogEvent>(m_Format->getPattern(), time(0), location.file_name(), threadId, LogLevel::Fatal, location.line(), clock(), message));
        for (auto& iter : m_AppenderList)
        {
            iter->log(event);
        }
    }

}