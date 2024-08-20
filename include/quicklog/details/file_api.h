#pragma once
#include <string>
#include <fstream>
#include<filesystem>

//#define USE_FILE_CPP
namespace quicklog{
//文件助手
class File_API
{
private:
    const int m_tries = 5; // 尝试打开文件时应该重试的次数
    const unsigned int m_interval = 1000;// 每次尝试打开文件之间的时间间隔ms
    std::filesystem::path m_filePath;

#ifdef USE_FILE_CPP
    std::ofstream m_ofs;//文件流
#else
    std::FILE* m_pfile{ nullptr };
#endif
    
public:
    File_API() = default;

    explicit File_API(std::string path);

    ~File_API();

    File_API(const File_API&) = delete;
    File_API& operator=(const File_API&) = delete;
    
    void setFilePath(const std::string path);

    void open();

    void reopen();

    void flush();

    void close();

    bool copy(const std::string newPath);

    bool exists(const std::string newPath);

    void remove();

    void rename(const std::string newName);

    void write(const std::string& content);

    size_t size() const;

    std::string filePath() const;

    std::string fileDir() const;

    std::string fileName() const;

    std::string fileStem() const;

    std::string fileEXT() const;

};

}//namespace quicklog

#include "file_api-inl.h" 