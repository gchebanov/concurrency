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
} crit(1), bar(0);

//Barrier Pattern

const int N = 10;
int cnt=0;

void f(int id) {
    cout << id << ": ready for party\n";
    
    crit.wait();
        cnt += 1;
        if(cnt==N)
            while(cnt-->0)
                bar.signal();
    crit.signal();
    bar.wait();
    cout << id << ": party!\n";
}

int main() {
    vector<thread> v;
    for(int i=1; i<=N; ++i)
        v.push_back(thread(f, i));
    for(auto& th: v) th.join();
    return 0; 
}
