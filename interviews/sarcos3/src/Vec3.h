/**
 * @file Vec3.h
 * @brief Definition of the three-component vector class.
 * @author Kevin Briggs <kevinabriggs@hotmail.com>
 * @version 1
 * @date 2022-10-18
 */
#pragma once

#include <iostream>

/******************************************************************************
* Vec3 Class ******************************************************************
******************************************************************************/

class Vec3
{
  public:
  double x, y, z;

  Vec3(const double x, const double y, const double z) : x(x), y(y), z(z)
  {
    // do nothing
  }

  Vec3() : Vec3(0.0, 0.0, 0.0)
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

  bool operator==(const Vec3& other) const
  {
    return x == other.x && y == other.y && z == other.z;
  }

  private:
};
