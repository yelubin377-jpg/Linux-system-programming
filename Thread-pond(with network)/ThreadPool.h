#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <vector>
#include <pthread.h>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <thread>

class DownloadTogether
{
    public:
        int GetThreadCount();
        void SetThreadCount(int ThreadCount);        
        DownloadTogether(int count);
        ~DownloadTogether();
        void AddMission(std::function<void()> Mission);
    private:
        void worker();
        std::vector<std::thread> _threads;//vector可以装线程，vector是一个万能容器
        std::queue<std::function<void()>> _Mission;
        std::mutex _Suo;
        std::condition_variable _cv;
        bool WhetherStop = false;//牛马下班铃
};





















#endif
