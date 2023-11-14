#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std::literals;

#define UNIQUE_ID_HELPER(lineno) _a_local_var_##lineno
#define UNIQUE_ID(lineno) UNIQUE_ID_HELPER(lineno)

#define LOG_DURATION(message) \
    LogDuration UNIQUE_ID(__LINE__){message};

class LogDuration {
   public:
    // поскольку конструктор у нас от одного параметра, нужно сделать его explicit
    explicit LogDuration(std::string message = ""s)
        : _message(message + ": "s),
          _start(std::chrono::steady_clock::now()) {}

    ~LogDuration() {
        auto finish = std::chrono::steady_clock::now();
        auto duration = finish - _start;
        std::cerr
            << std::left << std::setw(40) << _message
            << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
            // << std::chrono::duration_cast<std::chrono::microseconds>(duration).count()
            // << " microseconds"s << std::endl;
            << " milliseconds"s << std::endl;
    }

   private:
    std::string _message{};
    std::chrono::steady_clock::time_point _start;
};