#include "BlockQueue.h"

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