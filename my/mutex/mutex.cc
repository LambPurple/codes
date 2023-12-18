#include <pthread.h>
#include <assert.h>

#include <iostream>

/**
 * @brief 基于pthread实现了互斥锁
 * 
*/

class MutexLock{
public:
    explicit MutexLock() : holder_(0){
        pthread_mutex_init(&mutex_,NULL);
    }

    ~MutexLock(){
        assert(holder_ == 0);
        pthread_mutex_destroy(&mutex_);
    }

    bool isLockedByThisThread(){
        // return holder_ == C?????
        return false;
    }

    void assertLocked(){
        // assert()
    }

    void lock(){
        pthread_mutex_lock(&mutex_);
        // holder_ = 
    }

    void unlock(){
        holder_ = 0;
        pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t* GetPthreadMutex(){
        return &mutex_;
    }
private:
    pid_t holder_;
    pthread_mutex_t mutex_;
};

/**
 * @brief 使用RAII的风格实现了lock_guard类
*/

class MutexLockGuard{
public:
    MutexLockGuard(MutexLock &mutexLock) : mtx(mutexLock){
        mtx.lock();
    }

    ~MutexLockGuard(){
        mtx.unlock();
    }

private:
    MutexLock mtx;
};
//禁止使用临时对象，如MutexLock(x)这样的形式
#define MutexLockGuard(x) static_assert(0 == 1)


/**
 * @brief 实现条件变量
*/

class Condition{
public:
    explicit Condition(MutexLock &mutex) : mtx(mutex){
        pthread_cond_init(&cond,NULL);
    }

    ~Condition(){
        pthread_cond_destroy(&cond);
    }
    void wait(){
        pthread_cond_wait(&cond,mtx.GetPthreadMutex());
    }
    void notify_one(){
        pthread_cond_signal(&cond);
    }
    void notify_all(){
        pthread_cond_broadcast(&cond);
    }
private:
    MutexLock& mtx;
    pthread_cond_t cond;
};



int main(){
    std::cout << "" << std::endl;
    return 0;
}