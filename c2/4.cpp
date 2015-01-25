#include <bits/stdc++.h>

using namespace std;

mutex io;

void print_hi(int id, const string& hello) {
   io.lock();
   cout << id << ": " << hello << '\n';
   io.unlock();
}

template<int N>
class PhilosophersProblem {
public:
    PhilosophersProblem() {
        for(int i = 0 ; i < N; ++i) {
            can_eat[i].lock();
            status[i] = 't';
        }
    }
    void get_fork(int i) {
        mtx.lock();
            status[i] = 'h';
            test(i);
        mtx.unlock();
        can_eat[i].lock();
    }
    void put_fork(int i) {
        mtx.lock();
            status[i] = 't';
            test(right(i));
            test(left(i));
        mtx.unlock();
    }
private:
    void test(int i) {
        if( status[i] == 'h' &&
            status[left(i)] != 'e' &&
            status[right(i)] != 'e' ) {
            status[i] = 'e';
            can_eat[i].unlock();
        }
    }
    int left(int i) {
        return (i + N - 1) % N;
    }
    int right(int i) {
        return (i + 1) % N;
    }
    char status[N];
    mutex can_eat[N];
    mutex mtx;
};

PhilosophersProblem<5> pl;

void philosophers(int id) {
    srand(id);
    int cnt = 100000;
    while( cnt --> 0 ) {
        ///*
            print_hi(id, "thinking....");
            this_thread::sleep_for(chrono::milliseconds(1+rand()%100));
        //*/
        pl.get_fork(id%5);
        ///*
            print_hi(id, "eating....");
            this_thread::sleep_for(chrono::milliseconds(1+rand()%100));
        //*/
        pl.put_fork(id%5);
    }
}

int main() {
    thread t1(philosophers,1);
    thread t2(philosophers,2);
    thread t3(philosophers,3);
    thread t4(philosophers,4);
    thread t5(philosophers,5);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    return 0; 
}
