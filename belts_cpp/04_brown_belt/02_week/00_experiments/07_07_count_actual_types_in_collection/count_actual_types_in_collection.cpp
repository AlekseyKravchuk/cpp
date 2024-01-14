#include <algorithm>
#include <cstdlib>  // std::rand
#include <iostream>
#include <memory>  // std::shared_ptr
#include <random>

struct Point {
    int x, y;
};

class Shape {
   public:
    virtual void Draw() = 0;
};

class Rectangle : public Shape {
   public:
    Rectangle(Point p1, Point p2);
    virtual void Draw() override;

   private:
    int _x_left;
    int _x_right;
    int _y_bottom;
    int _y_top;
};

struct Circle : public Shape {
    Circle(Point p, int radius);
    Point _center;
    int _radius;
    virtual void Draw() override;
};

Rectangle::Rectangle(Point p1, Point p2)
    : _x_left(std::min(p1.x, p2.x)),
      _x_right(std::max(p1.x, p2.x)),
      _y_bottom(std::min(p1.y, p2.y)),
      _y_top(std::max(p1.y, p2.y)) {}

void Rectangle::Draw() {
    std::cout << "Rectangle::Draw()" << std::endl;
}

Circle::Circle(Point p, int radius)
    : _center(p), _radius(radius) {}

void Circle::Draw() {
    std::cout << "Circle::Draw()" << std::endl;
}

std::shared_ptr<Shape> Make() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist_100(1,100); // distribution in range [1, 6]

    switch (dist_100(rng) & 0x1) {
        case 0:
            std::cout << "Rect ";
            return std::make_shared<Rectangle>(Point{2, 2}, Point{6, 4});
            break;
        case 1:
            std::cout << "Circle ";
            return std::make_shared<Circle>(Point{0, 0}, 1);
            break;
    }
}

int main() {
    size_t MAX_SHAPES = 10;
    std::vector<std::shared_ptr<Shape>> shapes;

    for (size_t i = 0; i < MAX_SHAPES; ++i) {
        shapes.push_back(Make());
    }
    std::cout << std::endl;

    std::cout << "the number of Circles: "
              << std::count_if(shapes.begin(), shapes.end(),
                               [](std::shared_ptr<Shape>& p) {
                                //    return typeid(*p) == typeid(Circle);
                                // использование "dynamic_cast" здесь возможно только потому, что Circle/Rectangle
                                // являются листьями в дереве иерархий, т.е. от них никто не наследуется
                                return dynamic_cast<Circle*>(p.get());
                               })
              << " items." << std::endl;

    return 0;
}
