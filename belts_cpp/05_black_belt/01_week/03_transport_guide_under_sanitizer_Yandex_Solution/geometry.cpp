#include "geometry.h"

#include <cmath>



inline Point toRadian(Point p) {
  constexpr const double rad = PI / 180;
  return {p.latitude * rad, p.longitude * rad};
}

double computeDistance(Point a, Point b) {
  a = toRadian(a);
  b = toRadian(b);
  const double h = std::pow(std::sin( (b.latitude - a.latitude)/2 ), 2)
    + std::cos(a.latitude) * std::cos(b.latitude) * std::pow(sin( (b.longitude - a.longitude)/2 ),2);
  return 2 * EARTH_RADIUS * std::asin(std::sqrt(h));
}
