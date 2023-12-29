#include <bits/stdc++.h>


/*
实现on_scope_exit函数，调用该函数，可以将确保fd的生命周期结束后，一定会调用close(fd)函数
{
    int fd = open();
    on_scope_exit([&fd](){close(fd);})
}

*/

using namespace std;

int open()
{
    cout << "open()" << endl;
    return 0;
}
int close()
{
    cout << "close()" << endl;
    return 0;
}

template<typename T>
class on_scope_exit
{
public:
    explicit on_scope_exit(T&& t) : release(forward<T>(t)){} 

    ~on_scope_exit()
    {
        release();
        cout << "deconstruct" << endl;
    }

private:
    T release;
};

int main()
{
    int op = open();
    auto fun = [&op](){ close(); };
    // on_scope_exit<decltype(fun)> guard(fun);

    // 使用 op，确保 guard 不会被提前销毁
    std::cout << "Using op: " << op << std::endl;
}