#ifndef SINGLE_THREAD_SORT_H
#define SINGLE_THREAD_SORT_H
#include<vector>

std::vector<int> merge(const std::vector<int>& left , const std::vector<int>& right );
void merge_sort_single(std::vector<int>& arr,int left,int right);//left right是下标
void single_thread_sort(std::vector<int>& arr);

#endif
