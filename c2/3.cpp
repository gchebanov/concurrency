#include <bits/stdc++.h>

using namespace std;

mutex io;

void print_hi(int id, const string& hello) {
   io.lock();
   cout << id << ": " << hello << '\n';
   io.unlock();
}

class ReadersWriters {
public:
    ReadersWriters() {}
    template<typename Tf>
    void read(Tf f) {
        {
            unique_lock<mutex> l(mtx);
            no_writers_wait.wait(l, [this](){return writers==0;});
        }
        {
            lock_guard<mutex> l(mtx);
            readers += 1;
            if( readers == 1 ) {
                is_empty.lock();
            }
        }
        f();
        {
            lock_guard<mutex> l(mtx);
            readers -= 1;
            if( readers == 0 ) {
                is_empty.unlock();
            }
        }
    }
    template<typename Tf>
    void write(Tf f) {
        {
            lock_guard<mutex> l(mtx);
            writers += 1;
        }
        {
            lock_guard<mutex> l(is_empty);
            f();
        }
        {
            lock_guard<mutex> l(mtx);
            writers -= 1;
            if( writers == 0 ) 
                no_writers_wait.notify_all();
        }
    }
private:
    mutex mtx;
    mutex is_empty;
    int readers;
    int writers;
    condition_variable no_writers_wait;
} rw;


void reader(int id, int cnt) {
    while( cnt --> 0 ) {
        rw.read([&id,&cnt]() {
            print_hi(id, "reading " + to_string(cnt));
            this_thread::sleep_for(chrono::milliseconds(100));
            print_hi(id, "stop reading " + to_string(cnt));
        });
        this_thread::yield();
    }
}

void writer(int id, int cnt) {
   while ( cnt --> 0 ) {
        rw.write(
            [&id,&cnt]() {
                print_hi(id, "write " + to_string(cnt));
                this_thread::sleep_for(chrono::milliseconds(250));
                print_hi(id, "stop write " + to_string(cnt));
            }
        );
        this_thread::sleep_for(chrono::milliseconds(250));
        this_thread::yield();
   }
}

int main() {
    thread t1(reader,1,5);
    thread t2(reader,2,5);
    thread t3(writer,3,3);
    t1.join();
    t2.join();
    t3.join();
    return 0; 
}
