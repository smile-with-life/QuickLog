#pragma once


/*是否使用cpp的IO输出流*/
//#define USE_FILE_CPP 

/*单个logger最多可拥有的appender个数*/
const int MaxAppenderCount = 5;

/*是否启用运行时日志格式修改*/
//#define USE_RUN_FORMAT 
constexpr std::string_view Global_Pattern = "{0} {1} [{4}] line[{5}] {6}\n";
