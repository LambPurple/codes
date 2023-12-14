//对象池

/*
问题：
1.完美转发在此处能否用上？----sloved
2.stocks_的大小在程序运行期间只会增大，
    应该及时析构无效的stock对象的weak_ptr -- sloved
3.factory与stock的生命周期问题 -- sloved
*/

#include <map>
#include <string>
#include <memory>
#include <mutex>
#include <iostream>

using std::cout;
using std::endl;

//股票类
class Stock{
public:
    Stock() = default;
    //完美转发，可以处理左值、右值和常量引用
    template<typename T>
    explicit Stock(T &&t) : name(std::forward<T>(t)),price(0){}

    std::string getName() const{
        return name;
    }
    
    ~Stock(){
        cout << name << " class stock destruction!" << endl;
    }

private:
    std::string name;
    double price;
};

//public继承！！！
class StockFactory : public std::enable_shared_from_this<StockFactory>{
public:
    void removeStock(Stock *stock){
        std::unique_lock<std::mutex> lck(mtx);
        stocks_.erase(stock->getName());
    }

    std::shared_ptr<Stock> getObj(std::string&& s){
        std::shared_ptr<Stock> ptr;
        std::unique_lock<std::mutex> lck(mtx);
        auto &x = stocks_[s];
        ptr = x.lock();
        if(!ptr){
            /*
            传入原生的this指针，可能会导致在多线程环境下，stockFactory对象
            先于stock析构，该函数会访问到不存在的stocks_,会造成core dump
            */
            auto weak_this = std::weak_ptr<StockFactory>(shared_from_this());
            ptr.reset(new Stock(s),
                [weak_this](Stock* stock){
                    if(stock){
                        //c++中的mutex是可重入的，即在上方加锁后并不影响下方中继续加锁
                        //不会导致死锁
                        auto shared_this = weak_this.lock();
                        if(shared_this){
                            shared_this->removeStock(stock);
                        }
                    }
                    delete stock;
                });
            x = ptr;
        }
        return ptr;
    }

private:
    std::map<std::string,std::weak_ptr<Stock>> stocks_;
    std::mutex mtx;
};


int main(){
    std::shared_ptr<Stock> s1;
    auto tmp = std::make_shared<Stock>();
    {
        auto factory = std::make_shared<StockFactory>();
        s1 = factory->getObj("lili");
        auto s2 = factory->getObj("lili");
        cout << (s1 == s2) << endl;
    }
    cout << s1->getName() << endl;
    
    return 0;
}