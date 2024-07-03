#pragma once
#include"console_appender.h"
namespace quicklog {
template<typename Mutex>
inline Console_Appender<Mutex>::Console_Appender()
    : Base_Appender<Mutex>()
    , m_consoleMode(ConsoleMode::stdoutConsole)
    , m_colorMode(ColorMode::automatic)
{}

template<typename Mutex>
inline Console_Appender<Mutex>::Console_Appender(ColorMode colorMode)
    : Base_Appender<Mutex>()
    , m_consoleMode(ConsoleMode::stdoutConsole)
    , m_colorMode(colorMode)
{}

template<typename Mutex>
inline Console_Appender<Mutex>::Console_Appender(ColorMode colorMode, LogFormat::ptr format)
    : Base_Appender<Mutex>(std::move(format))
    , m_consoleMode(ConsoleMode::stdoutConsole)
    , m_colorMode(colorMode)
{}

template<typename Mutex>
inline void Console_Appender<Mutex>::setColorMode(ColorMode colorMode)
{
    m_colorMode = colorMode;
}

template<typename Mutex>
inline void Console_Appender<Mutex>::setConsoleMode(ConsoleMode consoleMode)
{
    m_consoleMode = consoleMode;
}

template<typename Mutex>
inline void Console_Appender<Mutex>::print(const log_msg& logMessage)
{
    std::string content;
    this->m_format->format(logMessage, content, m_colorMode);
    switch (m_consoleMode)
    {
    case ConsoleMode::stdoutConsole:
    {
        std::cout << content;
        break;
    }
    case ConsoleMode::stderrConsole:
    {
        std::cerr << content;
        break;
    }
    }//switch  
}

template<typename Mutex>
inline void Console_Appender<Mutex>::print(const std::string& content)
{
    switch (m_consoleMode)
    {
    case ConsoleMode::stdoutConsole:
    {
        std::cout << content;
        break;
    }
    case ConsoleMode::stderrConsole:
    {
        std::cerr << content;
        break;
    }
    }//switch
}

template <typename Mutex>
inline void Console_Appender<Mutex>::native_flush()
{
    std::cout.flush();
    std::cerr.flush();
}


}