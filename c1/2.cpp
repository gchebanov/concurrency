#include <bits/stdc++.h>

using namespace std;

struct Semaphore {
    //mutex mtx;
    volatile int val;
    Semaphore(int val=0) :val(val) {}
    void inc() {
        ++val;
    }
    void dec() {
        --val;
        while(val<0);
    }
    void signal() { inc(); }
    void wait() { dec(); }

    void V() { inc(); }
    void P() { wait(); }
} aArrived, bArrived;

void fa() {
    cout << "a1\n";
    aArrived.signal();
    bArrived.wait();
    cout << "a2\n";
}

void fb() {
    cout << "b1\n";
    bArrived.signal();
    aArrived.wait();
    cout << "b2\n";
}

int main() {
    thread b(fb);
    thread a(fa);
    cout << "main, run a, b...\n";
    a.join();
    b.join();
    cout << "completed a and b.\n";
    cout << "a2,b2 always runs after a1,b1.\n";
    return 0; 
}
