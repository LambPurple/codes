// 编写具有线程安全的观察者模式
// 采用shared_ptr来控制对象的生命周期
/*
问题：
1.出现莫名奇妙的std::bad_weak_ptr 
 答： 需要公有继承std::enable_shared_from_this<Observer>
2.如果要将this指针转换成weak_ptr,需要将该类继承
    自std::enable_shared_from_this<T>，调用
    share_from_this()函数可以将this转换成一个weak_ptr的指针


要点：

*/

#include <iostream>
#include <memory>
#include <mutex>
#include <list>

class Observable; // 声明

class Observer : public std::enable_shared_from_this<Observer>
{
public:
    virtual ~Observer() = default;
    virtual void update() = 0;

    void observe(Observable *s); // 订阅主题

protected:
    Observable *subject_;
};

class Observable
{
public:
    void register_(std::weak_ptr<Observer> x);

    // 每次调用该函数会将失效的weak_ptr删除
    void notifyObservers();

private:
    // 引入智能指针来确保对象的生存周期
    std::list<std::weak_ptr<Observer>> observers_;
    // mutex来保证对临界资源的访问是互斥的
    std::mutex mtx;
};

void Observer::observe(Observable *s)
{
    s->register_(shared_from_this());
    subject_ = s;
}

void Observable::register_(std::weak_ptr<Observer> x)
{
    std::unique_lock<std::mutex> lck(mtx);
    observers_.push_back(x);
}

void Observable::notifyObservers()
{
    std::unique_lock<std::mutex> lck(mtx);
    auto it = observers_.begin();
    while (it != observers_.end())
    {
        // 将weak_ptr提升为shared_ptr
        auto p = it->lock();
        if (p)
        {
            p->update();
            ++it;
        }
        else
        {
            // 说明该对象可能在别的线程中析构了
            it = observers_.erase(it);//list的删除更合适
            std::cout << "可能在别的线程中析构了" << std::endl;
        }
    }
}

class XiaoMing : public Observer
{
public:
    void update() override
    {
        std::cout << "该消息发生了变化" << std::endl;
    }
};

int main(){
    Observable subject;
    {
        auto p = std::make_shared<XiaoMing>();
        p->observe(&subject);
        subject.notifyObservers();
    }
    subject.notifyObservers();
    return 0;
}