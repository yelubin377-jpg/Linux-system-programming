//第三种解决办法：四个哲学家孤立剩下那一个哲学家，等四个人有人吃完，那个吃完的下桌，被孤立的那个再上桌
//Let me 使用信号量来收门票，限制人数
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <semaphore.h>
const int N = 5;
std::mutex _CoutSuo;
std::mutex chopsticks[N];
sem_t LimitNum;//4
void philosopher(int id)
{
    int left = id;
    int right = (id+1)%N;
    while(true)
    {
        sem_wait(&LimitNum);
        {
            std::unique_lock<std::mutex> lock1(_CoutSuo);
            std::cout << "philosopher:" <<id << " "<<"is Thinking"<<std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::lock(chopsticks[left],chopsticks[right]);
        std::unique_lock<std::mutex> lock1(chopsticks[left],std::adopt_lock);
        std::unique_lock<std::mutex> lock2(chopsticks[right],std::adopt_lock);
        {
            std::unique_lock<std::mutex> lock1(_CoutSuo);
            std::cout << "philosopher:" <<id << " "<<"is Eating"<<std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::this_thread::yield();
        sem_post(&LimitNum);
        
    }
}
int main()
{
    sem_init(&LimitNum,0,4);
    std::thread philosophers[N];
    for(int i = 0;i<N;i++)
    {
        philosophers[i] = std::thread(philosopher,i);
    }
    for(int i = 0;i<N;i++)
    {
        philosophers[i].join();
    }
    
}
//这个方法不能斯斯文文慢慢拿，不然桌上4个老吃家会把被孤立那人饿死
//这个方法必须筷子两只两只拿
