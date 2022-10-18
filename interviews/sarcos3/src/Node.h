#pragma once

#include "Mat33.h"

#include <cassert>
#include <iostream>
#include <ostream>

class Node
{
  public:
  Node(const Mat33& data, Node* const children, const unsigned int numChildren)
      : data(data), children(children), numChildren(numChildren)
  {
    // Our children array pointer should only be null if numChildren is zero
    assert((numChildren > 0 && children) || (numChildren == 0 && !children));
  }

  /**
   * @brief Print the data associated with the node, indented based on the level
   * of the tree / graph the node is relative to the root node.
   *
   * @param stream The stream to print the node data to.
   * @param level The level of the current node relative to the root node.
   * @param childID The child ID associated with the parent node.
   */
  void print(std::ostream& stream = std::cout,
             unsigned int level = 0,
             const unsigned int childID = 0) const
  {
    stream << Mat33::indentString(level) << "Level: " << level
           << ", Child ID: " << childID << std::endl;

    // Print this node's matrix data
    data.print(stream, level);

    stream << std::endl;

    // Print each child's data
    level++;
    for (unsigned int i = 0; i < numChildren; ++i) {
      children[i].print(stream, level, i);
    }
  }

  private:
  Mat33 data;
  Node* children;
  unsigned int numChildren;
};