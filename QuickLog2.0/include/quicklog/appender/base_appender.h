#pragma once

#include <mutex>

#include <quicklog/appender/appender.h>

namespace quicklog {
template <typename Mutex>
class Base_Appender :public Appender
{
protected:
    LogFormat::ptr m_format;

    Mutex m_mutex;
public:
    Base_Appender();
    explicit Base_Appender(LogFormat::ptr format);
    virtual ~Base_Appender() override = default;

    Base_Appender(const Base_Appender&) = delete;
    Base_Appender(Base_Appender&&) = delete;

    Base_Appender& operator=(const Base_Appender&) = delete;
    Base_Appender& operator=(Base_Appender&&) = delete;
public:
    void log(const log_msg& logMessage) final;
    
    void log(const std::string& content) final;

    void flush() final;

    void setFormat(LogFormat::ptr format) final;

    void setPattern(const std::string& pattern) final;
protected:
    virtual void print(const log_msg& logMessage) = 0;

    virtual void print(const std::string& content) = 0;

    virtual void native_flush() = 0;

};

}//namespace quicklog

#include"base_appender-inl.h"