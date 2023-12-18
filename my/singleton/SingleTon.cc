#include <stdlib.h>//atexit
#include <pthread.h>
#include <iostream>

/**
 * @brief 实现了懒汉式单例模式
 * @note 懒汉式：是需要的时候才创建实例，可以控制生命周期
 * @note 饿汉式：程序一开始运行的时候，就创建实例，生命周期与主程序一致
*/

template<typename T>
class Singleton{
public:
    Singleton() = delete;
    ~Singleton() = delete;
    Singleton(const Singleton &) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton &&) = delete;

    static T& instance(){
        pthread_once(&ponce,&Singleton::init);
        return *value;
    }

private:
    //保证该函数只会被执行一次
    static void init(){
        value = new T();
        //Register a function to be called when `exit' is called.
        ::atexit(destroy);
    }

    static void destroy(){
        delete value;
    }
private:
    static T* value;
    static pthread_once_t ponce;
};

template<typename T>
T* Singleton<T>::value = NULL;

template<typename T>
pthread_once_t Singleton<T>::ponce = PTHREAD_ONCE_INIT;


using std::endl,std::cout;


int main(){
    auto& x = Singleton<int>::instance();
    auto& y = Singleton<int>::instance();

    x = 21;
    cout << "它们的值是否相同？  ";
    cout << (x == y) << endl;

    cout << &x << endl;
    cout << &y << endl;
    return 0;
}