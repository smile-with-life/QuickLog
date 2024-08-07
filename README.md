# QuickLog2.0介绍(基于c++20)

QuickLog 一个基于C++20开发的高性能纯头文件同步日志库，它提供了向控制台和文件输出日志的能力。

QuickLog 具有以下特点：

- 高性能：QuickLog可以做到每秒百万条日志写入
- 丰富的日志级别：提供8种日志等级(All,Trace,Debug,Info,Warning,Error,Fatal,Off)
- 轻量化：QuickLog 是纯头文件库，用户只需要包含相应的头文件即可开始使用，无需编译链接额外的库文件
- 自定义格式化：QuickLog 允许用户自定义日志的格式，可以包含时间戳、线程ID、文件名、行号以及函数名等上下文信息
- 多目标输出：通过简单的接口可以配置多个输出目标
- 线程安全：对于多线程环境，QuickLog 提供了线程安全的日志接口，确保在并发环境下日志记录的正确性和完整性
- 简单接口：提供多种全局方法,可以直接在控制台打印日志

# QuickLog配置

QuickLog库的使用非常简单，只需要下载源代码，然后把QuickLog2.0/include目录下的文件拷贝到我们的工程下，在工程中包含相应的目录即可。使用时包含头文件quicklog,h，若想要进行一些优化可以在config.h文件中进行修改

### 示例

。。。

# QuickLog2.0性能分析

测试环境 vs2022  Windows11 

debug模式下平均百万条日志打印耗时**7737ms**

release模式下平均百万条日志打印耗时**617ms**

```c++
#include<iostream>
#include<chrono>
#include<quicklog/quicklog.h>
using namespace quicklog;
int main()
{
    auto log = registry.init_logger<file_st>("file_logger","test.txt");
    long long total_duration = 0;
    for(int j=0;j<20;++j)
    {
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000000; i++)
        {
            log->Debug("调试：指明调试过程中的事件信息");
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        total_duration += duration;
    }
    std::cout << "平均运行时间" << total_duration/20 << "ms" << std::endl;
    return 0;
}
```

# QuickLog使用

### 1.直接调用标准方法输出到控制台终端，代码如下

```c++
#include"quickLog.h"
using namespace quicklog;
int main()
{ 
    Trace("跟踪：指明程序运行轨迹");
    Debug("调试：指明调试过程中的事件信息");
    Info("信息：指明运行过程中的重要信息");
    Warning("警告：指明可能潜在的危险状况");
    Error("错误：指明错误事件");
    Fatal("致命：指明非常严重的可能会导致应用终止执行错误事件");       
    return 0;
}
```

**输出结果**

![log-image-8](./quicklog_image/log-image-8)

1. 日志库的输出等级有 Trace,Debug,Info,Warning,Error,Fatal(All为最低等级,表示全部打印;Off为最高等级,表示全部关闭)
2. 默认的输出等级为All
3. 默认的自动刷新等级为Warning（输出Warning即以上等级时，自动刷新缓冲区）
4. 标准方法默认输出到控制台（线程安全）
5. 默认采用彩色打印（automatic）
6. 默认的输出格式为: 时间   文件路径   线程Id   日志等级   行号   消息   程序运行时间
7. 默认的时间模式是本地时间

### 2.可以改变默认logger对象的输出等级,使其不输出低等级的日志，代码如下

```c++
#include"quickLog.h"
int main()
{ 
    registry.setDefaultLevel(LogLevel::Error);
    Trace("跟踪：指明程序运行轨迹");
    Debug("调试：指明调试过程中的事件信息");
    Info("信息：指明运行过程中的重要信息");
    Warning("警告：指明可能潜在的危险状况");
    Error("错误：指明错误事件");
    Fatal("致命：指明非常严重的可能会导致应用终止执行错误事件");      
    return 0;
}
```

**输出结果**

![log-image-9](./quicklog_image/log-image-9)

1. QuickLog提供一个全局registry对象，用于管理logger
2. 将默认logger对象的等级设置为Error后,Trace,Debug,Info,Warning等级的日志都不打印

