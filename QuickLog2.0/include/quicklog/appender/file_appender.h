#pragma once

#include <mutex>
#include <string>

#include <quicklog/details/null_mutex.h>
#include <quicklog/appender/base_appender.h>
#include <quicklog/details/file_api.h>


namespace quicklog {
//txt文件输出
template <typename Mutex>
class File_Appender final : public Base_Appender<Mutex> 
{
public:
    explicit File_Appender(std::string filePath);
public:
    std::string getFileName();

    std::string getFilePath();
protected:
    void print(const log_msg& logMessage) override;

    void print(const std::string& content) override;

    void native_flush() override;

private:
    File_API m_file;
};

using file_mt = File_Appender<std::mutex>;
using file_st = File_Appender<null_mutex>;

}//namespace quicklog

#include "file_appender-inl.h"
