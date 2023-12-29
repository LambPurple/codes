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

    // ʹ�� op��ȷ�� guard ���ᱻ��ǰ����
    std::cout << "Using op: " << op << std::endl;
}