#pragma once
#include"console_appender.h"
namespace quicklog {

inline Console_Appender::Console_Appender(ColorMode color_mode)
    : m_format(std::make_unique<LogFormat>())
    , m_color_mode(color_mode)
{}

inline void Console_Appender::setColorMode(ColorMode color_mode) noexcept
{
    m_color_mode = color_mode;
}

inline void Console_Appender::log(const logMsg& log_msg)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    native_log(log_msg);
}

inline void Console_Appender::flush()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    native_flush();
}

}//namespace quicklog