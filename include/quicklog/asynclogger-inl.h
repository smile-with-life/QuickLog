#pragma once

#include <quicklog/asynclogger.h>
#include <quicklog/details/asyncmsg.h>
#include <quicklog/details/threadpool.h>

namespace quicklog
{
inline AsyncLogger::AsyncLogger(std::weak_ptr<ThreadPool> thread_pool, std::string name)
    : Logger(name)
    , m_thread_pool(thread_pool)
{}

inline AsyncLogger::AsyncLogger(std::weak_ptr<ThreadPool> thread_pool, std::string name, LogLevel level)
    : Logger(name,level)
    , m_thread_pool(thread_pool)
{}

inline AsyncLogger::AsyncLogger(std::weak_ptr<ThreadPool> thread_pool, std::string name, LogLevel level, LogLevel auto_flush_level)
    : Logger(name, level, auto_flush_level)
    , m_thread_pool(thread_pool)
{}

inline void AsyncLogger::native_log(const logMsg& log_msg)
{
    if(auto pool_ptr = m_thread_pool.lock())
    {
        pool_ptr->submit_msg(shared_from_this(), AsyncMsgType::log, log_msg);
    }
    else
    {
        throw(log_exception("async log: thread pool doesn't exist"));
    }
}

inline void AsyncLogger::native_flush()
{
    if (auto pool_ptr = m_thread_pool.lock())
    {
        pool_ptr->submit_flush(shared_from_this(), AsyncMsgType::flush);
    }
    else
    {
        throw(log_exception("async flush: thread pool doesn't exist"));
    }
}

inline void AsyncLogger::async_log(const logMsg& log_msg)
{
    //std::cout << log_msg.m_message << std::endl;
    for (const auto& iter : m_appender_list)
    {
        iter->log(log_msg);
    }
    if (should_flush(log_msg.m_level))
    {
        async_flush();
    }
}

inline void AsyncLogger::async_flush()
{
    for (auto& iter : m_appender_list)
    {
        iter->flush();
    }
}

}//namespace quicklog