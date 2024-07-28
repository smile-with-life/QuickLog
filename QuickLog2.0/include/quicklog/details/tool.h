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

//返回一个表示日志等级的字符串(throw）

unsigned int getThreadId()
{
    thread_local auto threadId = std::this_thread::get_id();
    return *(unsigned int*)&threadId;
}
}//namespace quicklog
