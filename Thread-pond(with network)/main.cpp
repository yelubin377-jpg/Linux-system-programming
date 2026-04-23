#include <iostream>
#include <thread>
#include <cstdio>
#include "ThreadPool.h"
#include <chrono>

void download_file(const std::string &url,const std::string &CunchuPath)
{
    std::string command = "wget -O " + CunchuPath + " " + url;//下载命令：wget -o /保存路径 http://网址
    system(command.c_str());//让系统帮你输入，不用搞网络socket那些7788的
    
}
int main()
{
    int ThreadCount[] = {1,2,5,10};
    for(int i = 0;i < 4;i++)
    {
        auto Start = std::chrono::high_resolution_clock::now();

        DownloadTogether *XiaZai = new DownloadTogether(ThreadCount[i]);

        XiaZai->AddMission([](){download_file("https://httpbin.org/image/jpeg","./image/NaiLong1.jpg");});
        XiaZai->AddMission([](){download_file("https://httpbin.org/image/jpeg","./image/NaiLong2.jpg");});
        XiaZai->AddMission([](){download_file("https://httpbin.org/image/jpeg","./image/NaiLong3.jpg");});
        XiaZai->AddMission([](){download_file("https://httpbin.org/image/jpeg","./image/NaiLong10086.jpg");});
        XiaZai->AddMission([](){download_file("https://httpbin.org/image/jpeg","./image/NaiLong4.jpg");});
        XiaZai->AddMission([](){download_file("https://httpbin.org/image/jpeg","./image/NaiLong5.jpg");});
        XiaZai->AddMission([](){download_file("https://httpbin.org/image/jpeg","./image/NaiLong6.jpg");});
        XiaZai->AddMission([](){download_file("https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png","./image/NaiLong7.jpg");});
        XiaZai->AddMission([](){download_file("https://httpbin.org/image/jpeg","./image/NaiLong8.jpg");});
        XiaZai->AddMission([](){download_file("https://xiyoulinux.com/", "./image/xiyoulinux.html");});
        //std::this_thread::sleep_for(std::chrono::seconds(100));   100s有概率下载不下来
        std::cout << "电脑成功开放，黑客成功入侵" <<std::endl;
        delete XiaZai;
        auto End = std::chrono::high_resolution_clock::now();
        auto Time = std::chrono::duration_cast<std::chrono::milliseconds>(End - Start).count();
        printf("\n");
        printf("\n");
        printf("\n");
        printf("全体目光向我看齐！！！\n");
        std::cout << "有" << ThreadCount[i] << "个线程跑" << Time << "ms" <<std::endl;
        printf("\n");
        printf("全体目光向我看齐！！！\n");
        printf("全体目光向我看齐！！！\n");
        printf("全体目光向我看齐！！！\n");
        printf("全体目光向我看齐！！！\n");
        printf("全体目光向我看齐！！！\n");
        printf("全体目光向我看齐！！！\n");
    
    
    }
    return 0;
}


//这台电脑跑出来的数据：
//1   13929ms
//2   6240ms
//5   3129ms
//10  4056ms ???   

//because 线程创建也要时间，任务又不需要那么多线程，太多线程会有一堆混子
