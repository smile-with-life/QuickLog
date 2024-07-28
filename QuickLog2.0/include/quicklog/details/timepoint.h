#pragma once
#include <chrono>
#include <ctime>
#include <sstream>

namespace quicklog{
//返回自程序启动时起的处理器的时钟时间
uint64_t elapse()
{
    return clock();
}
struct TimePoint
{
    TimePoint()
        : system_time(std::chrono::system_clock::now())
    {}

    TimePoint(const TimePoint& other) = default;
    TimePoint& operator=(const TimePoint& other) = default;

    const std::chrono::time_point<std::chrono::system_clock> system_time;
    
    const std::string strLocal_time() const//3.5us耗时操作
    {
        std::time_t time = std::chrono::system_clock::to_time_t(system_time);
        std::tm Tm;
        localtime_s(&Tm, &time);
        char buf[64];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &Tm);     
        return buf;
    }

    const std::string strUTC_time() const
    {
        std::time_t time = std::chrono::system_clock::to_time_t(system_time);
        std::tm Tm;
        gmtime_s(&Tm, &time);
        char buf[64];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &Tm);
        return buf;
    }

};

}