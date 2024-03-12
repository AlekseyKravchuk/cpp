#include <queue>
#include <iostream>


using namespace std;

int main() {
    queue<int*> q;

    int* p1 = new int(1);
    int* p2 = new int(2);
    int* p3 = new int(3);
    int* p4 = new int(4);

    q.push(p1);
    q.push(p2);
    q.push(p3);
    q.push(p4);

    auto p = q.front();

    cout << "*q.front() = " << *p << endl;

    return 0;
}
