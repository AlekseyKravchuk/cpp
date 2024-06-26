#pragma once

#include <string>
#include <string_view>

struct Coordinates {
public:
  long double latitude;
  long double longitude;

  static long double Distance(const Coordinates& lhs, const Coordinates& rhs);

private:
  static const long double PI;
  static const long double ONE_DEG;
  static const long double EARTH_RADIUS;
};

struct Stop {
public:
  explicit Stop(std::string name, Coordinates coordinates = {});
  Stop() = default;
  std::string Name() const { return _name; }
  Coordinates StopCoordinates() const { return _coordinates; }
  void SetCoordinates(Coordinates coordinates) { _coordinates = coordinates; }

private:
  std::string _name;
  Coordinates _coordinates{};
};

