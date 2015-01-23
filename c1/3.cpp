#include <bits/stdc++.h>

using namespace std;

struct Semaphore {
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
} mtx(1);

int cnt = 0;

void fa() {
    mtx.wait();
    cnt = cnt + 1;
    mtx.signal();
}

void fb() {
    mtx.wait();
    cnt = cnt + 1;
    mtx.signal();
}

int main() {
    thread b(fb);
    thread a(fa);
    cout << "main, run a, b...\n";
    a.join();
    b.join();
    cout << "cout = " << cnt << "\n";
    return 0; 
}
