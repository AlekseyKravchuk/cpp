#pragma once

#include <chrono>
#include <iostream>

using namespace std::literals;

#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)
#define PROFILE_CONCAT_INTERNAL(X, Y) X ## Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profile_guard_, __LINE__)

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
