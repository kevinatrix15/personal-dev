#pragma once

#include <iostream>

class Vec3
{
  public:
  double x, y, z;

  Vec3(const double x, const double y, const double z) : x(x), y(y), z(z)
  {
    // do nothing
  }

  double dot(const Vec3& other) const
  {
    return (x * other.x) + (y * other.y) + (z * other.z);
  }

  void print() const
  {
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
  }

  bool operator ==(const Vec3& other) const
  {
    return x == other.x && y == other.y && z == other.z;
  }

  private:
};