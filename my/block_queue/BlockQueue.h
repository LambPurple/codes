#pragma once

#include <list>
#include <mutex>
#include <condition_variable>


template<typename T>
class BlockQueue {
private:
    //工具函数
    bool isEmpty() const {
        return queue.empty();
    }
    bool isFull() const {
        return queue.size() == max_cnt;
    }

public:
    explicit BlockQueue(size_t cnt = 10) : max_cnt(cnt){}
    size_t size() const;
    T take();
    void put(const T &x) ;

private:
    mutable std::mutex mtx;
    std::condition_variable consume;
    std::condition_variable produce;
    size_t max_cnt;
    std::list<T> queue;
};