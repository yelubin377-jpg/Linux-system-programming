//第二种解决办法，要么一次性拿两只筷子，要么不拿
#include<iostream>
#include<thread>
#include<chrono>
#include<mutex>
std::mutex _CoutSuo;
const int N = 5;
std::mutex chopsticks[N];
void philosopher(int id)
{
    int left = id;
    int right = (id+1)%N;
    while(true)
    {
        {
            std::unique_lock<std::mutex> lock3(_CoutSuo);
            std::cout << "philosopher:" <<id << " "<<"is Thinking"<<std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        std::lock(chopsticks[left],chopsticks[right]);
        
        std::unique_lock<std::mutex> lock1(chopsticks[left],std::adopt_lock);//Tell you，已经锁好了，帮我自动释放就行
        std::unique_lock<std::mutex> lock2(chopsticks[right],std::adopt_lock);
        {
            std::unique_lock<std::mutex> lock4(_CoutSuo);
            std::cout << "philosopher:" <<id <<" "<<"is Ganfaning"<<std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::this_thread::yield();//没加这个前，5个数会有一个数是老吃家，一直吃一直吃
                                  //线程饥饿
    }
}
int main()
{
    std::thread philosophers[N];
    for(int i = 0;i<N;i++)
    {
        philosophers[i] = std::thread(philosopher , i);
    }
    for(int i = 0;i<N;i++)
    {
        philosophers[i].join();
    }
}
