#pragma once

#include "Vec3.h"

#include <string>

// TODO: add section headers
namespace
{
constexpr unsigned int INDENT_MULT = 2;
constexpr unsigned int NUM_COLS = 3;
} // namespace

class Mat33
{
  public:
  Mat33(const Vec3& v0, const Vec3& v1, const Vec3& v2) : col{v0, v1, v2}
  {
    // do nothing
  }

#if 0
  // TODO: rule of 5
  // Deep copy constructor
  Mat33(const Mat33& other)
  {
    col = new Vec3*[NUM_COLS];
    for (size_t i = 0; i < NUM_COLS; ++i) {
      col[i] = {other[i].x, other[i].y, other[i].z};
    }
  }

  Mat33& operator=(const Mat33& other)
  {
    col = new Vec3[NUM_COLS];
    for (size_t i = 0; i < NUM_COLS; ++i) {
      col[i] = {other[i].x, other[i].y, other[i].z};
    }
    return *this;
  }
#endif

  const Vec3* get() const
  {
    return col;
  }

  // [[v0.x, v1.x, v2.x]        [[v0.x, v0.y, v0.z]
  //  [v0.y, v1.y, v2.y]   =>    [v1.x, v1.y, v1.z]
  //  [v0.z, v1.z, v2.z]         [v2.x, v2.y, v2.z]
  // ]						    ]
  void transpose()
  {
    // To transpose the 3x3 matrix in-place, we simply swap positions
    // of the six off-diagonal elements
    swap(col[0].y, col[1].x);
    swap(col[0].z, col[2].x);
    swap(col[1].z, col[2].y);
  }

  // TODO: add deep copy method

  static std::string indentString(const unsigned int level)
  {
    return std::string(level * INDENT_MULT, ' ');
  }

  void print(const unsigned int indentLevel = 0) const
  {
    const std::string indent = indentString(indentLevel);
    // TODO: look into setw to keep all column widths aligned
    // https://cplusplus.com/forum/beginner/275937/
    std::cout << indent << col[0].x << ", " << col[1].x << ", " << col[2].x
              << std::endl;
    std::cout << indent << col[0].y << ", " << col[1].y << ", " << col[2].y
              << std::endl;
    std::cout << indent << col[0].z << ", " << col[1].z << ", " << col[2].z
              << std::endl;
  }

  private:
  // NOTE: this is a pointer to the memory address of col[0], thus requiring
  // a deep copy method
  Vec3 col[NUM_COLS];

  void swap(double& aRef, double& bRef) const
  {
    double tmp = aRef;
    aRef = bRef;
    bRef = tmp;
  }
};