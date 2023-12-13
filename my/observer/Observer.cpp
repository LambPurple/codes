// 观察者模式 ，也叫发布-订阅模式
// subject发布，不同的观察者订阅
/*
要求：
1.观察者可以订阅某一个模式-->多对一
2.当观察者的生命周期消失后，所订阅的主题中应该去掉该订阅者

问题：
1.在使用find函数时，由于没有包含<algorithm>库，
    导致找到<iostream>中的另一个find()函数，报错
2.在声明类的时候，编译器还不能确定该类内部有什么东西，所以
    声明和实现应该分开实现,即不能使用不完全的类型
3.发布者和订阅者的生命周期不能控制，在多线程环境下可能会造成内存泄漏
*/ 
#include <vector>
#include <iostream>
#include <algorithm>
using std::cout;
using std::endl;

class Observable;

// 被观察的对象
class Observer
{
public:
    virtual ~Observer();
    virtual void update() = 0;

    void observe(Observable *s);

protected:
    Observable *subject_;
};


//观察者
class Observable{
public:
    //注册和取消注册
    void register_(Observer* x);
    void unregister(Observer* x);

    //通知所有的观察者
    void notifyObservers(){
        for(auto &p : observers_){
            if(p){
                p->update();
            }
        }
    }

private:
    std::vector<Observer*> observers_;
};

Observer::~Observer(){
    subject_->unregister(this);
}

void Observer::observe(Observable* s){
    s->register_(this);
    subject_ = s;
}

void Observable::register_(Observer* x){
    observers_.push_back(x);
}

void Observable::unregister(Observer* x){
    auto it = std::find(observers_.begin(),observers_.end(),x);
    if(it != observers_.end()){
        std::swap(*it,observers_.back());
        observers_.pop_back();
    }
}

// -------------------
class Foo : public Observer{
    void update() override{
        cout << "I am Foo" << endl;
    }
};

int main(){
    Foo *p = new Foo;
    Observable subject;
    p->observe(&subject);
    subject.notifyObservers();
    delete p;
    subject.notifyObservers();

    return 0;
}