### 3.可以改变默认logger对象的输出格式,使其不输出不同格式的日志，代码如下

```c++
#include"quickLog.h"
int main()
{ 
    registry.setDefaultPattern("时间:{0} 函数名:{2} [{4}] {6}");
    qTrace("跟踪：指明程序运行轨迹");
    qDebug("调试：指明调试过程中的事件信息");
    qInfo("信息：指明运行过程中的重要信息");
    qWarning("警告：指明可能潜在的危险状况");
    qError("错误：指明错误事件");
    qFatal("致命：指明非常严重的可能会导致应用终止执行错误事件"); 

    return 0;
}
```

**输出结果**

![log-image-10](./quicklog_image/log-image-10)

日志格式符
{0} 时间   
{1} 文件名 
{2} 函数名
{3} 线程Id
{4} 日志级别
{5} 行号
{6} 消息
默认:"{0} {1} thread[{3}] [{4}] line[{5}] message[{6}] "
2024-04-14 15:24:57  QuickLog.h main（） thread[23904] [Debug] line[72] message[hello]

可以通过改变字符串中的格式符来自定义自己的打印格式

### 4.可以自定义一个logger对象,用于将日志输出到控制台,代码如下

```c++
#include"quickLog.h"
int main()
{ 
    auto log = registry.init_logger("console_logger"); 
    log->Trace("跟踪：指明程序运行轨迹");
    log->Debug("调试：指明调试过程中的事件信息");
    log->Info("信息：指明运行过程中的重要信息");
    log->Warning("警告：指明可能潜在的危险状况");
    log->Error("错误：指明错误事件");
    log->Fatal("致命：指明非常严重的可能会导致应用终止执行错误事件");

    return 0;
}
```

**输出结果**

![log-image-11](./quicklog_image/log-image-11)

1. 上面的示例代码中,使用了init_logger函数进行自定义logger的创建
2. "console_logger"，指定了日志输出器的名称
3. 日志输出器默认为有锁版本的控制台输出

### 5.自定义控制台颜色输出，代码如下

```c++
#include"quickLog.h"
int main()
{ 
    auto log_always = registry.init_logger<console_st>("console_logger_always",ColorMode::always); 
    auto log_automatic = registry.init_logger<console_st>("console_logger_automatic", ColorMode::automatic);
    auto log_never = registry.init_logger<console_st>("console_logger_never", ColorMode::never); 
    
    log_always->Debug("调试：指明调试过程中的事件信息");
    log_automatic->Debug("调试：指明调试过程中的事件信息");
    log_never->Debug("调试：指明调试过程中的事件信息");

    return 0;
}
```

**输出结果**

![log-image-12](./quicklog_image/log-image-12)

1. 上面的示例代码中,使用了create模板函数进行自定义logger的创建
2. console_st，表示使用无锁版本的控制台
3. ColorMode::always，表示整个日志消息改变颜色
4. ColorMode::automatic，表示日志消息中的日志等级改变颜色
5. ColorMode::never，表示不使用颜色控制
6. Trace(白色) Debug(绿色) Info(蓝色) Warning(黄色) Error(红色) Fatal(紫色)

### 6.可以自定义一个logger对象,用于将日志输出到文件,代码如下

```c++
#include"quickLog.h"
int main()
{ 
    auto log = registry.init_logger<txt_st>("console_logger","test.txt");

    log->Trace("跟踪：指明程序运行轨迹");
    log->Debug("调试：指明调试过程中的事件信息");
    log->Info("信息：指明运行过程中的重要信息");
    log->Warning("警告：指明可能潜在的危险状况");
    log->Error("错误：指明错误事件");
    log->Fatal("致命：指明非常严重的可能会导致应用终止执行错误事件");

    return 0;
}
```

**输出结果**

![log-image-13](./quicklog_image/log-image-13)

1. 上面的示例代码中,使用了registry.init_logger模板函数进行自定义logger的创建
2. txt_st，表示使用无锁版本的txt文件
3. "test.txt"指定文件名/文件路径

# QuickLog其他功能

### 1.支持多种时间类型

