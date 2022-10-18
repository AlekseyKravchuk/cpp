#pragma once

#include <chrono>
#include <iostream>

using namespace std::literals;

class LogDuration {
public:
    // заменим имя типа std::chrono::steady_clock с помощью using для удобства
    using Clock = std::chrono::steady_clock;

    LogDuration() { }
    LogDuration(const std::string operationName) : _operationName(operationName) {}

    ~LogDuration() {
        using namespace std::chrono;
        using namespace std::literals;

        const auto end_time = Clock::now();
        const auto dur = end_time - _startTime;
        std::cerr << ": "s << duration_cast<milliseconds>(dur).count() << " ms"s << std::endl;
    }

private:
    const Clock::time_point _startTime = Clock::now();
    const std::string _operationName = ""s;
};
