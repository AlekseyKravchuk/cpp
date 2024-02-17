#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>  // std::mutex, std::lock_guard
#include <thread>

using namespace std;

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
    std::lock_guard<mutex> guard_Sec(_mtx_security);    // <== 1
    std::lock_guard<mutex> guard_FS(_mtx_file_system);  // <== 2

    // _out << "From " << id << ": " << value << endl;
    cout << "From " << id << ": " << value << endl;
}

// для предотвращения deadlock-ов, нужно убедиться в том, что блокировка мьютексов происходит в одном и том же порядке
void LogFile::shared_print_2(string id, int value) {
    // !!! DEADLOCK !!! ()
    std::lock_guard<mutex> guard_FS(_mtx_file_system);  // <== 2
    std::lock_guard<mutex> guard_Sec(_mtx_security);    // <== 1

    // _out << "From " << id << ": " << value << endl;
    cout << "From " << id << ": " << value << endl;
}

// пишем в лог-файл
void function_1(LogFile& log) {
    for (int i = 0; i > -100; --i) {
        log.shared_print(string("From fun_1: "), i);
    }
}

int main() {
    LogFile log("log.txt");
    std::thread t1(function_1, std::ref(log));

    for (int i = 0; i < 100; ++i) {
        log.shared_print_2(std::string("From main: "), i);
    }

    t1.join();

    return 0;
}
