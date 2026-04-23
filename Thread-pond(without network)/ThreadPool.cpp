#include "ThreadPool.h"
#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <condition_variable>
#include <mutex>

Matrix ChengFa(Matrix& A,Matrix& B)//A行*B列
{
    int z = A.size();//A有几横向
    int x = A[0].size();//A 有几个纵向
    int c = B[0].size();//B 有几个纵向
    Matrix C(z,std::vector<double>(c,0.0));
    for(int i = 0;i<z;i++)
    {
        for(int j = 0;j<c;j++)
        {
            for(int k = 0;k<x;k++)
            {
                C[i][j] += A[i][k] * B[k][j];                  
            }
        }
    }
    return C;
}

XianChengChi::XianChengChi(int ShuLiang)  //构造函数：对象创建时调用（开头）
{
    for(int i = 0;i<ShuLiang;i++)   
    {
        _Threads.emplace_back([this]                           //emplace_back 在数组末尾直接创建一个新元素
        {
            while(true)
            {
                std::unique_lock<std::mutex> lock1(_Suo);
                _cv.wait(lock1,
                    [this]
                    {
                        return whether_Stop || !_Mission.empty();//收工or牛马继续加班
                    });
                if(whether_Stop && _Mission.empty())
                {
                    return;
                }
                //跑不掉？抓过来干活（doge
                std::function<void()> RenWu = std::move(_Mission.front());
                _Mission.pop();
                lock1.unlock();
                RenWu();

            }
        });
    }
}


XianChengChi::~XianChengChi()  //细狗函数：对象销毁时调用（结尾）
{
    XianChengChi::whether_Stop = true;
    _cv.notify_all();
    for(int i = 0;i<_Threads.size();i++)
    {
        _Threads[i].join();
    }//前面有"_"是自己人，不是外面读取的
}


void XianChengChi::AddMission(std::function<void()> Mission)   //不是自己人，外来任务
{
    std::unique_lock<std::mutex> lock10086(_Suo);
    _Mission.push(Mission);
    _cv.notify_all();
}