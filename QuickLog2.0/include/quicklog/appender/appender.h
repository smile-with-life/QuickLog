#pragma once
#include <quicklog/common.h>

//日志输出基类
namespace quicklog{
class Appender
{
public:
    using ptr = std::shared_ptr<Appender>;
public:
    virtual ~Appender() = default;
public:
    virtual void log(const log_msg& logMessage) = 0;
    virtual void log(const std::string& content) = 0;
    virtual void flush() = 0;
    virtual void setFormat(LogFormat::ptr format) = 0;
    virtual void setPattern(const std::string& pattern) = 0;
};
}