#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <memory>


//来个全局锁保护一下cout，不然输出第一下容易抢位置
std::mutex _CoutSuo;

//Multi-producer , Multi-consumer Queue
template<class T>
class MPMCQueue 
{
public:
    explicit MPMCQueue(int capacity);
    virtual bool Push(std::unique_ptr<T>) = 0;
    virtual std::unique_ptr<T> pop() = 0;
    virtual ~MPMCQueue() = 0;
};
// 第19行下面加上
template<class T>
MPMCQueue<T>::MPMCQueue(int capacity) {}

template<class T>
MPMCQueue<T>::~MPMCQueue() {}
template<class T>
class MyQueue : public MPMCQueue<T>
{
private:
    std::queue<std::unique_ptr<T>> _queue;
    int _capacity;
    std::mutex _HuChiSuo;
    std::condition_variable _cv;
public:
    explicit MyQueue(int capacity) : MPMCQueue<T>(capacity) {_capacity = capacity;}
    bool Push(std::unique_ptr<T> ptr) override
    {
        std::unique_lock<std::mutex> lock(_HuChiSuo);
        while(_queue.size() == _capacity)
        {
            _cv.wait(lock);
        }
        _queue.push(std::move(ptr));
        _cv.notify_all();
        return true;
    }
    std::unique_ptr<T> pop() override
    {
        std::unique_lock<std::mutex> lock(_HuChiSuo);  // Lock the mutex
        while(_queue.empty())
        {
            _cv.wait(lock);
        }
        std::unique_ptr<T> CunChu = std::move(_queue.front());
        _queue.pop();
        _cv.notify_all();
        return CunChu;
    }
};



void Producer(MPMCQueue<int>& x,int id)
{
    for(int i = 0;i<=20;i++)
    {
        std::unique_ptr<int> data = std::make_unique<int>(i*20);
        x.Push(std::move(data));
        int val = i*20;
        {
            std::unique_lock<std::mutex> lock(_CoutSuo);
            std::cout << "生产者牛马: "<< id <<" "<<"Produced" <<" " << val <<std::endl;
        }
        
    }
}

void Consumer(MPMCQueue<int>&x ,int id)
{
    for(int  i = 0;i<=20;i++)
    {
        std::unique_ptr<int>data  = x.pop();
        {
            std::unique_lock<std::mutex> lock(_CoutSuo);
            std::cout << "牛马: "<< id <<"Produced" << *data <<std::endl;
        }
    }
}

int main()
{
    MyQueue<int> x(20);

    std::thread p1(Producer,std::ref(x),1);
    std::thread p2(Producer,std::ref(x),2);


    std::thread c1(Consumer,std::ref(x),1);
    std::thread c2(Consumer,std::ref(x),2);

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    return 0;

}