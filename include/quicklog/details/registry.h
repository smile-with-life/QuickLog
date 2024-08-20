#pragma once

#include <quicklog/common.h>
#include <quicklog/logger.h>
#include <quicklog/asynclogger.h>
#include <mutex>
#include <unordered_map>


namespace quicklog{

class Registry
{
private:
    std::mutex m_mutex;
    std::unordered_map<std::string, Logger_ptr> m_logger_map;
    Logger_ptr m_default_logger;

public:
    Registry();
    ~Registry() = default;
    void enroll(Logger_ptr new_logger);//注册logger
    Logger_ptr init(const std::string& logger_name);
    template<typename Type,typename... Args>
    Logger_ptr init(const std::string& logger_name,Args&&... args);
    Logger_ptr init_async(std::weak_ptr<ThreadPool> thread_pool, const std::string& logger_name);
    template<typename Type, typename... Args>
    Logger_ptr init_async(std::weak_ptr<ThreadPool> thread_pool, const std::string& logger_name, Args&&... args);
    Logger_ptr get(const std::string& logger_name) noexcept;//通过名字查找获取对应的logger
    Logger_ptr default_ptr() noexcept;//获得默认logger智能指针
    Logger* default_raw() noexcept;//获得默认logger的原始指针
    
    //设置指定logger的属性
    void setLevel(const std::string& logger_name, LogLevel new_level);
    void setAutoLevel(const std::string& logger_name,LogLevel new_auto_level);
    void setTimeType(const std::string& logger_name,TimeType time_type);
    void addAppender(const std::string& logger_name,Appender::ptr appender);
    void clearAppender(const std::string& logger_name);
    //获取指定logger的属性
    LogLevel getLevel(const std::string& logger_name);
    LogLevel getAutoLevel(const std::string& logger_name);
    TimeType getTimeType(const std::string& logger_name);
    //设置默认logger的属性
    void setDefaultLevel(LogLevel new_level);
    void setDefaultAutoLevel(LogLevel new_auto_level);
    void setDefaultTimeType(TimeType time_type);
    void addDefaultAppender(Appender_ptr appender);
    void clearDefaultAppender();
    //获取默认logger的属性
    LogLevel getDefaultLevel();
    LogLevel getDefaultAutoLevel();
    TimeType getDefaultTimeType();
    //设置所有logger的属性
    void setAllLevel(LogLevel new_level);
    void setAllAutoLevel(LogLevel new_auto_level);
    void setAllTimeType(TimeType time_type);
    void addAllAppender(Appender::ptr appender);
    void clearAllAppender();
#ifdef USE_RUN_FORMAT
    void setPattern(const std::string& logger_name, const std::string& new_pattern);

    void setDefaultPattern(const std::string& new_pattern);

    void setAllPattern(const std::string& new_pattern);

    std::vector<std::string> getPattern(const std::string& logger_name);

    std::vector<std::string> getDefaultPattern();

#endif // USE_RUN_FORMAT
    void flush_all();//刷新所有logger的缓冲区
    void drop(const std::string& logger_name);//删除指定logger对象
    void drop_all();//删除所有logger对象（包括默认logger）
private:
    bool check_logger(std::string& logger_name);//检查logger是否注册过
};


}//namespace quicklog
#include "registry-inl.h"