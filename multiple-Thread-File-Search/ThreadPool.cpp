#include "ThreadPool.h"
#include <iostream>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <vector>

Thread_Pool::Thread_Pool(int ThreadCount)
{
    for(int i = 0;i<ThreadCount;i++)
    {
        _threads.emplace_back([this]
            {
                while(true)
                {
                    std::unique_lock<std::mutex> lock10001(_Suo);
                    _cv.wait(lock10001,[this]
                    {
                        return WhetherStop || !_Mission.empty();
                    });
                    if(WhetherStop && _Mission.empty())
                    {
                        return;
                    }
                    std::function<void()> RenWu = _Mission.front();
                    _Mission.pop();
                    lock10001.unlock();
                    RenWu();
                }
            });
    }
}


Thread_Pool::~Thread_Pool()
{
    WhetherStop = true;
    _cv.notify_all();
    for(int i = 0;i<_threads.size();i++)
    {
        _threads[i].join();
    }
}


void Thread_Pool::AddMission(std::function<void()> Mission)
{
    std::unique_lock<std::mutex> lock10086(_Suo);
    _Mission.push(Mission);
    _cv.notify_all();
}