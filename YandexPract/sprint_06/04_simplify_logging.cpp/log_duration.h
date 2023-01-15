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

        const auto endTime = Clock::now();
        const auto dur = endTime - _startTime;
        std::cerr << _operationName << ": "s << std::chrono::duration_cast<std::chrono::milliseconds>(dur).count() << " ms"s << std::endl;
    }

private:
    const Clock::time_point _startTime = Clock::now();
    const std::string _operationName = ""s;
};
