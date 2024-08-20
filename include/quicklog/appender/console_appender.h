#pragma once
#include <quicklog/appender/appender.h>
#include <quicklog/common.h>
#include <mutex>

namespace quicklog {
//控制台输出
class Console_Appender :public Appender
{
public:
    Console_Appender(ColorMode colorMode = ColorMode::automatic);

    virtual ~Console_Appender() = default;
public:
    void setColorMode(ColorMode colorMode) noexcept;

    void log(const logMsg& logMessage) final;

    void flush() final;
private:
protected:
    virtual void native_log(const logMsg& log_msg) = 0;

    virtual void native_flush() = 0;
protected:
    LogFormat_ptr m_format;
    std::mutex m_mutex;
    ColorMode m_color_mode;
};

}
#include"console_appender-inl.h"