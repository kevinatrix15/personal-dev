#include "Grid.h"
#include "Map.h"

#include <iostream>
#include <vector>

int main()
{
  const Grid grid(10, 20);
  // TODO: move grid here??
  ConfigurationSpace cSpace(grid, 2);

  std::vector<Circle> obstacles;
  obstacles.emplace_back(Circle({4, 4}, 3));
  obstacles.emplace_back(Circle({7, 13}, 5));
  cSpace.addObstacles(obstacles);

  std::cout << cSpace;

  return 0;
}