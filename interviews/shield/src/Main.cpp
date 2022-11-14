#include "Grid.h"
#include "Map.h"
#include "MotionPlanning.h"

#include <iostream>
#include <vector>

int main()
{
  // TODO: make interactive assignment of grid size and robot radius
  const Grid grid(50, 25);
  // TODO: move grid here??
  ConfigurationSpace cSpace(grid, 3);

  // TODO: make interactive assignment of obstacles
  std::vector<Circle> obstacles;
  obstacles.emplace_back(Circle({14, 4}, 4));
  obstacles.emplace_back(Circle({37, 13}, 6));
  obstacles.emplace_back(Circle({0, 0}, 4));

  std::cout << cSpace;
  std::cout << std::endl;
  cSpace.addObstacles(obstacles);
  std::cout << cSpace;

  AStar search(cSpace);
  const std::vector<Point> path = searchPath({0,0}, {grid.numX()-1, grid.numY()-1});

  return 0;
}