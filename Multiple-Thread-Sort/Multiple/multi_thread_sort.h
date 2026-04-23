#ifndef MULTI_THREAD_SORT_H
#define MULTI_THREAD_SORT_H
#include<vector>
#include<thread>

void merge_arr(std::vector<int>& arr,int left,int mid,int right);
void merge_sort_multi(std::vector<int>& arr,int left,int right);
void multi_thread_sort(std::vector<int>& arr);






#endif