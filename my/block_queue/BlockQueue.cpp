#include "BlockQueue.h"

//使用了两个条件变量来实现生产者-消费者模型
template <typename T>
size_t BlockQueue<T>::size() const
{
    std::unique_lock<std::mutex> lock(mtx);
    return queue.size();
}

template <typename T>
T BlockQueue<T>::take()
{
    std::unique_lock<std::mutex> lock(mtx);
    while (isEmpty())
    {
        produce.notify_one();
        consume.wait(lock);
    }
    auto x = queue.front();
    std::cout << "取走" << x << std::endl;
    queue.pop_front();
    produce.notify_one();
    return x;
}

template <typename T>
void BlockQueue<T>::put(const T &x)
{
    std::unique_lock<std::mutex> lock(mtx);
    produce.wait(lock, [this]() -> bool
                 { return !isFull(); });
    queue.emplace_back(std::move(x));
    std::cout << "放下" << x << std::endl;
    consume.notify_one();
}


//一种不使用条件变量的方法 -> 使用自旋锁
/*
#define CAN_CONSUME (depth > 0)
#define CAN_PRODUCE (depth < N)
int N = 4;//容量是4
int depth = 0;

mutex mtx;

//spin实现线程同步
void produce(){
retry:
    unique_lock<std::mutex> lck(mtx);
    if(!CAN_PRODUCE){
        lck.unlock();
        goto retry;
    }
    assert(CAN_PRODUCE);
    printf("(");
    ++ depth;
}

void consume(){
retry:
    unique_lock<std::mutex> lck(mtx);
    if(!CAN_CONSUME){
        lck.unlock();
        goto retry;
    }
    assert(CAN_CONSUME);
    printf(")");
    -- depth;
}

*/

/*
#define CAN_CONSUME (depth > 0)
#define CAN_PRODUCE (depth < N)
int N = 4;//容量是4
int depth = 0;

mutex mtx;
condition_variable cond;

//采用spin的思想，使用一个条件变量，一个mutex实现线程同步
void produce(){
    unique_lock<std::mutex> lck(mtx);
    while(!CAN_PRODUCE){
        cond.wait(lck);
    }
    assert(CAN_PRODUCE);
    printf("(");
    ++ depth;
    cond.notify_all();//每次修改需要唤醒所有睡眠中的线程
}

void consume(){
    unique_lock<std::mutex> lck(mtx);
    while(!CAN_CONSUME){
        cond.wait(lck);
    }
    assert(CAN_CONSUME);
    printf(")");
    -- depth;
    cond.notify_all();
}

*/