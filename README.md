# QuickLog
QuickLog 一个基于C++20开发的高性能纯头文件同步日志库，它提供了向控制台和文件输出日志的能力。

QuickLog 具有以下特点：

- 高性能：QuickLog可以做到每秒百万条日志写入
- 丰富的日志级别：提供8种日志等级(All,Trace,Debug,Info,Warning,Error,Fatal,Off)
- 轻量化：QuickLog 是纯头文件库，用户只需要包含相应的头文件即可开始使用，无需编译链接额外的库文件
- 自定义格式化：QuickLog 允许用户自定义日志的格式，可以包含时间戳、线程ID、文件名、行号以及函数名等上下文信息
- 多目标输出：通过简单的接口可以配置多个输出目标
- 线程安全：对于多线程环境，QuickLog 提供了线程安全的日志接口，确保在并发环境下日志记录的正确性和完整性
- 简单接口：提供多种全局方法,可以直接在控制台打印日志

## QuickLog性能分析

测试环境 vs2022 windows11
### QuickLog2.0
debug模式下平均百万条日志打印耗时**9604ms**

release模式下平均百万条日志打印耗时**867ms**

#### 测试代码

```c++
#include<iostream>
#include<chrono>
#include<quicklog/quicklog.h>
using namespace quicklog;
int main()
{
    create<txt_st>("logger", "test.txt");
    auto log = registry.get("logger");
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

### QuickLog1.0
debug模式下平均百万条日志打印耗时**19829ms**

release模式下平均百万条日志打印耗时**2225ms**

#### 测试代码

```c++
#include"quickLog.h"
#include<iostream>
#include<chrono>
int main()
{
    
    auto start = std::chrono::high_resolution_clock::now();
    Logger::ptr log(std::make_shared<Logger>(LogLevel::Debug, initFormat("{0} {1} Level[{3}] 消息:{5}"), initFile("test.txt")));
    for (int i = 0; i < 1000000; i++)
    {
        log->Debug("调试：指明调试过程中的事件信息");
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << duration << "ms" << std::endl;
    return 0;
}
```



## QuickLog 使用文档

[QuickLog1.0中文文档​](QuickLog1.0/日志库介绍1.0.1.md)

[QuickLog2.0中文文档​](QuickLog2.0/日志库介绍2.0.md)

## 计划更新

#

- [ ] 异步日志
- [ ] csv文件输出
- [ ] 数据库输出

## 更新日志

**2024-7-24** 微软更新了std::make_format_args()函数，参数采用左值引用而不是转发引用，右值将不再能作为参数进行传递

参考链接：
[P2905R2:运行时格式字符串 报告](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2023/p2905r2.html#impact)



