#include "multi_thread_sort.h"
#include "random_generator.h"
#include "single_thread_sort.h"
#include<iostream>
#include<thread>
#include<vector>
#include<algorithm>
#include<chrono>//哲学家问题游泳到--sleep_for
#include<mutex>


int main()
{
    std::vector<int>numbers = GenerateRandomNumbers(100000, 1, 100000);
    std::vector<int>numbers_Copy = numbers;
    
    
    std::chrono::time_point<std::chrono::high_resolution_clock> start1 = std::chrono::high_resolution_clock::now();
    //等价于auto start1 = std::chrono::high_resolution_clock::now();
    //However 搞清楚auto代表什么really significant
    single_thread_sort(numbers);
    std::chrono::time_point<std::chrono::high_resolution_clock> end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long long, std::milli> duration = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1);
    long long SingleTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();


    std::chrono::time_point<std::chrono::high_resolution_clock> start2 = std::chrono::high_resolution_clock::now();
    multi_thread_sort(numbers_Copy);
    std::chrono::time_point<std::chrono::high_resolution_clock> end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<long long,std::milli> duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2-start2);
    long long MultiTime = std::chrono::duration_cast<std::chrono::milliseconds>(duration2).count();
    //简化版：eg:long long MultiTime = std::duration_cast<std::chrono::milliseconds>(end2 - start2).count();
    
    std::cout << "Single thread costs:  "  << SingleTime << "  milliseconds" << std::endl;
    std::cout << "Multi  thread  cost:  "  << MultiTime  << "  milliseconds" <<  std::endl;
}


