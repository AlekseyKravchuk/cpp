#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>  // std::mutex, std::lock_guard
#include <thread>

using namespace std;
using namespace std::chrono_literals;

const int NUM_ENTRIES_TO_WRITE = 1'000;

class LogFile {
   public:
    LogFile(std::string fname);
    ~LogFile();
    void shared_print(std::string id, int value);

   private:
    std::string _fname;
    std::mutex _mtx;
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


void LogFile::shared_print(string id, int value) {
    std::lock_guard<mutex> guard(_mtx);

    // становимся владельцем мьютекса "_mtx", но откладываем его блокировку (второй параметр - defer_lock)
    //  std::unique_lock является более тяжелым с точки зрения производительности
    std::unique_lock<std::mutex> unique_guard(_mtx, std::defer_lock);

    // выполняем необходимые действия, которые не требуют записи в std::ofstream, т.е. не трубуют блокирования мьютекса
    // ===================================================================================
    int count = 0;
    for (int i = 0; i < 1'000'000; ++i) {
        ++count;
    }
    cout << "count = " << count << endl;
    // ===================================================================================

    // перед тем как начать писать в std::ofstream, нужно не забыть заблокировать мьютекс
    unique_guard.lock();

    // пишем в std::ofstream
    _out << "From " << id << ": " << value << endl;

    unique_guard.unlock();

    // И std::lock_guard<>, и std::unique_lock<> не могут быть скопированы.
    // Однако мы можем передать владение от одного std::unique_lock<> другому
    std::unique_lock locker_2 = std::move(unique_guard);

    // снова поделали какую-то работу, не требующую блокирования мьютекса

    // снова блокируем мьютекс
     unique_guard.lock();
}


void fun_1(LogFile& log) {
    for (int i = 0; i > -NUM_ENTRIES_TO_WRITE; --i) {
        log.shared_print(string("fun_1"), i);
    }
}


int main() {
    LogFile log("log.txt");

    // пишем в лог-файл - выделяем для этого отдельный поток
    std::thread t1(fun_1, std::ref(log));

    // снова пишем в лог-файл, но уже из основного потока
    for (int i = 0; i < NUM_ENTRIES_TO_WRITE; ++i) {
        log.shared_print(std::string("main"), i);
    }

    t1.join();

    return 0;
}
