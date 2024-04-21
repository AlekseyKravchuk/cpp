#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>  // std::mutex, std::lock_guard
#include <thread>

using namespace std;

/*
Avoiding deadlock.
1. Prefer locking single mutex.
2. Avoid locking a mutex and then calling a user-provided function.
3. Use std::lock() to lock more than one mutex.
4. Lock mutex in same order.
*/

class LogFile {
   public:
    LogFile(std::string fname);
    ~LogFile();
    void shared_print(std::string id, int value);
    void shared_print_2(std::string id, int value);

   private:
    std::string _fname;
    std::mutex _mtx_file_system;
    std::mutex _mtx_security;
    std::ofstream _out;  // защищаемый ресурс
};

LogFile::LogFile(std::string fname)
    : _fname(fname) {
    _out.open(_fname);

    if (!_out) {
        throw std::runtime_error("Could not open file \"" + _fname + "\" for writing to.");
    }
}

LogFile::~LogFile() {
    if (_out.is_open()) {
        _out.close();
    }

    cout << R"(File ")" << _fname << R"(" is closed.)" << endl;
}

// для предотвращения deadlock-ов, нужно убедиться в том, что блокировка мьютексов происходит в одном и том же порядке
void LogFile::shared_print(string id, int value) {
    std::lock(_mtx_file_system, _mtx_security);
    std::lock_guard<mutex> guard_Sec(_mtx_security, std::adopt_lock);    // <== 1
    std::lock_guard<mutex> guard_FS(_mtx_file_system, std::adopt_lock);  // <== 2

    // _out << "From " << id << ": " << value << endl;
    cout << "From " << id << ": " << value << endl;
}

// для предотвращения deadlock-ов, нужно убедиться в том, что блокировка мьютексов происходит в одном и том же порядке
void LogFile::shared_print_2(string id, int value) {
    std::lock(_mtx_security, _mtx_file_system);
    
    // Now 
    std::lock_guard<mutex> guard_FS(_mtx_file_system, std::adopt_lock);  // <== 2
    std::lock_guard<mutex> guard_Sec(_mtx_security, std::adopt_lock);    // <== 1

    // _out << "From " << id << ": " << value << endl;
    cout << "From " << id << ": " << value << endl;
}

// пишем в лог-файл
void function_1(LogFile& log) {
    for (int i = 0; i > -100; --i) {
        log.shared_print(string("fun_1"), i);
    }
}

int main() {
    LogFile log("log.txt");
    std::thread t1(function_1, std::ref(log));

    for (int i = 0; i < 100; ++i) {
        log.shared_print_2(std::string("main"), i);
    }

    t1.join();

    return 0;
}
