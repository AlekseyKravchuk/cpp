#include <iostream>
#include <vector>

#include "log_duration.h"

using namespace std;

void LogVectorParams(const vector<int>& v) {
    cout << "Length = "s << v.size() << ", "
         << "capacity = "s << v.capacity() << endl;

    const int* data = v.data();

    for (size_t i = 0; i < v.capacity(); ++i) {
        cout << *(data + i) << " "s;
    }
    std::cout << std::endl;
}

int main() {
    // vector<int> v = {1, 2, 3};
    // LogVectorParams(v);
    // v.push_back(4);
    // LogVectorParams(v);

    // v.shrink_to_fit();
    // LogVectorParams(v);

    int size = 10'000'000;

    {
        LOG_DURATION("push_back"s);
        vector<int> v;
        for (int i = 0; i < size; ++i) {
            v.push_back(i);
        }
    }

    // {
    //     LOG_DURATION("push_back with reserved space"s);
    //     vector<int> d;
    //     d.reserve(size + 10);
    //     for (int i = 0; i < size; ++i) {
    //         d.push_back(i);
    //     }
    // }

    return 0;
}
