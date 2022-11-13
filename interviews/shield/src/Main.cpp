#include "Grid.h"
#include "Map.h"

#include <iostream>
#include <vector>

int main()
{
  // TODO: make interactive assignment of grid size and robot radius
  const Grid grid(50, 25);
  // TODO: move grid here??
  ConfigurationSpace cSpace(grid, 1);

  // TODO: make interactive assignment of obstacles
  std::vector<Circle> obstacles;
  obstacles.emplace_back(Circle({14, 4}, 4));
  obstacles.emplace_back(Circle({37, 13}, 6));
  obstacles.emplace_back(Circle({0, 0}, 4));

  cSpace.addObstacles(obstacles);

  std::cout << cSpace;

  return 0;
}