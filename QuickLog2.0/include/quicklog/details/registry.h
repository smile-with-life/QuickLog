#pragma once
#include<memory>
#include<mutex>
#include<unordered_map>

#include<quicklog/common.h>
#include<quicklog/logger.h>
#include<quicklog/logformat.h>
#include<quicklog/appender/console_appender.h>
#include<quicklog/appender/txt_appender.h>

namespace quicklog{

class Registry
{
private:
    std::mutex m_mutex;
    std::unordered_map<std::string, Logger::ptr> m_loggerMap;
    Logger::ptr m_default_logger;//"{0} {1} thread[{3}] [{4}] line[{5}] message[{6}] \n"
    LogFormat::ptr m_default_format;
    LogLevel m_default_level;
    LogLevel m_default_auto_level;
public:
    Registry();
    ~Registry();
    void register_logger(Logger::ptr new_logger);//注册logger
    void check_logger(std::string& logger_name);//进行检查，当发现logger已经注册过，抛出异常(throw)
    Logger::ptr init_logger(const std::string& logger_name);//默认初始化一个logger并注册
    template<typename Type>
    Logger::ptr init_logger(const std::string& logger_name, const  std::string& fileName);
    template<typename Type>
    Logger::ptr init_logger(const std::string& logger_name, ColorMode colorMode);
    Logger::ptr get(const std::string& logger_name);//通过名字查找获取对应的logger
    Logger::ptr default_logger();//获得默认logger智能指针
    Logger* get_default_raw();//获得默认logger的原始指针
    void setFormat(const std::string& logger_name,LogFormat::ptr new_default_format);//设置指定logger的format
    void setLevel(const std::string& logger_name,LogLevel new_default_level);//设置指定logger的日志级别
    void setAutoLevel(const std::string& logger_name,LogLevel new_default_auto_flush_level);//设置指定logger的自动刷新级别
    void setDefaultLogger(Logger::ptr new_default_logger);//设置新的默认logger
    void setDefaultFormat(LogFormat::ptr new_default_format);//设置默认logger的format
    void setDefaultLevel(LogLevel new_default_level);//设置默认logger的日志级别
    void setDefaultAutoLevel(LogLevel new_default_auto_flush_level);//设置默认logger的自动刷新级别
    void setDefaultPattern(const std::string pattern);//设置默认logger的pattern
    void setAllFormat(LogFormat::ptr new_all_format);//设置所有logger的format
    void setAllLevel(LogLevel new_all_level);//设置所有logger的日志级别
    void setAllAutoLevel(LogLevel new_all_auto_flush_level);//设置所有logger的自动刷新级别
    void flush_all();//刷新所有logger的缓冲区
    void drop(const std::string& logger_name);//删除logger对象
    void drop_all();//删除所有logger对象
};


}//namespace quicklog
#include "registry-inl.h"