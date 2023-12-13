//解决了智能指针循环引用
//解决了智能指针的拷贝，赋值操作的引用计数的变化

#include <iostream>
#include <memory>


using namespace std;

class B;
class A{
public:
    A() = default;
    ~A(){
        cout << "class A deconstruction!" << endl;
    }
    weak_ptr<B> b;
    
};

class B{
public:
    B() = default;
    ~B(){
        cout << "class B deconstruction!" << endl;
    }
    shared_ptr<A> a;
};

int main(){
    auto a = make_shared<A>();
    cout << a.use_count() << endl;
    auto b = make_shared<B>();
    a->b = b;
    b->a = a;
    cout << a.use_count() << endl;
    return 0;
}