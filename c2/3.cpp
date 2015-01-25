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
            readers += 1;
        }
        f();
        {
            unique_lock<mutex> l(mtx);
            readers -= 1;
        }
        can_write.notify_one();
    }
    template<typename Tf>
    void write(Tf f) {
        unique_lock<mutex> l(mtx);
        can_write.wait(l, [this](){return readers==0;});
        f();
    }
private:
    mutex mtx;
    int readers;
    condition_variable can_write;
} rw;


void reader(int id, int cnt) {
    while( cnt --> 0 ) {
        rw.read([&id,&cnt]() {
            print_hi(id, "reading " + to_string(cnt));
            this_thread::sleep_for(chrono::milliseconds(100));
            print_hi(id, "stop reading " + to_string(cnt));
        });
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
   }
}

int main() {
    thread t1(reader,1,10);
    thread t2(reader,2,10);
    thread t3(writer,3,3);
    t1.join();
    t2.join();
    t3.join();
    return 0; 
}
