#pragma once

#include <quicklog/details/threadpool.h>

namespace quicklog{

inline ThreadPool::ThreadPool(unsigned int thread_num)
    : m_runFlag(false)
    , m_coreThread(thread_num)
    , m_maxThread(100)
{
    start();
}

inline ThreadPool::~ThreadPool()
{
    stop();
}

inline std::shared_ptr<ThreadPool> ThreadPool::get_ptr(unsigned int num)
{
    static std::shared_ptr<ThreadPool> tp_ptr = std::make_shared<ThreadPool>(num);
    return tp_ptr;
}

inline void ThreadPool::start()
{
    m_runFlag.store(true);
    for (unsigned int i = 0; i < m_coreThread; i++)
    {
        m_threadList.emplace_back(&ThreadPool::threadFunction,this);
    }
    m_curThread.store(m_coreThread);
    m_idleThread.store(m_curThread);
}

inline void ThreadPool::stop()
{
    while (!m_queue.empty());
    m_runFlag.store(false);
    m_cv.notify_all();
    for (auto& iter : m_threadList)
    {
        if (iter.joinable())
        {
            iter.join();
        }
    }
}

inline void ThreadPool::submit_msg(std::shared_ptr<AsyncLogger>&& logger, AsyncMsgType type, const logMsg& log_msg)
{   
    if (!m_runFlag.load())
        return;
    std::unique_lock<std::mutex> lock(m_mtx);
    m_queue.emplace(std::move(logger), type, log_msg);
    lock.unlock();
    m_cv.notify_one();
}

inline void ThreadPool::submit_flush(std::shared_ptr<AsyncLogger>&& logger, AsyncMsgType type)
{
    if (!m_runFlag.load())
        return;
    std::unique_lock<std::mutex> lock(m_mtx);
    m_queue.emplace(std::move(logger), type);
    m_cv.notify_one();
}

inline void ThreadPool::threadFunction()
{
    while (m_runFlag.load())
    {     
        std::unique_lock<std::mutex> lock(m_mtx);
        m_cv.wait(lock, [this](){return !m_runFlag.load() || !m_queue.empty(); });
        if (m_queue.empty())[[unlikely]]
        {
            break;
        }
        asyncMsg async_msg(std::move(m_queue.front()));
        this->m_queue.pop();
        lock.unlock();
        
        m_idleThread--;
        switch(async_msg.m_type)
        {
        case AsyncMsgType::log :
        {
            async_msg.m_logger->async_log(async_msg);
            break;
        }
        case AsyncMsgType::flush :
        {
            async_msg.m_logger->async_flush();
            break;
        }
        }
        m_idleThread++;       
    }
}

}//namespace quicklog