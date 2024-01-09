#include <iostream>
#include <vector>
#include <memory>  // std::shared_ptr

using namespace std;

// What is runtime Polymorphism
class Base {
  public:
    virtual void f() {
        cout << "Base::f called." << endl;
    }
};

class Derived : public Base {
  public:
    virtual void f() override {
        cout << "Derived::f called." << endl;
    }
};

void CallFunctionByPtrToBaseClass(Base* bp) {
    bp->f();
}

int main() {
    vector<shared_ptr<Base>> v;
    v.push_back(make_shared<Base>());
    v.push_back(make_shared<Derived>());

    for (const auto& sh_ptr : v) {
        CallFunctionByPtrToBaseClass(sh_ptr.get());
    }
    
    return 0;
}
