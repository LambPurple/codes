#include <iostream>
#include <thread>

#include "BlockQueue.h"
#include "BlockQueue.cpp"

using namespace std;

int main(){
    BlockQueue<int> bq(10);
    const int N = 100;
    cout << "#####测试开始#####" << endl;
    thread([&]() {
        for (int i = 0; i < N; i++) {
            bq.put(i);
        }
    }).detach();
    thread([&]() {
        for (int i = 0; i < N; i++) {
            bq.take();
        }
    }).detach();
    while (true);
    return 0;
}