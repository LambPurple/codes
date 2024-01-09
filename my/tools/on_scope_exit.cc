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
class on_scope_exit_
{
public:
    explicit on_scope_exit_(T&& t) : release(forward<T>(t)){} 

    ~on_scope_exit_()
    {
        release();
        cout << "deconstruct" << endl;
    }

private:
    T release;
};

#define on_scope_exit(args) auto __##arg \
                            on_scope_exit_<function<decltype(args)>> __##args(args)


void func(){
    close();
}


int main()
{
    
    int op = open();
    auto x = [&](){close();};
    // on_scope_exit_<function<decltype(x)>> p(x);
    // on_scope_exit([](){close();});
    // 使用 op，确保 guard 不会被提前销毁
    std::cout << "Using op: " << op << std::endl;
}