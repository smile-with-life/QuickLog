#pragma once

#include <quicklog/asynclogger.h>
#include <mutex>
#include <queue>

namespace quicklog
{
class ThreadPool
{
public:
    ThreadPool(unsigned int thread_num = 4);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
public:
    static std::shared_ptr<ThreadPool> get_ptr(unsigned int num = 4);

    void start();

    void stop();

    void submit_msg(std::shared_ptr<AsyncLogger>&& logger, AsyncMsgType type, const logMsg& log_msg);

    void submit_flush(std::shared_ptr<AsyncLogger>&& logger, AsyncMsgType type);

private:
    void threadFunction();


private:
    std::mutex m_mtx;
    std::condition_variable m_cv;
    std::atomic_bool m_runFlag;//线程池的启动状态
    std::queue<asyncMsg> m_queue;//任务队列
    std::vector<std::thread> m_threadList;//线程列表
    unsigned int m_coreThread;//核心线程数量
    unsigned int m_maxThread;//最大线程数量
    std::atomic_uint m_curThread;//当前线程数量
    std::atomic_uint m_idleThread;//空闲线程数量
};
}//namespace quicklog

#include"threadpool-inl.h"