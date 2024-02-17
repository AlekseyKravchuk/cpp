#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>  // std::mutex, std::lock_guard, std::once_flag
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
    std::once_flag _flag;  // используем вместо мьютекса для открытия файла
    std::ofstream _out;    // защищаемый ресурс
};

LogFile::LogFile(std::string fname)
    : _fname(fname) {

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
    // now our method "shared print" is both efficient, thread-safe and accurate
    std::call_once(_flag,
                   [this](){
                    _out.open(_fname);  // file will be opened only once and only by one thread
                   });

    std::lock_guard<mutex> guard(_mtx);
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
