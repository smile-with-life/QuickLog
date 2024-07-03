#pragma once

#include <mutex>
#include <string>

#include <quicklog/details/null_mutex.h>
#include <quicklog/appender/base_appender.h>
#include <quicklog/details/file_helper.h>


namespace quicklog {
//txt文件输出
template <typename Mutex>
class TXT_Appender final : public Base_Appender<Mutex> 
{
public:
    explicit TXT_Appender(std::string fileName);

    TXT_Appender(std::string fileName, LogFormat::ptr format);
public:
    std::string& getFileName();
protected:
    void print(const log_msg& logMessage) override;

    void print(const std::string& content) override;

    void native_flush() override;

private:
    file_helper m_file_helper;
};

using txt_mt = TXT_Appender<std::mutex>;
using txt_st = TXT_Appender<null_mutex>;

//template <typename Factory = spdlog::synchronous_factory>
//inline std::shared_ptr<logger> basic_logger_mt(const std::string& logger_name,
//    const filename_t& filename,
//    bool truncate = false,
//    const file_event_handlers& event_handlers = {}) {
//    return Factory::template create<sinks::basic_file_sink_mt>(logger_name, filename, truncate,
//        event_handlers);
//}
//
//template <typename Factory = spdlog::synchronous_factory>
//inline std::shared_ptr<logger> basic_logger_st(const std::string& logger_name,
//    const filename_t& filename,
//    bool truncate = false,
//    const file_event_handlers& event_handlers = {}) {
//    return Factory::template create<sinks::basic_file_sink_st>(logger_name, filename, truncate,
//        event_handlers);
//}

}//namespace quicklog

#include "txt_appender-inl.h"
