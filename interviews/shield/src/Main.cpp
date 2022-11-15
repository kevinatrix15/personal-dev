#include "FileIO.h"
#include "Grid.h"
#include "Map.h"
#include "MotionPlanning.h"

#include <filesystem>
#include <iostream>
#include <vector>

// TODO: add command line args for file path, etc.
int main()
{
  // TODO: make interactive assignment of grid size and robot radius
  const Grid grid(50, 25);
  // TODO: move grid here??
  ConfigurationSpace cSpace(50, 25, 1);

  // TODO: make interactive assignment of obstacles
  std::vector<Circle> obstacles;
  // obstacles.emplace_back(Circle({25, 12}, 12));
  obstacles.emplace_back(Circle({14, 4}, 4));
  obstacles.emplace_back(Circle({37, 13}, 6));
  obstacles.emplace_back(Circle({40, 20}, 4));
  obstacles.emplace_back(Circle({10, 17}, 3));

  std::cout << cSpace;
  std::cout << std::endl;
  cSpace.addObstacles(obstacles);
  std::cout << cSpace;

  // write map
  const std::filesystem::path cSpaceFile("./output/config-space.txt");
  SolutionWriter::write(cSpace, cSpaceFile);

  // read map
  ConfigurationSpace cSpace2 = SolutionWriter::read(cSpaceFile);
  std::cout << cSpace2;

  AStar search(cSpace);
  const std::vector<Point> path = search.searchPath({2,2}, {grid.numX()-2, grid.numY()-2});

  std::cout << "Path: " << std::endl;
  for (const auto& p : path) {
    std::cout << p << std::endl;
  }
  std::cout << std::endl;

  return 0;
}