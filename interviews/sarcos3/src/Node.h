#pragma once

#include "Mat33.h"

#include <cassert>
#include <iostream>

class Node
{
  public:
  Node(const Mat33& data, Node* const children, const unsigned int numChildren)
      : data(data), children(children), numChildren(numChildren)
  {
    // Our children array pointer should only be null if numChildren is zero
    assert((numChildren > 0 && children) || (numChildren == 0 && !children));
  }

  void print(unsigned int level = 0, const unsigned int childID = 0) const
  {
    std::cout << Mat33::indentString(level) << "Level: " << level
              << ", Child ID: " << childID << std::endl;

    // print this node's data
    data.print(level);

    std::cout << std::endl;

    // print each child's data
    // TODO: give a 'level' argument to this print() to indicate
    // number of tabs to indent??
    level++;
    for (unsigned int i = 0; i < numChildren; ++i) {
      children[i].print(level, i);
    }
  }

  private:
  Mat33 data;
  Node* children;
  unsigned int numChildren;
};