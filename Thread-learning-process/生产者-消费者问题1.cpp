#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <memory>


// Single-producer , single-consumer Queue
template<class T>
class SPSCQueue 
{
private:
    std::queue<std::unique_ptr<T>> _queue;
    int _capacity;
    std::mutex _HuChiSuo; 
    std::condition_variable _cv;

public:
    explicit SPSCQueue(int capacity);
    virtual bool Push(std::unique_ptr<T>);
    virtual std::unique_ptr<T> pop();
    virtual ~SPSCQueue();
};

// 构造函数的实现
template<class T>
SPSCQueue<T>::SPSCQueue(int capacity) : _capacity(capacity) {}

// 析构函数的实现
template<class T>
SPSCQueue<T>::~SPSCQueue() {}

template<class T>
bool SPSCQueue<T>::Push(std::unique_ptr<T> ptr)
{
    std::unique_lock<std::mutex> lock(_HuChiSuo);//上锁
    while(_queue.size() == _capacity)
    {
        _cv.wait(lock);
    }
    _queue.push(std::move(ptr));
    _cv.notify_all(); 
    return true;
}
template<class T>
std::unique_ptr<T> SPSCQueue<T>::pop()
{
    std::unique_lock<std::mutex> lock(_HuChiSuo);
    while(_queue.empty())
    {
        _cv.wait(lock);
    }
    std::unique_ptr<T> CunChu = std::move(_queue.front());
    _queue.pop();
    _cv.notify_all();
    return CunChu;
    
}
int main()
{
    int newdata = 0;   
    std::cin >> newdata;
    
    SPSCQueue<int> a(5);
    std::unique_ptr<int> data = std::make_unique<int>(666);//创建一个值为666的智能指针
    std::unique_ptr<int> data1 = std::make_unique<int>(newdata);
    a.Push(std::move(data));
    a.Push(std::move(data1));
    
    
    std::unique_ptr<int> result1 = a.pop();
    std::cout << *result1 << std::endl;
    
    
    std::unique_ptr<int> result2 = a.pop();
    std::cout << *result2 <<std::endl;

}
