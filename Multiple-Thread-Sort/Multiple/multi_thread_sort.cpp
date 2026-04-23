#include "multi_thread_sort.h"
#include <algorithm>
#include<vector>
void merge_arr(std::vector<int>& arr,int left,int mid,int right)
{
    std::vector<int>left_arr(arr.begin()+left,arr.begin()+mid+1);
    std::vector<int>right_arr(arr.begin()+mid+1,arr.begin()+right+1);
    int i = 0;
    int j = 0;
    int k = left;
    while(i<left_arr.size() && j<right_arr.size())
    {
        if(left_arr[i]<right_arr[j])
        {
            arr[k] = left_arr[i];
            k++;
            i++;
        }
        else
        {
            arr[k] = right_arr[j];
            k++;
            j++;
        }
    }
    while(i<left_arr.size())
    {
        arr[k] = left_arr[i];
        k++;
        i++;
    }
    while(j<right_arr.size())
    {
        arr[k] = right_arr[j];
        k++;
        j++;
    }
}
void merge_sort_multi(std::vector<int>& arr,int left,int right)
{
    if(left >= right || right - left <= 4096) // 没有后半句会炸
    //必须限制什么时候用多线程，因为当数足够小的时候，为了溜去开线程浪费的时间方而更多，而且会出现段错误

    //！这个深度的限制很重要，之前是了一下32,发现多线程居然比但线程曼3倍，因为限制力度不够大，创线程太费时间了
    //2048多线程快3倍左右
    //When you are 4096的时候，快了甚至7倍！
    {
        if(left<right) std::sort(arr.begin()+left,arr.begin()+right+1);
        return;

    }
    int mid = (left + right) / 2;
    std::thread left_thread(merge_sort_multi,std::ref(arr),left,mid);
    std::thread right_thread(merge_sort_multi,std::ref(arr),mid+1,right);

    left_thread.join();
    right_thread.join();
    merge_arr(arr,left,mid,right);
}
void multi_thread_sort(std::vector<int>& arr)
{
    if(arr.empty())
    {
        return;
    }
    merge_sort_multi(arr,0,arr.size() - 1);
}
