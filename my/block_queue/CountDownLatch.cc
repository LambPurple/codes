#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <iostream>

using std::cout,std::endl;


/**
 * @brief 条件变量的简单使用
 * */ 
class CountDownLatch{
public:
    explicit CountDownLatch(int count);
    void wait();//等到计数器为0时
    void countDown();//计数器减1

private:
    mutable std::mutex mtx;
    std::condition_variable cond;
    int cnt;
    static int timer;
};

int CountDownLatch::timer = 0;

CountDownLatch::CountDownLatch(int count) : cnt(count){

}

//倒计时结束，多个线程做的事
void CountDownLatch::wait(){
    std::unique_lock<std::mutex> lck(mtx);
    while(cnt > 0){
        cond.wait(lck);
    }
    //todo 
    cout << timer ++ << endl;
}

//每调用一次，cnt--
void CountDownLatch::countDown(){
    std::unique_lock<std::mutex> lck(mtx);
    -- cnt;
    if(cnt == 0)cond.notify_all();
}

int main(){
    CountDownLatch cnt(3);
    for(int i = 0;i < 5;i ++)//有五个线程在等待倒计时结束
        std::thread(&CountDownLatch::wait,&cnt).detach();//将成员函数绑定到对象上的方式1

    for(int i = 0;i < 3;i ++)//调用三次从cnt --
        std::thread(std::bind(&CountDownLatch::countDown,&cnt)).detach();//方式2

    auto x = std::bind(&CountDownLatch::countDown,&cnt);
    while(1);
    return 0;
}
