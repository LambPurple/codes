//对象池

/*
问题：
1.完美转发在此处能否用上？
2.stocks_的大小在程序运行期间只会增大，
    应该及时析构无效的stock对象的weak_ptr
3.factory与stock的生命周期问题
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
    explicit Stock(const std::string &s) :
        name(s),price(0){}
    explicit Stock(std::string &&s) : 
        name(std::forward<std::string>(s)),price(0){}
    
    ~Stock(){
        cout << "class stock destruction!" << endl;
    }

private:
    std::string name;
    double price;
};

class StockFactory{
public:
    std::shared_ptr<Stock> get(std::string&& s){
        std::shared_ptr<Stock> ptr;
        std::unique_lock<std::mutex> lck(mtx);
        auto &x = stocks_[s];
        ptr = x.lock();
        if(!ptr){
            ptr = std::make_shared<Stock>(s);
            x = ptr;
        }
        return ptr;
    }

private:
    std::map<std::string,std::weak_ptr<Stock>> stocks_;
    std::mutex mtx;
};


int main(){
    {
        auto stock = std::make_shared<Stock>("xiaoming");
    }
    
    auto ptr = new Stock("jklfd");
    delete ptr;
    return 0;
}