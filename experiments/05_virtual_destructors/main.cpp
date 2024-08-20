#include <iostream>
#include <vector>
#include <memory>

using namespace std;

class Base {
  public:
    Base() : _storage1(new int[15]) {
        cout << "Constructor for class Base" << endl;
    }

    // !!! По умолчанию в базовом классе деструктор нужно делать ВИРТУАЛЬНЫМ
    virtual ~Base() {
        delete [] _storage1;
        cout << "DESTRUCTOR for class Base" << endl;
    }

  private:
    int* _storage1;
};


class Derived : public Base {
  public:
    Derived() : _storage2(new int[30]) {
        cout << "Constructor for class Derived" << endl;
    }

    ~Derived() override {
        delete [] _storage2;
        cout << "DESTRUCTOR for class Derived" << endl;
    }

  private:
    int* _storage2;
};


int main() {
    unique_ptr<Base> b1 = make_unique<Base>();
    unique_ptr<Derived> d1 = make_unique<Derived>();

    vector<unique_ptr<Base>> v;
    v.push_back(std::move(b1));
    v.push_back(std::move(d1));

    return 0;
}