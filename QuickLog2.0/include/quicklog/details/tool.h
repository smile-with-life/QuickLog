#pragma once
#include <quicklog/common.h>
#include <string>
#include <thread>
#include <sstream>
#include <fstream>
#include <source_location>
#include <filesystem> 



namespace quicklog{
//返回一个表示日志等级的带颜色的字符串(throw）
std::string strLogColorLevel(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Trace:
    {
        return "\033[37mTrace\033[0m";
        break;
    }
    case LogLevel::Debug:
    {
        return "\033[32mDebug\033[0m";
        break;
    }
    case LogLevel::Info:
    {
        return "\033[34mInfo\033[0m";
        break;
    }
    case LogLevel::Warning:
    {
        return "\033[33mWarning\033[0m";
        break;
    }
    case LogLevel::Error:
    {
        return "\033[31mError\033[0m";
        break;
    }
    case LogLevel::Fatal:
    {
        return "\033[35mFatal\033[0m";
        break;
    }
    default:
    {
        throw(log_exception("传入了一个错误的日志等级——logger.h文件被修改"));
    }   
    }
}
//返回一个表示日志等级的字符串(throw）
std::string strLogLevel(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Trace:
    {
        return "Trace";
        break;
    }
    case LogLevel::Debug:
    {
        return "Debug";
        break;
    }
    case LogLevel::Info:
    {
        return "Info";
        break;
    }
    case LogLevel::Warning:
    {
        return "Warning";
        break;
    }
    case LogLevel::Error:
    {
        return "Error";
        break;
    }
    case LogLevel::Fatal:
    {
        return "Fatal";
        break;
    }
    default:
    {
        throw(log_exception("传入了一个错误的日志等级——logger.h文件被修改"));
    }
    }

}
//检查文件是否存在
bool check_file(const std::string& fileName)
{
    std::ifstream ifs(fileName);
    int state = ifs.good();
    ifs.close();
    return state;
}
//获取文件所在目录
std::string getFilePath(const std::string& fileName)
{
    size_t pos = fileName.find_last_of('/');
    if (pos == std::string::npos)
    {
        pos = fileName.find_last_of('\\');
        if (pos == std::string::npos)
        {
            return std::string("");
        }
        else
        {
            return fileName.substr(0, pos);
        }
    }
    else
    {
        return fileName.substr(0, pos);
    }
}
//创建一个文件
bool create_file(const std::string& fileName)
{
    std::ofstream ofs(fileName);
    bool state = ofs.is_open();
    ofs.close();
    return state;
}
//创建文件夹
bool create_dir(const std::string& dirName)
{
    if (dirName.empty()) 
    {
        return false;
    }
    std::filesystem::path path(dirName);
    return std::filesystem::create_directories(path);
}
//获取一个可用文件
bool getFile(const std::string& fileName)
{
    if(check_file(fileName))
    {
        return true;//若文件存在直接返回
    }
    else
    {
        if (create_file(fileName))
        {
            return true;
        }
        else
        {
            //文件创建失败，创建该文件路径的所有目录
            if (create_dir(getFilePath(fileName)))
            {
                create_file(fileName);
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
//获得文件大小，单位字节
size_t getFileSize(const std::string& fileName)
{
    if (fileName.empty()) 
    {
        return 0;
    }
    // 存储文件(夹)信息的结构体，其中有文件大小和创建时间、访问时间、修改时间等
    struct stat statbuf;
    // 提供文件名字符串，获得文件属性结构体
    stat(fileName.c_str(), &statbuf);
    // 获取文件大小
    return statbuf.st_size;
}
//分割目录，返回文件名
std::string split_floder(const std::string& fileName)
{
    auto folder_pos = fileName.find_last_of('\\');
    if (folder_pos == std::string::npos)
    {
        folder_pos = fileName.find_last_of('/');
        if (folder_pos == std::string::npos)
        {
            return fileName;
        }
        else
        {
            return fileName.substr(folder_pos + 1);
        }        
    }
    else
    {
        return fileName.substr(folder_pos + 1);
    }
}

}//namespace quicklog
