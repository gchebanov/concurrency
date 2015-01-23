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
} sem(2);

//Multiplex Pattern

void f(int id) {
    cout << id << ": prepare for party\n";
    sem.wait();
    cout << id << ": party!\n";
    this_thread::yield();
    sem.signal();
    cout << id << ": go away\n";
}

int main() {
    vector<thread> v;
    for(int i=1; i<=10; ++i)
        v.push_back(thread(f, i));
    for(auto& th: v) th.join();
    return 0; 
}
