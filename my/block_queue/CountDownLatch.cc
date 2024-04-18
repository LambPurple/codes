#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <iostream>

using std::cout,std::endl;


/**
 * @brief ���������ļ�ʹ��
 * */ 
class CountDownLatch{
public:
    explicit CountDownLatch(int count);
    void wait();//�ȵ�������Ϊ0ʱ
    void countDown();//��������1

private:
    mutable std::mutex mtx;
    std::condition_variable cond;
    int cnt;
    static int timer;
};

int CountDownLatch::timer = 0;

CountDownLatch::CountDownLatch(int count) : cnt(count){

}

//����ʱ����������߳�������
void CountDownLatch::wait(){
    std::unique_lock<std::mutex> lck(mtx);
    while(cnt > 0){
        cond.wait(lck);
    }
    //todo 
    cout << timer ++ << endl;
}

//ÿ����һ�Σ�cnt--
void CountDownLatch::countDown(){
    std::unique_lock<std::mutex> lck(mtx);
    -- cnt;
    if(cnt == 0)cond.notify_all();
}

int main(){
    CountDownLatch cnt(3);
    for(int i = 0;i < 5;i ++)//������߳��ڵȴ�����ʱ����
        std::thread(&CountDownLatch::wait,&cnt).detach();//����Ա�����󶨵������ϵķ�ʽ1

    for(int i = 0;i < 3;i ++)//�������δ�cnt --
        std::thread(std::bind(&CountDownLatch::countDown,&cnt)).detach();//��ʽ2

    auto x = std::bind(&CountDownLatch::countDown,&cnt);
    while(1);
    return 0;
}
