#pragma once
#include<memory>
#include<mutex>
#include<unordered_map>

#include<quicklog/common.h>
#include<quicklog/logger.h>
#include<quicklog/logformat.h>
#include<quicklog/appender/console_appender.h>
#include<quicklog/appender/file_appender.h>


namespace quicklog{

class Registry
{
private:
    std::mutex m_mutex;
    std::unordered_map<std::string, Logger::ptr> m_loggerMap;
    Logger::ptr m_default_logger;

public:
    Registry();
    ~Registry();
    void register_logger(Logger::ptr new_logger);//注册logger
    Logger::ptr init_logger(const std::string& logger_name);//默认初始化一个logger并注册
    template<typename Type,typename... Args>
    Logger::ptr init_logger(const std::string& logger_name,Args&&... args);
    Logger::ptr get(const std::string& logger_name);//通过名字查找获取对应的logger
    Logger::ptr default_logger();//获得默认logger智能指针
    
    //设置指定logger的属性
    void setLevel(const std::string& logger_name, LogLevel new_level);
    void setAutoLevel(const std::string& logger_name,LogLevel new_auto_level);
    void setPattern(const std::string& logger_name,const std::string& new_pattern);
    void addAppender(const std::string& logger_name,Appender::ptr appender);
    void clearAppender(const std::string& logger_name);
    //获取指定logger的属性
    LogLevel getLevel(const std::string& logger_name);
    LogLevel getAutoLevel(const std::string& logger_name);
    std::vector<std::string> getPattern(const std::string& logger_name);
    //设置默认logger的属性
    void setDefaultLevel(LogLevel new_level);
    void setDefaultAutoLevel(LogLevel new_auto_level);
    void setDefaultPattern(const std::string& new_pattern);
    void addDefaultAppender(Appender::ptr appender);
    void clearDefaultAppender();
    //获取默认logger的属性
    LogLevel getDefaultLevel();
    LogLevel getDefaultAutoLevel();
    std::vector<std::string> getDefaultPattern();
    //设置所有logger的属性
    void setAllLevel(LogLevel new_level);
    void setAllAutoLevel(LogLevel new_auto_level);
    void setAllPattern(const std::string& new_pattern);
    void addAllAppender(Appender::ptr appender);
    void clearAllAppender();

    void flush_all();//刷新所有logger的缓冲区
    void drop(const std::string& logger_name);//删除指定logger对象
    void drop_all();//删除所有logger对象（包括默认logger）
private:
    bool check_logger(std::string& logger_name);//检查logger是否注册过
};


}//namespace quicklog
#include "registry-inl.h"