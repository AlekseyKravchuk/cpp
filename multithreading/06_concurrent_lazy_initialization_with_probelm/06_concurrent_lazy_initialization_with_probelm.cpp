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
    std::mutex _mtx;       // this mutex is used to provide thread-safety when writing to std::ofstream
    std::mutex _mtx_open;  // this mutex is used to provide thread-safety when opening the file
    std::ofstream _out;    // защищаемый ресурс
};

LogFile::LogFile(std::string fname)
    : _fname(fname) {
    // в данном случае мы не хотим открывать файл заблаговременно- хотим это сделать только в случае необходимости,
    // например, если метод "shared_print" ни разу не будет вызван, то и нет необходимости открывать файл.
    // Мы хотим открыть файл только в том случае, если метод "shared_print" будет вызван
    // _out.open(_fname);

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
    // хотим открыть файл для записи только перед непосредственным использованием
    // ===> "Lazy Initialization" or "Initialization Upon First Use" idiom <===

    // Now the code is thread-safe
    // But it introduces another problem:
    // The file only needs to be opened once, but now every time we call "LogFile::shared_print()",
    // the code will 1)lock the mutex, 2)check if ifle is open, 3) unlock the mutex.
    // So:
    // 1) we are wasting computer cycles;
    // 2) extra useless locking and unlocking of mutex hinder(препятствует) the program being run concurrently.
    if (std::lock_guard<mutex> open_guard(_mtx_open); !_out.is_open()) {
        _out.open(_fname);
    }

    std::lock_guard<mutex> guard(_mtx);

    // thread-safe access to "std::ofstream"
    _out << "From " << id << ": " << value << endl;
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
