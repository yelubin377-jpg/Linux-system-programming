#include "ThreadPool.h"
#include <vector>
#include <pthread.h>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>

void DownloadTogether::worker() 
{
    while(true)
    {
        std::unique_lock<std::mutex> lock1(_Suo);
        _cv.wait(lock1,
            [this]
            {
                return WhetherStop || !_Mission.empty();
            }     
        );
        if(WhetherStop && _Mission.empty())
        {
            return;
        }
        std::function<void()> RenWu = _Mission.front();
        _Mission.pop();
        lock1.unlock();
        RenWu();
    }
}

DownloadTogether::DownloadTogether(int count)
{
    for(int i = 0;i<count;i++)
    {
        _threads.emplace_back(&DownloadTogether::worker,this);
    }
}

DownloadTogether::~DownloadTogether()
{
    DownloadTogether::WhetherStop = true;
    _cv.notify_all();
    for(int i = 0;i<_threads.size();i++)
    {
        _threads[i].join();
    }

}

void DownloadTogether::AddMission(std::function<void()> Mission)
{
    std::unique_lock<std::mutex> lock114514(_Suo);
    _Mission.push(Mission);
    _cv.notify_all();
    
}


int DownloadTogether::GetThreadCount()
{
    return _threads.size();
}
void DownloadTogether::SetThreadCount(int NewThreadCount)
{
    int now = _threads.size();
    if(NewThreadCount > now)
    {
        for(int i = 0;i<NewThreadCount - now;i++)
        {
            _threads.emplace_back(&DownloadTogether::worker,this);
        }

    }
}