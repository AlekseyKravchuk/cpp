#pragma once

static constexpr double PI = 3.1415926535;

static constexpr short EARTH_RADIUS = 6371;


struct Point {
  double latitude;
  double longitude;
};


Point toRadian(Point p);

double computeDistance(Point a, Point b);
