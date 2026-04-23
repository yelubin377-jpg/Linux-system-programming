#include "random_generator.h"
#include "single_thread_sort.h"
#include <iostream>
int main()
{
    std::vector<int> numbers = GenerateRandomNumbers(10,1,100);
    
    std::cout << "Random Numbers:" << std::endl;
    for(int num=0;num<numbers.size();num++)
    {
        std::cout << numbers[num] << " ";
    }
    std::cout << std::endl;
    single_thread_sort(numbers);
    std::cout << "When the ShuZu behind sort:"<<std::endl;
    for(int i = 0;i<numbers.size();i++)
    {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}