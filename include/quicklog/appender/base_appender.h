#pragma once

#include <mutex>

#include <quicklog/appender/appender.h>

namespace quicklog {
template <typename Mutex>
class Base_Appender :public Appender
{
protected:
    LogFormat_ptr m_format;

    Mutex m_mutex;
public:
    Base_Appender();
    virtual ~Base_Appender() override = default;

    Base_Appender(const Base_Appender&) = delete;
    Base_Appender(Base_Appender&&) = delete;

    Base_Appender& operator=(const Base_Appender&) = delete;
    Base_Appender& operator=(Base_Appender&&) = delete;
public:
    void log(const logMsg& log_msg) final;

    void flush() final;
#ifdef USE_RUN_FORMAT
    void setPattern(const std::string& pattern) final;

    std::string getPattern() final;
#endif
protected:
    virtual void native_log(const logMsg& log_msg) = 0;

    virtual void native_flush() = 0;

};

}//namespace quicklog

#include"base_appender-inl.h"