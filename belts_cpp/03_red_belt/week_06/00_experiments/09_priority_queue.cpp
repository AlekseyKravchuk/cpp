// C++ program to demonstrate the use of priority_queue
#include <iostream>
#include <queue>

// driver code
int main() {
    std::vector<int> v = {10, 2, 4, 8, 6, 9};

    // defining priority queue
    std::priority_queue<int> pq;

    // printing array
    std::cout << "Array: ";
    for (auto i : v) {
        std::cout << i << ' ';
    }

    std::cout << std::endl;

    // pushing array sequentially one by one
    for (int i = 0; i < 6; i++) {
        pq.push(v[i]);
    }

    // printing priority queue
    std::cout << "Priority Queue: ";
    while (!pq.empty()) {
        std::cout << pq.top() << ' ';
        pq.pop();
    }
    std::cout << std::endl;

    return 0;
}
