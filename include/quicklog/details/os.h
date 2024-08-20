#pragma once

namespace quicklog {
std::tm localtime(const std::time_t time) noexcept
{
#ifdef _WIN32
    std::tm Tm;
    ::localtime_s(&Tm, &time);
#else
    std::tm tm;
    ::localtime_r(&time, &Tm);
#endif
    return Tm;
}

std::tm localtime() noexcept
{
    std::time_t now_time = ::time(nullptr);
    return localtime(now_time);
}

std::tm gmtime(const std::time_t& time) noexcept
{
#ifdef _WIN32
    std::tm Tm;
    ::gmtime_s(&Tm, &time);
#else
    std::tm tm;
    ::gmtime_r(&time, &Tm);
#endif
    return Tm;
}

std::tm gmtime() noexcept 
{
    std::time_t now_time = ::time(nullptr);
    return gmtime(now_time);
}





}//namespace quicklog
