#include "Mat33.h"
#include "Node.h"
#include "Vec3.h"

#include <iostream>

int main()
{
  // Vector dot product
  {
    const Vec3 v1{1.0, 2.0, 3.0};
    const Vec3 v2{2.0, 4.0, 6.0};
    const auto r = v1.dot(v2);

    std::cout << "v1 dot v2: " << std::endl;
    std::cout << "v1: ";
    v1.print();
    std::cout << "v2: ";
    v2.print();
    std::cout << "res: " << r << std::endl;
  }

  // Matrix construction and transpose
  {
    const Vec3 c0{1.0, 2.0, 3.0};
    const Vec3 c1{4.0, 5.0, 6.0};
    const Vec3 c2{7.0, 8.0, 9.0};
    Mat33 m(c0, c1, c2);
    std::cout << std::endl;
    std::cout << "m: " << std::endl;
    m.print();

    // Matrix transpose (in-place)
    std::cout << std::endl;
    std::cout << "m': " << std::endl;
    m.transpose();
    m.print();
  }

  // Create a node with children nodes
  {
    Node c0({{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}, nullptr, 0);

    Node c1c0({{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}}, nullptr, 0);
    Node c1Children[] = {c1c0};
    Node c1({{1.0, 1.0, 1.0}, {2.0, 2.0, 2.0}, {3.0, 3.0, 3.0}}, c1Children, 1);

    Node rootChildren[] = {c0, c1};
    Node root({{0, 1, 2}, {0, 1, 2}, {0, 1, 2}}, rootChildren, 2);

    std::cout << std::endl;
    root.print();
  }

  return 0;
}