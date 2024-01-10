#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

struct Shape {
    virtual Shape* Clone() = 0;
    virtual ~Shape() = default;
};

// Curiously Recurring Template Pattern
// Вместо того, чтобы переопределять (override) виртуальную функцию "Clone()" в каждом производном классе, используем CRTP
template <typename T>
struct ShapeCRTP : public Shape {
    virtual Shape* Clone() override {
        return new T(*static_cast<T*>(this));
    }
};

struct Square : public ShapeCRTP<Square> {
    int x = 1;
};

struct Rectangle : public ShapeCRTP<Rectangle> {
    int x = 1;
    int y = 2;
};

struct Triangle : public ShapeCRTP<Triangle> {
    int x = 1;
    int y = 2;
    int z = 3;
};

int main() {
    vector<Shape*> v;
    v.push_back(new Square);
    v.push_back(new Rectangle);
    v.push_back(new Triangle);

    for (const auto& shape_ptr : v) {
        Shape* sh = shape_ptr->Clone();
        int i = 0;
    }

    return 0;
}
