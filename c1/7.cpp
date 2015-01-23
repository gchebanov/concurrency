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

struct Queue {
    Semaphore mtx;
    Semaphore followers;
    queue<int> q;
    Queue() :mtx(1), followers(0) {}

    void push(int id) {
        mtx.wait();
            q.push(id);
            followers.signal();
        mtx.signal();
    }

    int pop() {
        followers.wait();
        mtx.wait();
            int r = q.front();
            q.pop();
        mtx.signal();
        return r;
    }

} Q;

struct DanceQueue {
    Semaphore leaderQueue, followerQueue;
    DanceQueue():leaderQueue(0), followerQueue(0) {}

    void leader(int id) {
        leaderQueue.signal();
        followerQueue.wait();
        io_lock.lock();
            cout << "dance " << id << '\n';
        io_lock.unlock();
    }
    void follower(int id) {
        followerQueue.signal();
        leaderQueue.wait();
        io_lock.lock();
            cout << "dance " << id << '\n';
        io_lock.unlock();
    }
} dQ;

void f(int id) {
    if(id>N/2) {
        Q.push(id);
        //!
        dQ.leader(id);
    } else {
        int r = Q.pop();
        io_lock.lock();
            cout << "Qdance " << id << ' ' << r << '\n';
        io_lock.unlock();
        //!
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
