#pragma once
#include "txt_appender.h"

namespace quicklog {
template<typename Mutex>
inline TXT_Appender<Mutex>::TXT_Appender(std::string fileName)
    : Base_Appender<Mutex>()
    , m_file_helper(fileName)   
{
    try
    {
        m_file_helper.open(fileName);
    }
    catch (log_exception& exception)
    {
        std::cout << exception.what() << std::endl;
    }
    catch (...)
    {
        throw;
    }
}
template<typename Mutex>
inline TXT_Appender<Mutex>::TXT_Appender(std::string fileName, LogFormat::ptr format)
    : Base_Appender<Mutex>(std::move(format))
    , m_file_helper(fileName)
{
    try 
    {
        m_file_helper.open(fileName);
    }
    catch (log_exception& exception)
    {
        std::cout << exception.what() << std::endl;
    }
    catch (...)
    {
        throw;
    }   
}

template<typename Mutex>
inline std::string& TXT_Appender<Mutex>::getFileName()
{
    return m_file_helper.fileName();
}
template<typename Mutex>
inline void TXT_Appender<Mutex>::print(const log_msg& logMessage)
{
    std::string content;
    this->m_format->format(logMessage, content);
    m_file_helper.write(content);
}
template<typename Mutex>
inline void TXT_Appender<Mutex>::print(const std::string& content)
{
    m_file_helper.write(content);
}
template <typename Mutex>
inline void TXT_Appender<Mutex>::native_flush()
{
    m_file_helper.flush();
}


}//namespace quicklog
