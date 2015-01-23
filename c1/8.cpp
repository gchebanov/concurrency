#include <bits/stdc++.h>

using namespace std;

mutex io_lock;

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

const int N = 10;

struct DanceQueue {
    Semaphore leaderQueue, followerQueue, mtx, rendezvous;
    int leaders, followers;
    DanceQueue():
        leaderQueue(0), followerQueue(0), mtx(1), rendezvous(0),
        leaders(0), followers(0)
    {}

    void leader(int id) {
        mtx.wait();
            if( followers > 0 ) {
                followers -= 1;
                followerQueue.signal();
            } else {
                leaders += 1;
                mtx.signal();
                leaderQueue.wait();
            }
            io_lock.lock();
                cout << "dance " << id << '\n';
            io_lock.unlock();
            rendezvous.wait();
        mtx.signal();
    }
    void follower(int id) {
        mtx.wait();
        if( leaders > 0 ) {
            leaders -= 1;
            leaderQueue.signal();
        } else {
            followers += 1;
            mtx.signal();
            followerQueue.wait();
        }
        io_lock.lock();
            cout << "dance " << id << '\n';
        io_lock.unlock();
        rendezvous.signal();
    }
} dQ;

void f(int id) {
    if(id>N/2) {
        dQ.leader(id);
    } else {
        dQ.follower(id);
    }
}

int main() {
    vector<thread> v;
    for(int i=1; i<=N; ++i)
        v.push_back(thread(f, i));
    for(auto& th: v) th.join();
    return 0; 
}
