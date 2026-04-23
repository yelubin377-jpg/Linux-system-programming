#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <vector>
typedef std::vector<std::vector<double>> Matrix;
Matrix ChengFa(Matrix &A,Matrix &B);
class XianChengChi
{
    public:
        XianChengChi(int ShuLiang); //线程数量
        ~XianChengChi();
        void AddMission(std::function<void()> Mission);
        private:
        std::vector<std::thread> _Threads;
        std::queue<std::function<void()>> _Mission;
        std::mutex _Suo;
        std::condition_variable _cv;
        bool whether_Stop = false;
};






#endif