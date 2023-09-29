/*
Jacek Galowicz - C++17 STL Cookbook (2017)
Chapter 9. Starting and stopping threads.
*/

#include <iostream>
#include <thread>

using namespace std::chrono_literals;

class Thread_ID {
   public:
    Thread_ID(int id = 0) : _id(id){};

    int GetThreadID() {
        return _id;
    }

   private:
    int _id;
};

static void thread_with_param(Thread_ID thr_id) {
    std::this_thread::sleep_for(1ms * thr_id.GetThreadID());
    std::cout << "Hello from thread " << thr_id.GetThreadID() << '\n';

    std::this_thread::sleep_for(1s * thr_id.GetThreadID());
    std::cout << "Bye from thread " << thr_id.GetThreadID() << '\n';
}

int main() {
    // В консоли Linux информацию о количестве ядер CPU можно получить с помощью следующей команды:
    // lscpu | egrep 'Model name|Socket|Thread|NUMA|CPU\(s\)'
    /*
        CPU(s):                             12
        On-line CPU(s) list:                0-11
        Thread(s) per core:                 2
        Socket(s):                          1
        NUMA node(s):                       1
        Model name:                         AMD Ryzen 5 3600 6-Core Processor
        NUMA node0 CPU(s):                  0-11
    */

    // "std::thread::hardware_concurrency()"
    std::cout << std::thread::hardware_concurrency()
              << " concurrent threads are supported.\n";  // OUTPUT: 12 concurrent threads are supported. <= количество ЛОГИЧЕСКИХ ядер

    std::thread t1{thread_with_param, Thread_ID{1}};
    std::thread t2{thread_with_param, Thread_ID{2}};
    std::thread t3{thread_with_param, Thread_ID{3}};

    // Вызов "join()" ЗАБЛОКИРУЕТ ТЕКУЩИЙ (вызываюЩИЙ) поток до тех пор, пока вызываеМЫЙ поток не отработает
    t1.join();
    t2.join();

    
    t3.detach();

    std::cout << "Threads joined.\n";

    return 0;
}