#include "single_thread_sort.h"
#include <vector>
std::vector<int> merge(const std::vector<int>& left , const std::vector<int>& right )//合并
{
    std::vector<int> result;
    std::size_t i=0,j=0;
    while(i<left.size() && j<right.size())
    {
        if(left[i]<right[j])
        {
            result.push_back(left[i]);
            i++;
        }
        else
        {
            result.push_back(right[j]);
            j++;
        }
    }
    while(i<left.size())
    {
        result.push_back(left[i]);
        i++;
    }
    while(j<right.size())
    {
        result.push_back(right[j]);
        j++;
    }
    return result;
}
void merge_sort_single(std::vector<int>& arr,int left,int right)//left right是下标
{
    if(left >= right)
    {
        return;
    }
    int mid = (left + right) / 2;
    merge_sort_single(arr,left,mid);
    merge_sort_single(arr,mid+1,right);
    std::vector<int> leftArr(arr.begin() + left,arr.begin() + mid + 1);//            2,5,6,8
    std::vector<int> rightArr(arr.begin() + mid + 1,arr.begin() + right + 1);//      0,1,2,3
    std::vector<int> merged = merge(leftArr,rightArr);//              left(0),...,mid+1(2)
    for(int i = 0;i<merged.size();i++)
    {
        arr[left + i] = merged[i];
    }
}

void single_thread_sort(std::vector<int>& arr)
{
    if(arr.empty())
    {
        return;
    }
    else merge_sort_single(arr,0,arr.size() - 1);
}

