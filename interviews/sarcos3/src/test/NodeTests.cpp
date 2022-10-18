#include "Node.h"
#include "catch2.h"

#include <sstream>

/**
 * @brief Attempt to create the following tree of Nodes and print it out:
 *
 *                      root
 *                      / \
 *                     /   \
 *                    /     \
 *                   c0      c1
 *                           /
 *                          /
 *                         /
 *                       c1c0
 *
 */
TEST_CASE("Node's print is executed successfully", "[print]")
{
  // arrange
  Node c0({{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}, nullptr, 0);

  Node c1c0({{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}}, nullptr, 0);
  Node c1Children[] = {c1c0};
  Node c1({{1.0, 1.0, 1.0}, {2.0, 2.0, 2.0}, {3.0, 3.0, 3.0}}, c1Children, 1);

  Node rootChildren[] = {c0, c1};
  Node root({{0, 1, 2}, {0, 1, 2}, {0, 1, 2}}, rootChildren, 2);

  // act & assert
  std::stringstream ss;
  root.print(ss);

  // TODO: FUTURE WORK- Parse ss line by line, comparing to inputs.
  // For now, consider visual inspection as passing
}