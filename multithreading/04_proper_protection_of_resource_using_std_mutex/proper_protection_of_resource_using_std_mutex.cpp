#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex> // std::mutex, std::lock_guard
#include <thread>

using namespace std;

/*
======== Avoiding Data Race ========
1. use mutex to synchronize data access;
2. never leak an address (pointer or reference) of protected(by mutex) data to outside;
3. design interface appropriately.
*/

// Для полной защиты ресурса std::mutex должен быть объединен (must be bundled together) с ресурсом, который он защищает
class LogFile {
  public:
    LogFile(std::string fname);
    ~LogFile();
    void shared_print(std::string id, int value);

  private:
    std::string _fname;
    std::mutex _mtx;
    std::ofstream _out; // защищаемый ресурс

    // ПРИМЕЧАНИЕ: подразумевается, что
    //                   - мы не будем выдавать наружу ссылку на защищаемый ресурс "_out";
    //                   - мы не позволим принимать ссылку на "_out" в качестве параметра user-defined callback'a
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

    cout << R"(File ")"<< _fname << R"(" is closed.)" << endl;
}

void LogFile::shared_print(string id, int value) {
    std::lock_guard<mutex> guard(_mtx); // блокируем мьютекс-член класса LogFile

    // по факту получаем доступ к объекту через мьютекс
    _out << "From " << id << ": " << value << endl;
}

// пишем в лог-файл
void fun_1(LogFile& log) {
    for (int i = 0; i > -100; --i) {
        log.shared_print(string("From fun_1: "), i);
    }
}

// пишем в лог-файл
void fun_2(LogFile& log) {
    for (int i = 0; i < 100; ++i) {
        log.shared_print(string("From fun_2: "), i);
    }
}

int main() {
    LogFile log("log.txt");
    std::thread t1(fun_1, std::ref(log));
    std::thread t2(fun_2, std::ref(log));

    t1.join();
    t2.join();

    return 0;
}
