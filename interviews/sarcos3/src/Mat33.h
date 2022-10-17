#pragma once

#include "Vec3.h"

class Mat33
{
  public:
  Vec3 col[3];

  Mat33(const Vec3& v0, const Vec3& v1, const Vec3& v2) : col{v0, v1, v2}
  {
  }

  // [[v0.x, v1.x, v2.x]        [[v0.x, v0.y, v0.z]
  //  [v0.y, v1.y, v2.y]   =>    [v1.x, v1.y, v1.z]
  //  [v0.z, v1.z, v2.z]         [v2.x, v2.y, v2.z]
  // ]						    ]
  void transform()
  {
    // To transpose the 3x3 matrix in-place, we simply swap positions
    // of the six off-diagonal elements
    swap(col[0].y, col[1].x);
    swap(col[0].z, col[2].x);
    swap(col[1].z, col[2].y);
  }

  // TODO: add deep copy method

  void print()
  {
    std::cout << col[0].x << ", " << col[1].x << ", " << col[2].x << "\n";
    std::cout << col[0].y << ", " << col[1].y << ", " << col[2].y << "\n";
    std::cout << col[0].z << ", " << col[1].z << ", " << col[2].z << "\n";
  }

  private:
  void swap(double& aRef, double& bRef) const
  {
    double tmp = aRef;
    aRef = bRef;
    bRef = tmp;
  }
};