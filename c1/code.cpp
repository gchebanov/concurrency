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
} a1done;

void fa() {
    cout << "a1\n";
    a1done.signal();
}

void fb() {
    a1done.wait();
    cout << "b1\n";
}

int main() {
    thread b(fb);
    thread a(fa);
    cout << "main, run a, b...\n";
    a.join();
    b.join();
    cout << "completed a and b.\n";
    cout << "a1 always runs before b1.\n";
    return 0; 
}
