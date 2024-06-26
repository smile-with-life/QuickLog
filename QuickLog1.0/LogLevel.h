#pragma once

enum class LogLevel : int
{
    All,      //所有：所有日志级别
    Trace,    //跟踪：指明程序运行轨迹
    Debug,    //调试：指明调试过程中的事件信息
    Info,     //信息：指明运行过程中的重要信息
    Warning,  //警告：指明可能潜在的危险状况
    Error,    //错误：指明错误事件
    Fatal,    //致命：指明非常严重的可能会导致应用终止执行错误事件
    Off       //关闭：最高级别，不打印日志
};