#pragma once

#include<quicklog/logger.h>

namespace quicklog 
{
class ThreadPool;
class AsyncLogger final: public std::enable_shared_from_this<AsyncLogger>, public Logger
{
    friend class ThreadPool;
public:
    AsyncLogger(std::weak_ptr<ThreadPool> thread_pool, std::string name);
    AsyncLogger(std::weak_ptr<ThreadPool> thread_pool, std::string name, LogLevel level);
    AsyncLogger(std::weak_ptr<ThreadPool> thread_pool, std::string name, LogLevel level, LogLevel auto_level);

    ~AsyncLogger() = default;

protected:
    void native_log(const logMsg& msg) override;
    void native_flush() override;
    void async_log(const logMsg& msg);
    void async_flush();
private:
    std::weak_ptr<ThreadPool>  m_thread_pool;
};
}//namespace quicklog

#include "asynclogger-inl.h"