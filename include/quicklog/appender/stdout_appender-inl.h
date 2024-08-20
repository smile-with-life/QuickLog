#pragma once

namespace quicklog
{
//Stdout_Appender
inline Stdout_Appender::Stdout_Appender(ColorMode color_mode)
    : Console_Appender(color_mode)
{}

inline void Stdout_Appender::native_log(const logMsg& log_msg)
{
    std::string content;
    content.reserve(256);
    log_msg.m_mode = m_color_mode;
    this->m_format->format(log_msg, content);
    std::cout << content;
}

inline void Stdout_Appender::native_flush()
{
    std::cout.flush();
}

//Stderr_Appender
inline Stderr_Appender::Stderr_Appender(ColorMode color_mode)
    : Console_Appender(color_mode)
{}

inline void Stderr_Appender::native_log(const logMsg& log_msg)
{
    std::string content;
    content.reserve(256);
    log_msg.m_mode = m_color_mode;
    this->m_format->format(log_msg, content);
    std::cerr << content;
}

inline void Stderr_Appender::native_flush()
{
    std::cerr.flush();
}
}