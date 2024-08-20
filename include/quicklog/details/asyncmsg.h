#pragma once

#include <quicklog/details/logmsg.h>
#include<quicklog/asynclogger.h>
#include <memory>


namespace quicklog
{
class AsyncLogger;
enum class AsyncMsgType
{
    log,
    flush
};

class asyncMsg : public logMsg
{ 
public:
    asyncMsg() = default;
    asyncMsg(std::shared_ptr<AsyncLogger>&& logger, AsyncMsgType async_type, const logMsg& log_msg)
        : logMsg(log_msg)
        , m_logger(std::move(logger))
        , m_type(async_type)
        , m_buffer(log_msg.m_message.data())
    {
        update_string_view();
    }

    asyncMsg(std::shared_ptr<AsyncLogger>&& logger, AsyncMsgType async_type)
        : logMsg()
        , m_logger(std::move(logger))
        , m_type(async_type)
    {}

    ~asyncMsg() = default;



    asyncMsg(const asyncMsg& other)
        : logMsg(other)
        , m_logger(other.m_logger)
        , m_type(other.m_type)
        , m_buffer(other.m_buffer)
    {
        update_string_view();
    }

    asyncMsg& operator=(asyncMsg& other)
    {
        logMsg::operator=(other);
        m_logger = other.m_logger;
        m_type = other.m_type;
        m_buffer = other.m_buffer;
        update_string_view();
        return *this;
    }
    asyncMsg(asyncMsg&& other) noexcept
        : logMsg(other)
        , m_logger(std::move(other.m_logger))
        , m_type(other.m_type)
        , m_buffer(std::move(other.m_buffer))
    {
        update_string_view();
    }
    

    asyncMsg& operator=(asyncMsg&& other) noexcept
    {
        logMsg::operator=(other);
        m_logger = std::move(other.m_logger);
        m_type = other.m_type;
        m_buffer = std::move(other.m_buffer);
        update_string_view();
        return *this;
    }

    void update_string_view()
    {
        m_message = std::string_view(m_buffer.data(), m_message.size()+1);
    }

    std::shared_ptr<AsyncLogger> m_logger;
    AsyncMsgType m_type{ AsyncMsgType::log};
    std::string m_buffer;//内容缓存
};
}//namespace quicklog