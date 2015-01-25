#include <bits/stdc++.h>

using namespace std;


mutex io;

void print_hi(int id, const string& hello) {
   io.lock();
   cout << id << ": " << hello << '\n';
   io.unlock();
}

template<typename TEvent>
class ProducerConsumer {
public:
    ProducerConsumer() {}
    TEvent get() {
        unique_lock<mutex> l(mtx);
        not_empty.wait(l, [this](){return !buffer.empty();});
        TEvent res(buffer.top());
        buffer.pop();
        return res;
    }
    void put(const TEvent &e) {
        lock_guard<mutex> l(mtx);
        buffer.push(e);
        not_empty.notify_one();
    }
private:
    stack<TEvent> buffer;
    mutex mtx;
    condition_variable not_empty;

};

ProducerConsumer<int*> pc;

const int N = 2;

void f(int id) {
    if(id<=N/2) {
        int cnt = 10;
        //print_hi(id, "try get");
        while( cnt --> 0 ) {
            int* x = pc.get();
            print_hi(id, "-" + to_string(*x));
            delete x;
        }
    } else {
        int cnt = 10;
        while( cnt --> 0 ) {
            int* x = new int;
            *x = id * 10 + cnt;
            print_hi(id, "+" + to_string(*x));
            //this_thread::yield();
            pc.put(x);
        }
    }
}

int main() {
    vector<thread> v;
    for(int i=1; i<=N; ++i)
        v.push_back(thread(f, i));
    for(auto& th: v) th.join();
    return 0; 
}
