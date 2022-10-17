// CPP Program to implement Deque in STL
#include <deque>
#include <iostream>

using namespace std;

void show_dq(deque<int> dq) {
    for (auto it = dq.begin(); it != dq.end(); ++it) {
        if (next(it) != dq.end()) {
            cout << *it << " ";
        } else {
            cout << *it << endl;
        }
    }
}

int main() {
    deque<int> q;
    q.push_back(10);
    q.push_front(20);
    q.push_back(30);
    q.push_front(15);
    cout << "The deque q is : ";
    show_dq(q);

    cout << endl << "q.pop_back() : ";
    q.pop_back();
    show_dq(q);

    cout << endl << "q.pop_back() : ";
    q.pop_back();
    show_dq(q);


    return 0;
}
