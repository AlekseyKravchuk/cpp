#include <iostream>
#include <memory>  // std::unique_ptr, std::make_unique
#include <utility> // std::move

using namespace std;

// этот класс будет говорить нам о своем состоянии
struct Actor {
    Actor() { cout << "I was born! :)" << endl; }
    ~Actor() {cout << "I died! :(" << endl; }
    void DoWork() {cout << "I did some job!" << endl;}
};

void run(Actor* ptr) {
    if (ptr) {
        ptr->DoWork();
    } else {
        cout << "An actor was expected :-/, but there is nullptr" << endl;
    }
}

int main() {
    std::unique_ptr<Actor> ptr = std::make_unique<Actor>();
    run(ptr.get());

    std::unique_ptr<Actor> ptr_copy = std::move(ptr);
    run(ptr_copy.get());

    // после перемещения исходный unique_ptr становится пустым после перемещения из него
    run(ptr.get());

    return 0;
}
