#include <iostream>
#include <memory>
#include <vector>

using namespace std;

// абстрактный базовый класс
struct Shape {
    virtual Shape* Clone() = 0;
    virtual ~Shape() = default;
};

struct Square : public Shape {
    virtual Square* Clone() override {
        cout << "Square::Clone() function is called." << endl;
        return new Square(*this);
    }

    int x = 1;
};

struct Rectangle : public Shape {
    virtual Rectangle* Clone() override {
        cout << "Rectangle::Clone() function is called." << endl;
        return new Rectangle(*this);
    }

    int x = 1;
    int y = 2;
};

int main() {
    vector<shared_ptr<Shape>> v;
    v.push_back(make_shared<Square>());
    v.push_back(make_shared<Rectangle>());

    for (const auto& smart_ptr : v) {
        smart_ptr->Clone();
    }

    return 0;
}
