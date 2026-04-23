#include "random_generator.h"
#include<random>
std::vector<int> GenerateRandomNumbers(int count,int MinVal,int MaxVal)
{
    std::vector<int> result;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(MinVal,MaxVal);
    for(int i = 0;i<count;i++)
    {
        result.push_back(dis(gen));//生成一个随机数，加入vector
    }
    return result;
}