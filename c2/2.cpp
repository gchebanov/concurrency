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
    ProducerConsumer(int capacity = 1) : capacity(capacity) {
        buffer = new TEvent[capacity];
        head = count = tail = 0;
    }
    ~ProducerConsumer() {
        delete[] buffer;
    }
    TEvent get() {
        unique_lock<mutex> l(mtx);
        not_empty.wait(l, [this](){return count != 0;});

        TEvent res = buffer[tail];
        buffer[tail] = TEvent();
        if( ++tail == capacity ) tail = 0;
        --count;

        not_full.notify_one();
        return res;
    }
    void put(const TEvent &e) {
        unique_lock<mutex> l(mtx);
        not_full.wait(l, [this](){return count != capacity;});

        buffer[head] = e;
        if( ++head == capacity ) head = 0;
        ++count;

        not_empty.notify_one();
    }
private:
    size_t capacity;
    size_t head;
    size_t tail;
    size_t count;
    TEvent* buffer;
    mutex mtx;
    condition_variable not_empty;
    condition_variable not_full;
};

ProducerConsumer<int> pc(1000000);

void consumer(int id, int cnt) {
    for(int i = 0; i < cnt; ++i) {
        int val = pc.get();
        //print_hi(id, "Consumer get val=" + to_string(val) );
        //this_thread::sleep_for(chrono::milliseconds(250));
    }
}

void producer(int id, int cnt) {
    for(int i = 0; i < cnt; ++i) {
        pc.put(id*100 + i);
        //print_hi(id, "Producer put val=" + to_string(id*100 + i) );
        //this_thread::sleep_for(chrono::milliseconds(1000/6));
    }
}

int main() {
    thread t4(producer,4,75000);
    thread t5(producer,5,75000);
    thread t1(consumer,1,50000);
    thread t2(consumer,2,50000);
    thread t3(consumer,3,50000);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    return 0; 
}
