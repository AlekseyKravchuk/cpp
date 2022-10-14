// queue::front
#include <iostream>  // std::cout
#include <queue>     // std::queue
#include <stack>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

int main() {
    std::stack<int> myStack1;
    std::stack<int> myStack2;

    myStack1.push(77);
    myStack1.push(16);
    myStack1.push(88);

    myStack2.push(myStack1.top());
    myStack1.pop();

    std::cout << "Finished." << endl;

    return 0;
}