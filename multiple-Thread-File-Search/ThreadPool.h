#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <thread>
#include <vector>
#include <mutex>
#include <cstdio>
#include <queue>
#include <functional>
#include <condition_variable>
class Thread_Pool
{
private:
    std::vector<std::thread> _threads;
    std::queue<std::function<void()>> _Mission;
    std::mutex _Suo;
    std::condition_variable _cv;
    bool WhetherStop = false;
public:
    Thread_Pool(int ThreadCount);//构造函数
    ~Thread_Pool();//细狗函数
    void AddMission(std::function<void()> Mission);
};

















#endif