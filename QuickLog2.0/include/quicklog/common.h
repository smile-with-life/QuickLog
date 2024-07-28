#pragma once

#include<exception>
#include<quicklog/config.h>

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
    Null,     //日志等级为空
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


/* 控制台模式 */
enum class ConsoleMode
{
    stdoutConsole,
    stderrConsole
    //...
};

/* 时间类型 */
enum class TimeType 
{
    Local,  // 本地时间
    UTC     // UTC时间
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

}//namespace quicklog


