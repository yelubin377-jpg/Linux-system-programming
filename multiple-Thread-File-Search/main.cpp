#include "Search.h"
#include "ThreadPool.h"
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

int main()
{
    Search SouSuo(4);
    SouSuo.RootPath = "/home/michael-377/桌面/";
    SouSuo.FileKind = ".md";
    SouSuo.MaxThreadCount = 4;
    SouSuo.MaxDepth = 10;
    SouSuo.WhetherJumpHidden = true;
    SouSuo.JumpPath = {"/home/michael-377/桌面/ls"};

    std::cout << "开始搜索: " << SouSuo.RootPath << std::endl;
    std::cout << "目标类型: " << SouSuo.FileKind << std::endl;

    SouSuo.SearchFunction(SouSuo.RootPath,0);

    return 0;
}