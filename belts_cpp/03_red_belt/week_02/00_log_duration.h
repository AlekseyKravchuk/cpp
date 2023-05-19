#pragma once

#include <chrono>
#include <iostream>
#include <string>

using namespace std::literals;

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
            << _message
            << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
            << " milliseconds"s << std::endl;
    }

   private:
    std::string _message{};
    std::chrono::steady_clock::time_point _start;
};