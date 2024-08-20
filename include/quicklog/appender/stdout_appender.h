#pragma once

#include<quicklog/appender/console_appender.h>

namespace quicklog
{ 
class Stdout_Appender :public Console_Appender
{
public:
    explicit Stdout_Appender(ColorMode color_mode = ColorMode::automatic);

    ~Stdout_Appender() = default;

protected:
    void native_log(const logMsg& log_msg) override;

    void native_flush() override;   
};
class Stderr_Appender :public Console_Appender
{
public:
    explicit Stderr_Appender(ColorMode color_mode);

    ~Stderr_Appender() = default;

protected:
    void native_log(const logMsg& log_msg) override;

    void native_flush() override;
};
using stdout_mt = Stdout_Appender;
using stderr_mt = Stderr_Appender;
}//namespace quicklog

#include"stdout_appender-inl.h"