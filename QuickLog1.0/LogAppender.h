#pragma once
#include<memory>
#include<iostream>
#include<format>
#include <fstream>
#include<mutex>
#include"LogEvent.h"


//日志输出基类
class LogAppender
{
public:
    using ptr = std::shared_ptr<LogAppender>;
public:
    LogAppender() = default;
    virtual ~LogAppender() = default;

    virtual void log(LogEvent::ptr event) = 0;
public:
    std::mutex m_mutex;
};

//输出到控制台
class ConsoleAppender : public LogAppender
{
public:
    using ptr = std::shared_ptr<ConsoleAppender>;
public:
    static ConsoleAppender::ptr getInstance();
    void log(LogEvent::ptr event) override;
};

//输出到文件
class FileAppender : public LogAppender
{
public:
    using ptr = std::shared_ptr<FileAppender>;   
private:
    std::string m_Filename{};// 文件路径   
    std::ofstream m_Filestream{};// 文件流
    uint64_t m_LastTime{}; //上次文件打开时间
public:
    FileAppender(const std::string& filename);
    ~FileAppender();
    bool reopen();
public:
    void log(LogEvent::ptr event) override;
};


//ConsoleAppender实现
ConsoleAppender::ptr ConsoleAppender::getInstance()
{
    static ConsoleAppender::ptr instance = std::make_shared<ConsoleAppender>();
    return instance;
}
void ConsoleAppender::log(LogEvent::ptr event)
{    
    std::lock_guard<std::mutex> lock(m_mutex);
    std::cout << std::vformat(std::string_view(event->getFormat())
        , std::make_format_args(event->getTime(), event->getFile() \
            , event->getThreadId(), event->getLevel(), event->getLine() \
            , event->getContent(), event->getElapse()));
}

//FileAppender实现
FileAppender::FileAppender(const std::string& filename)
    : LogAppender()
    , m_Filename(filename)
    , m_Filestream(m_Filename, std::ios_base::out)
    , m_LastTime(time(nullptr))
{
    reopen();
}
void FileAppender::log(LogEvent::ptr event) 
{  
    uint64_t nowTime = time(nullptr);
    if (nowTime >= (m_LastTime + 5)) //每5秒进行一次
    {
        reopen();//重新打开文件
        m_LastTime = nowTime;//记录上次重新打开时间
    }
    std::lock_guard<std::mutex> lock(m_mutex);
    m_Filestream << std::vformat(std::string_view(event->getFormat())
        , std::make_format_args(event->getTime(), event->getFile() \
            , event->getThreadId(), event->getLevel(), event->getLine() \
            , event->getContent(), event->getElapse()));       
}
bool FileAppender::reopen() { 
    if (m_Filestream.is_open()) 
    {
        m_Filestream.close();
    }
    m_Filestream.open(m_Filename, std::ios_base::app);
    return m_Filestream.is_open();
}

FileAppender::~FileAppender()
{
    if (m_Filestream.is_open())
    {
        m_Filestream.close();
    }
}