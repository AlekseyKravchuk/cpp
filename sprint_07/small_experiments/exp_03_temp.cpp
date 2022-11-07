#include <iostream>

using namespace std;

struct Log {
    explicit Log(int id)
        : id_(id) {
        cout << id_;
    }

    ~Log() {
        cout << "-" << id_;
    }

   private:
    int id_;
};

void One() {
    Log local1{1};
}

void Two() {
    Log local2{2};
    One();
    Log local3{3};
}

Log global1{4};

int main() {
    Two();
}

Log global2{5};