#pragma once

#include "Vec3.h"

#include <ostream>
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

  /**
   * @brief Create a deep copy from another Mat33 object.
   * 
   * @param other The other Mat33 to copy.
   */
  Mat33(const Mat33& other)
  {
    for (size_t i = 0; i < NUM_COLS; ++i) {
      col[i] = {other.col[i].x, other.col[i].y, other.col[i].z};
    }
  }

  /**
   * @brief Copy assignment operator for deep copy of another Mat33.
   * 
   * @param other The other Mat33 to copy.
   * @return Mat33& The copied object.
   */
  Mat33& operator=(const Mat33& other)
  {
    for (size_t i = 0; i < NUM_COLS; ++i) {
      col[i] = {other.col[i].x, other.col[i].y, other.col[i].z};
    }
    return *this;
  }

  const Vec3* get() const
  {
    return col;
  }

  /**
   * @brief Transpose the matrix data in place. This looks
   * like the following:
   * 
   * [[v0.x, v1.x, v2.x]        [[v0.x, v0.y, v0.z]
   *  [v0.y, v1.y, v2.y]   =>    [v1.x, v1.y, v1.z]
   *  [v0.z, v1.z, v2.z]         [v2.x, v2.y, v2.z]
   */
  void transpose()
  {
    // To transpose the 3x3 matrix in-place, we simply swap positions
    // of the six off-diagonal elements
    swap(col[0].y, col[1].x);
    swap(col[0].z, col[2].x);
    swap(col[1].z, col[2].y);
  }

  static std::string indentString(const unsigned int level)
  {
    return std::string(level * INDENT_MULT, ' ');
  }

  /**
   * @brief Print the matrix data, optionally indented based on the level specified.
   * 
   * @param stream The stream to print to.
   * @param indentLevel The indentation level.
   */
  void print(std::ostream& stream = std::cout,
             const unsigned int indentLevel = 0) const
  {
    const std::string indent = indentString(indentLevel);
    // TODO: FUTURE WORK- use setw to keep all column widths aligned
    // https://cplusplus.com/forum/beginner/275937/
    stream << indent << col[0].x << ", " << col[1].x << ", " << col[2].x
           << std::endl;
    stream << indent << col[0].y << ", " << col[1].y << ", " << col[2].y
           << std::endl;
    stream << indent << col[0].z << ", " << col[1].z << ", " << col[2].z
           << std::endl;
  }

  private:
  Vec3 col[NUM_COLS];

  void swap(double& aRef, double& bRef) const
  {
    double tmp = aRef;
    aRef = bRef;
    bRef = tmp;
  }
};