#pragma once

#include <cstdint>

namespace geo2d {

struct Point {
  int x, y;
};

uint64_t DistanceSquared(Point p1, Point p2);

struct Vector {
  int x, y;

  Vector(int xx, int yy) : x(xx), y(yy) {
  }

  Vector(Point from, Point to) : x(to.x - from.x), y(to.y - from.y) {
  }
};

int64_t operator * (Vector lhs, Vector rhs);
int64_t ScalarProduct(Vector lhs, Vector rhs);

struct Segment {
  Point p1, p2;
};

class Rectangle {
private:
  int x_left, x_right;
  int y_bottom, y_top;

public:
  Rectangle(Point p1, Point p2);

  int Left() const { return x_left; }
  int Right() const { return x_right; }
  int Top() const { return y_top; }
  int Bottom() const { return y_bottom; }

  Point BottomLeft() const { return {x_left, y_bottom}; }
  Point BottomRight() const { return {x_right, y_bottom}; }
  Point TopRight() const { return {x_right, y_top}; }
  Point TopLeft() const { return {x_left, y_top}; }
};

struct Circle {
  Point center;
  uint32_t radius;
};

/*1*/  bool Collide(Point p, Point q);
/*2*/  bool Collide(Point p, Segment s);
/*3*/  bool Collide(Point p, Rectangle r);
/*4*/  bool Collide(Point p, Circle c);
/*5*/  bool Collide(Rectangle r, Point p);
/*6*/  bool Collide(Rectangle r, Segment s);
/*7*/  bool Collide(Rectangle r1, Rectangle r2);
/*8*/  bool Collide(Rectangle r, Circle c);
/*9*/  bool Collide(Segment s, Point p);
/*10*/ bool Collide(Segment s1, Segment s2);
/*11*/ bool Collide(Segment s, Rectangle r);
/*12*/ bool Collide(Segment s, Circle c);
/*13*/ bool Collide(Circle c, Point p);
/*14*/ bool Collide(Circle c, Rectangle r);
/*15*/ bool Collide(Circle c, Segment s);
/*16*/ bool Collide(Circle c1, Circle c2);

}
