#pragma once
#include <quicklog/appender/base_appender.h>
#include <quicklog/details/null_mutex.h>
#include <quicklog/common.h>

namespace quicklog {
//控制台输出
template<typename Mutex>
class Console_Appender final :public Base_Appender<Mutex>
{
private:
    ConsoleMode m_consoleMode;
    ColorMode m_colorMode;
public:
    Console_Appender();

    explicit Console_Appender(ColorMode colorMode);

    Console_Appender(ColorMode colorMode, LogFormat::ptr format);
public:
    void setColorMode(ColorMode colorMode);

    void setConsoleMode(ConsoleMode consoleMode);
protected:
    void print(const log_msg& logMessage) override;

    void print(const std::string& content) override;

    void native_flush() override;

};
using console_mt = Console_Appender<std::mutex>;
using console_st = Console_Appender<null_mutex>;
}
#include"console_appender-inl.h"