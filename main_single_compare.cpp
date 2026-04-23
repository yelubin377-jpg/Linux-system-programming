#include "ThreadPool.h"
#include <iostream>
#include <cstdio>
#include <random>
Matrix SuiJi_Matrix(int Hang,int Lie)
{
    Matrix m(Hang,std::vector<double>(Lie));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0,1000.0);
    for(int i = 0;i<m.size();i++)
    {
        for(int j = 0;j<m[i].size();j++)
        {
            m[i][j] = dis(gen);
        }
    }
    return m;
}


int main()
{
    XianChengChi Thread(10);
    Matrix A = SuiJi_Matrix(1000,1000); //横向=C的横向数量
    Matrix B = SuiJi_Matrix(1000,1000); //纵向=C的纵向数量
    Matrix C(1000,std::vector<double>(1000));
    Thread.AddMission([&](){C = ChengFa(A,B);});               //[&]捕获列表，把外面所有变量给我  ()参数列表，但好像用不到参数，空着  {}任务内容、执行内容
    std::this_thread::sleep_for(std::chrono::seconds(5));//PS：这里的时间必须要给足够，不然就会发生段错误！锯个栗子：5ms就会炸,3s有概率全炸或者炸一半，一大堆0, 5s就没问题了 
    for(int i = 0;i<C.size();i++)
    {
        for(int j = 0;j<C[i].size();j++)
        {
            std::cout << "C[" << i << "][" << j << "] = " << C[i][j] << std::endl;
        }
        printf("\n");
    }
        
    return 0;
}