`void setTimeType(TimeType timeType);`

- **TimeType::Local**  本地时间
- **TimeType::UTC**   UTC时间

### 2.支持输出到标准错误流

`void setConsoleMode(ConsoleMode consoleMode);`

- **ConsoleMode::stdoutConsole**  标准输出流
- **ConsoleMode::stderrConsole**   标准错误流

### 3.支持线程安全打印

**txt_mt,console_mt等**，表示有锁版本

**txt_st，console_st等**，表示无锁版本

### 4.registry，全局logger管理者

| 方法                                                         | 描述                         |
| ------------------------------------------------------------ | ---------------------------- |
| `void register_logger(Logger::ptr new_logger);`              | 注册logger                   |
| `Logger::ptr init_logger(const std::string& logger_name);`   | 默认初始化一个logger并注册   |
| `template<typename Type,typename... Args><br/>Logger::ptr init_logger(const std::string& logger_name,Args&&... args);` | 初始化一个logger并注册       |
| `Logger::ptr get(const std::string& logger_name);`           | 通过名字查找获取对应的logger |
| `Logger::ptr default_logger();`                              | 获得默认logger智能指针       |
| `void setLevel(const std::string& logger_name,LogLevel new_default_level);` | 设置指定logger的日志级别     |
| `void setAutoLevel(const std::string& logger_name,LogLevel new_default_auto_flush_level);` | 设置指定logger的自动刷新级别 |
| `void setPattern(const std::string& logger_name,const std::string& new_pattern);` | 设置指定logger的日志格式     |
| `void addAppender(const std::string& logger_name,Appender::ptr appender);` | 向指定logger中添加appender   |
| `void clearAppender(const std::string& logger_name);`        | 清空指定logger的appender列表 |
| `LogLevel getLevel(const std::string& logger_name);`         | 获取指定logger的日志等级     |
| `LogLevel getAutoLevel(const std::string& logger_name);`     | 获取指定logger的自动刷新级别 |
| `std::vector<std::string> getPattern(const std::string& logger_name);` | 获取指定logger的             |
| `void setDefaultLevel(LogLevel new_default_level);`          | 设置默认logger的日志级别     |
| `void setDefaultAutoLevel(LogLevel new_default_auto_flush_level);` | 设置默认logger的自动刷新级别 |
| `void setDefaultPattern(const std::string pattern);`         | 设置默认logger的日志格式     |
| `void addDefaultAppender(Appender::ptr appender);`           | 向默认logger中添加appender   |
| `void clearDefaultAppender();`                               | 清空默认logger的appender列表 |
| `LogLevel getDefaultLevel();`                                | 获取默认logger的日志等级     |
| `LogLevel getDefaultAutoLevel();`                            | 获取默认logger的自动刷新级别 |
| `std::vector<std::string> getDefaultPattern();`              | 获取默认logger的日志格式     |
| `void setAllLevel(LogLevel new_all_level);`                  | 设置所有logger的日志级别     |
| `void setAllAutoLevel(LogLevel new_all_auto_flush_level);`   | 设置所有logger的自动刷新级别 |
| `void setAllPattern(const std::string& new_pattern);`        | 设置默认logger的日志格式     |
| `void addAllAppender(Appender::ptr appender);`               | 向所有logger中添加appender   |
| `void clearAllAppender();`                                   | 清空所有logger的appender列表 |
| `void flush_all();`                                          | 刷新所有logger的缓冲区       |
| `void drop(const std::string& logger_name);`                 | 删除指定logger对象           |
| `void drop_all();`                                           | 删除所有logger对象           |

# 更新

- [ ] 异步日志
- [ ] 性能优化

## 更新日志

**2024-7-24** 微软更新了std::make_format_args()函数，参数采用左值引用而不是转发引用，右值将不再能作为参数进行传递

参考链接：
[P2905R2:运行时格式字符串 报告](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2905r2.html#impact)

**2024-7-27** 简化了部分接口，优化性能，修改了file_helper类

1.让输出更加简单易用

2.性能提升20%

3.file_helper类变为file_API类，增加了更多的接口


