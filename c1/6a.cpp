#include <bits/stdc++.h>

using namespace std;

const int N = 10;

struct Semaphore {
    mutex mtx;
    volatile int val;
    Semaphore(int val=0) :val(val) {}
    void signal(int dval = 1) {
        mtx.lock();
        val += dval;
        mtx.unlock();
    }
    void wait() {
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
};

struct Barrier {
    int n, cnt;
    Semaphore mtx,turnstile,turnstile2;
    Barrier(int n) :n(n), cnt(0), mtx(1), turnstile(0), turnstile2(1) { }
    void phase1() {
        mtx.wait();
            cnt += 1;
            if( cnt == n ) {
                turnstile.signal(n);
            }
        mtx.signal();
        turnstile.wait();
    }
    void phase2() {
        mtx.wait();
            cnt -= 1;
            if( cnt == 0 ) {
                turnstile2.signal(n);
            }
        mtx.signal();
        turnstile2.wait();
    }
} bar(N);

void f(int id) {
    cout << id << ":a\n";
    bar.phase1();
    cout << id << ":b\n";
    bar.phase2();
    cout << id << ":c\n";
}

int main() {
    vector<thread> v;
    for(int i=1; i<=N; ++i)
        v.push_back(thread(f, i));
    for(auto& th: v) th.join();
    return 0; 
}
