#include <iostream>
#include <string>

using namespace std;

class Tree {
public:
    Tree() {
        cout << "+tree"s;
    }
    ~Tree() {
        cout << "-tree"s;
    }
};

class House {
public:
    House() {
        cout << "+house"s;
    }
    ~House() {
        cout << "-house"s;
    }
};

House BuildHouse(Tree tree) {
    return House{};
}

int main() {
    House house = BuildHouse({});
    cout << "~life"s;
}