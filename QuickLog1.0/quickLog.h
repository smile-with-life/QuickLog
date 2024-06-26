#pragma once
#include"LogLevel.h"
#include"LogEvent.h"
#include"LogAppender.h"
#include"LogFormat.h"
#include"Logger.h"
//c++20标准

// 日志级别数量
#define QUICKLOG_LEVEL_TOTAL    8   
// 版本号
#define QUICKLOG_VERSION        "1.0.1"

//标准方法调用一个全局静态logger进行日志打印
// 默认级别:All
// 默认格式:"{0} {1} thread[{2}] [{3}] line[{4}] message[{5}] elapse[{6}]"
// 默认输出: 控制台

//标准Trace方法 
void qTrace(std::string message \
    , std::thread::id threadId = std::this_thread::get_id() \
    , std::source_location location = std::source_location::current())
{
    Logger::getInstance()->Trace(message, threadId, location);
}
//标准Debug方法 
void qDebug(std::string message \
    , std::thread::id threadId = std::this_thread::get_id() \
    , std::source_location location = std::source_location::current())
{
    Logger::getInstance()->Debug(message, threadId, location);
}
//标准Info方法
void qInfo(std::string message \
    , std::thread::id threadId = std::this_thread::get_id() \
    , std::source_location location = std::source_location::current())
{
    Logger::getInstance()->Info(message, threadId, location);
}
//标准Warning方法
void qWarning(std::string message \
    , std::thread::id threadId = std::this_thread::get_id() \
    , std::source_location location = std::source_location::current())
{
    Logger::getInstance()->Warning(message, threadId, location);
}
//标准Error方法
void qError(std::string message \
    , std::thread::id threadId = std::this_thread::get_id() \
    , std::source_location location = std::source_location::current())
{
    Logger::getInstance()->Error(message, threadId, location);
}
//标准Fatal方法
void qFatal(std::string message \
    , std::thread::id threadId = std::this_thread::get_id() \
    , std::source_location location = std::source_location::current())
{
    Logger::getInstance()->Fatal(message, threadId, location);
}
//设置全局logger的级别
void setLocalLevel(LogLevel level)
{
    Logger::getInstance()->setLevel(level);
}
//获取全局logger的级别
LogLevel getLocalLevel(LogLevel level)
{
    return Logger::getInstance()->getLevel();
}
//设置全局logger的格式
void setLocalFormat(std::string format)
{
    Logger::getInstance()->setFormat(format);
}
//获取全局logger的格式
std::string getLocalFormat()
{
    return Logger::getInstance()->getFormat();
}
//设置全局logger的输出
void setLocalAppender(LogAppender::ptr appender)
{
    Logger::getInstance()->clearAppender();
    Logger::getInstance()->addAppender(appender);
}
//获取一个日志格式的智能指针
LogFormat::ptr initFormat(std::string pattern)
{
    return std::make_shared< LogFormat>(pattern);
}
//获取一个控制台输出的智能指针
ConsoleAppender::ptr initConsole()
{
    return ConsoleAppender::getInstance();
}
//获取一个文件输出的智能指针
FileAppender::ptr initFile(std::string filename)
{
     return std::make_shared<FileAppender>(filename);       
}

