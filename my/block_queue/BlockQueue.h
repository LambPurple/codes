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
    explicit BlockQueue(size_t cnt = 10) : max_cnt(cnt) {}

    size_t size() const {
        std::unique_lock<std::mutex> lock(mtx);
        return queue.size();
    }

    //消费者
    T take() {
        std::unique_lock<std::mutex> lock(mtx);
        while (isEmpty()) {
            produce.notify_one();
            consume.wait(lock);
        }
        auto x = queue.front();
        std::cout << "取走" << x << std::endl;
        queue.pop_front();
        produce.notify_one();
        return x;
    }

    //生产者
    void put(const T &x) {
        std::unique_lock<std::mutex> lock(mtx);
        produce.wait(lock,[this]()->bool{return !isFull();});
        queue.emplace_back(std::move(x));
        std::cout << "放下" << x << std::endl;
        consume.notify_one();
    }


private:
    mutable std::mutex mtx;
    std::condition_variable consume;
    std::condition_variable produce;
    size_t max_cnt;
    std::list<T> queue;
};