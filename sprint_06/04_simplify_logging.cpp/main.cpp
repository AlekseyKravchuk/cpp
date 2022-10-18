#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>

#include "log_duration.h"

using Clock = std::chrono::steady_clock;

int main() {
    // название переменной не играет роли
    LogDuration guard("Total sleeping"s);

    {
        LogDuration guard("Sleeping 1 sec"s);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    {
        LogDuration guard("Sleeping 2 sec"s);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}