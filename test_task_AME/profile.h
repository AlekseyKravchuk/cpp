#pragma once

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>

#define GENERATE(x) a_local_var_##x
#define PROXY(x) GENERATE(x)
#define UNIQ_ID PROXY(__LINE__)
#define LOG_DURATION(message) LogDuration UNIQ_ID{message};

class LogDuration {
  public:
    explicit LogDuration(const std::string& message = "")
            : _message(message + ": "),
              _start(std::chrono::high_resolution_clock::now()) {}

    ~LogDuration() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = end - _start;
        std::cout << std::left <<std::setw(35) << _message
                  << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
                  << " milliseconds" << std::endl;
    }

  private:
    std::string _message;
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
};