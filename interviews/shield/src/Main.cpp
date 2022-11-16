#include "FileIO.h"
#include "Map.h"
#include "MotionPlanning.h"

#include <exception>
#include <filesystem>
#include <iostream>
#include <vector>

constexpr int EXPECTED_CMDS = 5;

// TODO: add command line args for file path, etc.
int main(int argc, char** argv)
{
  // args:
  // 0- this command
  // 1- M (num rows => ny)
  // 2- N (num cols => nx)
  // 3- robot radius (in cells)
  // 4- Pre-configured case

  if (argc != EXPECTED_CMDS) {
    throw std::runtime_error("Invalid number of args provided: " + std::to_string(argc) + ", expected " + std::to_string(EXPECTED_CMDS));
  }
  const size_t ny = static_cast<size_t>(std::stoi(argv[1]));
  const size_t nx = static_cast<size_t>(std::stoi(argv[2]));
  const size_t robotRadius = static_cast<size_t>(std::stoi(argv[3]));

  enum class obstacle_config
  {
    NONE = 1,
    IMPOSSIBLE,
    SIMPLE,
    COMPLEX,
    MAZE
  };
  const obstacle_config obstacleCase = static_cast<obstacle_config>(std::stoi(argv[4]));

  // Pre-configured obstacle cases:
  // I- no obstacles
  // II- impossible path- single circle in center of domain = max(M, N)
  // III- simple path, following diagonal- circles at opposite corners from start and goal
  // IV- more complex path 
  // V- Maze- staggered barriers

  // TODO: make interactive assignment of grid size and robot radius
  ConfigurationSpace cSpace(nx, ny, robotRadius);

  // TODO: make interactive assignment of obstacles
  std::vector<Circle> obstacles;
  switch (obstacleCase) {
    case obstacle_config::NONE:
    {
      // nothing to do here
      break;
    }
    case obstacle_config::IMPOSSIBLE:
    {
      // single circle in center of domain with radius spanning the narrow dimension
      const size_t minRadius = std::min(nx, ny);
      const Point midPt = Point(nx/2, ny/2);
      obstacles.emplace_back(Circle(midPt, minRadius));
      break;
    }
    case obstacle_config::SIMPLE:
    {
      // two circles at opposite corners
      const size_t radius = static_cast<int>(std::min(nx, ny) / 2) - static_cast<int>(robotRadius);
      obstacles.emplace_back(Circle({0, ny-1}, radius));
      obstacles.emplace_back(Circle({nx-1, 0}, radius));
      break;
    }
    case obstacle_config::COMPLEX:
    {
      // two circles at opposite corners
      const size_t radius = static_cast<int>(std::min(nx, ny) / 8) - static_cast<int>(robotRadius);
      obstacles.emplace_back(Circle({0, ny/4}, radius));
      obstacles.emplace_back(Circle({0, ny/2}, radius));
      obstacles.emplace_back(Circle({0, 3*ny/4}, radius));

      obstacles.emplace_back(Circle({nx/4, 0}, radius));
      obstacles.emplace_back(Circle({nx/4, ny/3}, radius));
      obstacles.emplace_back(Circle({nx/4, 2*ny/3}, radius));
      obstacles.emplace_back(Circle({nx/4, ny-1}, radius));

      obstacles.emplace_back(Circle({nx/2, ny/4}, radius));
      obstacles.emplace_back(Circle({nx/2, ny/2}, radius));
      obstacles.emplace_back(Circle({nx/2, 3*ny/4}, radius));

      obstacles.emplace_back(Circle({3*nx/4, 0}, radius));
      obstacles.emplace_back(Circle({3*nx/4, ny/3}, radius));
      obstacles.emplace_back(Circle({3*nx/4, 2*ny/3}, radius));
      obstacles.emplace_back(Circle({3*nx/4, ny-1}, radius));

      obstacles.emplace_back(Circle({nx-1, ny/4}, radius));
      obstacles.emplace_back(Circle({nx-1, ny/2}, radius));
      obstacles.emplace_back(Circle({nx-1, 3*ny/4}, radius));

      break;
    }
    case obstacle_config::MAZE:
    {
      // two circles at opposite corners
      // TODO: fix the below- it runs into nx and is out of bounds
      const size_t radius = static_cast<int>(std::min(nx, ny) / 12) - static_cast<int>(robotRadius);
      obstacles.emplace_back(Circle({nx/5, 0}, radius));
      obstacles.emplace_back(Circle({nx/5, ny/6}, radius));
      obstacles.emplace_back(Circle({nx/5, ny/3}, radius));
      obstacles.emplace_back(Circle({nx/5, ny/2}, radius));
      obstacles.emplace_back(Circle({nx/5, 2*ny/3}, radius));
      obstacles.emplace_back(Circle({nx/5, 5*ny/6}, radius));

      obstacles.emplace_back(Circle({2*nx/5, ny/6}, radius));
      obstacles.emplace_back(Circle({2*nx/5, ny/3}, radius));
      obstacles.emplace_back(Circle({2*nx/5, ny/2}, radius));
      obstacles.emplace_back(Circle({2*nx/5, 2*ny/3}, radius));
      obstacles.emplace_back(Circle({2*nx/5, 5*ny/6}, radius));
      // obstacles.emplace_back(Circle({2*nx/5, ny-1}, radius));

      obstacles.emplace_back(Circle({3*nx/5, 0}, radius));
      obstacles.emplace_back(Circle({3*nx/5, ny/6}, radius));
      obstacles.emplace_back(Circle({3*nx/5, ny/3}, radius));
      obstacles.emplace_back(Circle({3*nx/5, ny/2}, radius));
      obstacles.emplace_back(Circle({3*nx/5, 2*ny/3}, radius));
      obstacles.emplace_back(Circle({3*nx/5, 5*ny/6}, radius));

      obstacles.emplace_back(Circle({4*nx/5, ny/6}, radius));
      obstacles.emplace_back(Circle({4*nx/5, ny/3}, radius));
      obstacles.emplace_back(Circle({4*nx/5, ny/4}, radius));
      obstacles.emplace_back(Circle({4*nx/5, 4*ny/3}, radius));
      obstacles.emplace_back(Circle({4*nx/5, 5*ny/6}, radius));
      // obstacles.emplace_back(Circle({4*nx/5, ny-1}, radius));

      break;
    }
    default:
      throw std::runtime_error("Invalid obstacleCase argument: " + std::to_string(static_cast<int>(obstacleCase)));
  }
  #if 0
  obstacles.emplace_back(Circle({14, 4}, 4));
  obstacles.emplace_back(Circle({37, 13}, 6));
  obstacles.emplace_back(Circle({40, 20}, 4));
  obstacles.emplace_back(Circle({10, 17}, 3));
  #endif

  std::cout << cSpace;
  std::cout << std::endl;
  cSpace.addObstacles(obstacles);
  std::cout << cSpace;


  // read map
  // ConfigurationSpace cSpace2 = ConfigSpaceIO::read(cSpaceFile);
  // std::cout << cSpace2;

  AStar search(cSpace);
  const std::vector<Point> path = search.searchPath({robotRadius + 1, robotRadius + 1}, {cSpace.numX()-robotRadius - 1, cSpace.numY()-robotRadius - 1});

  // write data
  const std::filesystem::path cSpaceFile("./output/config-space.txt");
  ConfigSpaceIO::write(cSpace, cSpaceFile);

  const std::filesystem::path pathFile("./output/solution-path.txt");
  SolutionPathIO::write(path, pathFile);

  return 0;
}