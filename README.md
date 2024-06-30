# QuickLog
基于C++20的日志库实现，支持自定义格式和自定义输出

## QuickLog 支持多种功能：

- 日志分级,提供8种日志等级(All,Trace,Debug,Info,Warning,Error,Fatal,Off)
- 多目标输出,通过简单的接口可以配置多个输出目标
- 简单接口，提供多种全局方法,可以直接在控制台打印日志
- 格式化的输出,使用C++20提供的format库,可以自定义打印内容
- 线程安全日志(1.0.1)

## QuickLog性能分析

测试环境 vs2022 windows11

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



### QuickLog 使用文档

[QuickLog1.0中文文档​](QuickLog1.0/日志库介绍1.0.1.md)
