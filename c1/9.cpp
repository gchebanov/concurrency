#include <bits/stdc++.h>

using namespace std;

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

struct Fifo {
    Semaphore mtx;
    deque<Semaphore*> qfifo;
    Semaphore qsize;
    Fifo(): mtx(1), qsize(0) {}
    void wait() {
        mtx.wait();
            Semaphore* my = new Semaphore(0);
            qfifo.push_back(my);
        mtx.signal();
        qsize.signal();
            my->wait();
            delete my;
    }
    void signal() {
        qsize.wait();
        mtx.wait();
            Semaphore* to_up = qfifo.front();
            qfifo.pop_front();
        mtx.signal();
            to_up->signal();
        //Listing 3.22 from little book of semaphores is broken!
    }
} fifo;

const int N = 10;

mutex io;

void f(int id) {
    if(id<=N/2) {
        io.lock();cout << "wait " << id << '\n';io.unlock();
        fifo.wait();
        io.lock();cout << "raised " << id << '\n';io.unlock();
    } else {
        io.lock();cout << "alarm by " << id << '\n';io.unlock();
        fifo.signal();
        io.lock();cout << "alarmed " << id << '\n';io.unlock();
    }
}

int main() {
    vector<thread> v;
    for(int i=1; i<=N; ++i)
        v.push_back(thread(f, i));
    for(auto& th: v) th.join();
    return 0; 
}
