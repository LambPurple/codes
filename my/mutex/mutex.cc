#include <pthread.h>
#include <assert.h>

#include <iostream>



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
private:
    pid_t holder_;
    pthread_mutex_t mutex_;
};

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


int main(){
    std::cout << "" << std::endl;
    return 0;
}