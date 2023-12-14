//�����

/*
���⣺
1.����ת���ڴ˴��ܷ����ϣ�
2.stocks_�Ĵ�С�ڳ��������ڼ�ֻ������
    Ӧ�ü�ʱ������Ч��stock�����weak_ptr
3.factory��stock��������������
*/

#include <map>
#include <string>
#include <memory>
#include <mutex>
#include <iostream>

using std::cout;
using std::endl;

//��Ʊ��
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