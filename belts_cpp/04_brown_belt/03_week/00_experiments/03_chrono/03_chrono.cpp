#include <iostream>
#include <chrono>
#include <set>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace std::this_thread; // sleep_for, sleep_until

struct Priority {
    string key;
    chrono::time_point<chrono::steady_clock> time;
};

struct PriorityLess {
    bool operator()(const Priority& lhs, const Priority& rhs) const {
        return lhs.time < rhs.time;
    }
};

int main() {
    set<Priority, PriorityLess> use_times;
    use_times.insert(Priority{"one", chrono::steady_clock::now()});
    sleep_for(std::chrono::milliseconds(500));

    use_times.insert(Priority{"one", chrono::steady_clock::now()});
    sleep_for(std::chrono::milliseconds(500));

    cout << "END" << endl;
    
    return 0;
}
