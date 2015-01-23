#include <bits/stdc++.h>

using namespace std;

struct Semaphore {
    mutex mtx;
    volatile int val;
    Semaphore(int val=0) :val(val) {}
    void inc() {
        mtx.lock();
        ++val;
        mtx.unlock();
    }
    void dec() {
        while(1) {
            mtx.lock();
            if( val > 0 ) {
                --val;
                mtx.unlock();
                break;
            }
            mtx.unlock();
        }
    }
    void signal() { inc(); }
    void wait() { dec(); }

    void V() { inc(); }
    void P() { wait(); }
} crit(1), turnstile, turnstile2(1);

//Reusable Barrier Pattern

const int N = 10;
int cnt=0;

void f(int id) {
    cout << id << ": ready for party\n";
    
    crit.wait();
        cnt += 1;
        if(cnt==N) {
            turnstile2.wait();
            turnstile.signal();
        }
    crit.signal();

    turnstile.wait();
    turnstile.signal();

    cout << id << ": party!\n";
    
    crit.wait();
        cnt -= 1;
        if(cnt==0) {
            turnstile.wait();
            turnstile2.signal();
        }
    crit.signal();

    turnstile2.wait();
    turnstile2.signal();
}

int main() {
    vector<thread> v;
    for(int i=1; i<=N; ++i)
        v.push_back(thread(f, i));
    for(auto& th: v) th.join();
    return 0; 
}
