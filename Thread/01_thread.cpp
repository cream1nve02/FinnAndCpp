#include <iostream>
#include <thread>
using namespace std;

void func1() {
    for (int i = 0 ; i < 10; i++) {
        cout << "Thread 1 Working! \n";
    }
}

void func2() {
    for (int i = 0; i < 10; i++ ) {
        cout << "Thread 2 Working! \n";
    }
}

void func3() {
    for (int i = 0; i< 10; i++) {
        cout << "Thread 3 Working! \n";
    }
}

int main(){
    thread t1(func1);
    thread t2(func2);
    thread t3(func3);

    t1.join();
    t2.join();
    t3.join();
}