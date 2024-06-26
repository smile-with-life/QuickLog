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