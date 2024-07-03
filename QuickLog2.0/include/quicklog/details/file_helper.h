#pragma once
#include <string>
#include <fstream>

//#define USE_FILE_CPP
namespace quicklog{
//文件助手
class file_helper
{
private:
    const int m_tries = 5; // 尝试打开文件时应该重试的次数
    const unsigned int m_interval = 1000;// 每次尝试打开文件之间的时间间隔ms
#ifdef USE_FILE_CPP
    std::ofstream m_ofs;//文件流
#else
    std::FILE* m_pfile{ nullptr };

#endif
    std::string m_fileName;//文件名
public:
    file_helper() = default;

    explicit file_helper(std::string fileName);

    ~file_helper();

    file_helper(const file_helper&) = delete;
    file_helper& operator=(const file_helper&) = delete;
    

    void open(const std::string fileName);

    void reopen();

    void flush();

    void close();

    void write(const std::string& buf);

    size_t fileSize() const;

    const std::string& fileName() const;

    static std::tuple<std::string, std::string> split_EXT(const std::string& fileName);//拆分扩展名
};

}//namespace quicklog

#include "file_helper-inl.h" 