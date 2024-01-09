#include <bits/stdc++.h>


/*
ʵ��on_scope_exit���������øú��������Խ�ȷ��fd���������ڽ�����һ�������close(fd)����
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
    // ʹ�� op��ȷ�� guard ���ᱻ��ǰ����
    std::cout << "Using op: " << op << std::endl;
}