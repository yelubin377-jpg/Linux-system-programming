//第一种解决办法：1,3,5先左后右  2,4,先右后左
#include<iostream>
#include<thread>
#include<chrono>
#include<mutex>
std::mutex _CoutSuo;
//std::mutex _ThinkingSuo;//思考的时候也来一把锁     //一把锁一起管才有用，两把所个馆阁的反而不行
const int N = 5;
std::mutex chopsticks[N];
void philosopher(int id)
{
    int left = id;
    int right = (id + 1) % N;//别越界
    
    while(true)
    {
        {
            std::unique_lock<std::mutex> lock4(_CoutSuo);      
            std::cout << "Philosopher " << id << " is thinking whether to eat or not >__<" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        if(id % 2 != 0)//奇左
        {
            std::unique_lock<std::mutex> lock1(chopsticks[left]);
            std::unique_lock<std::mutex> lock2(chopsticks[right]);
            {
                std::unique_lock<std::mutex> lock3(_CoutSuo);
                std::cout << "Philosopher " << id << " is GanFaning with chopsticks " << left << " and " << right << std::endl;
            }    
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            
        }
        else
        {
            std::unique_lock<std::mutex> lock1(chopsticks[right]);
            std::unique_lock<std::mutex> lock2(chopsticks[left]);
            {
                std::unique_lock<std::mutex> lock3(_CoutSuo);
                std::cout << "Philosopher " << id << " is GanFaning with chopsticks " << right << " and " << left << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            
        }
    }
}

int main()
{
    std::thread philosophers[N];//创建线程数组
    for(int i = 0;i<N;i++)
    {
        philosophers[i] = std::thread(philosopher,i);
    }
    for(int i = 0;i<N;i++)
    {
        philosophers[i].join();
    }
    return 0;
}

//第一次写完后发现会抢cout，那就给cout上个锁
