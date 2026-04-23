#ifndef SEARCH_H
#define SEARCH_H
#include <string>
#include <vector>
#include <mutex>
#include <thread>
#include <filesystem>
#include <iostream>
#include <algorithm>
#include "ThreadPool.h"
class Search
{
public:
    std::string RootPath;
    std::string FileKind;
    int MaxThreadCount;
    int MaxDepth;
    bool WhetherJumpHidden = true;
    Thread_Pool *ThreadPool;
    Search(int thread_count);
    ~Search();
    std::vector<std::string> JumpPath;
    void SearchFunction(const std::filesystem::path &path,int NowDepth);
private:
    std::mutex _Suo;
};
Search::Search(int thread_count)
{
    ThreadPool = new Thread_Pool(thread_count);
}
Search::~Search()
{
    delete ThreadPool;
}

void Search::SearchFunction(const std::filesystem::path &path,int NowDepth)
{
    if(NowDepth > MaxDepth)
    {
        return;
    }
    for(const auto &entry :std::filesystem::directory_iterator(path))  
    {
        if(WhetherJumpHidden && entry.path().filename().string()[0] == '.')
        {
            continue;
        }
        bool ShouldSkip = false;
        for(const auto &skipPath : JumpPath)
        {
            if(entry.path().string().find(skipPath) != std::string::npos)
            {
                ShouldSkip = true;
                break;
            }
        }
        if(ShouldSkip)
        {
            continue;
        }
        if(entry.is_regular_file())
        {
            if(entry.path().extension() == FileKind)
            {
                std::unique_lock<std::mutex> lock1415926(_Suo);
                std::cout << entry.path() <<std::endl;
            }
        }
        else if(entry.is_directory())
        {
            ThreadPool->AddMission([this,BaoXian = entry.path(),NowDepth](){
                SearchFunction(BaoXian, NowDepth + 1);
            });

        }

    }

}













#endif