#include "Grid.h"
#include "Point.h"

#include <vector>

// TODO: get rid of namespaces...
using namespace MESH;

class DistanceMap
{
    public:
    DistanceMap(const Grid& grid) : m_grid(grid), m_distances(grid.size())
    {
        // do nothing
    }

    size_t operator()(const size_t xIdx, const size_t yIdx) const
    {
        return m_distances[m_grid(xIdx, yIdx)];
    }

    size_t& operator()(const size_t xIdx, const size_t yIdx)
    {
        return m_distances[m_grid(xIdx, yIdx)];
    }

    // TODO: consider adding a method to print output to stream for writing / visualizing
    // friend std::ostream& operator<<(std::ostream &stream, vect3D const &p) { return stream << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::flush; };


    private:
    Grid m_grid;
    std::vector<size_t> m_distances;
};

// TODO: consider replacing below class with lambda function / functional programming.
class BoundaryDistanceDetector
{
    public:
    // TODO: consider returning the modified DistanceMap to follow functional programming /
    // enable piping
    static void detect(const Grid& grid, DistanceMap& distMapRef)
    {
        // TODO: define a lambda to find minimum distance to nearest edge
        constexpr size_t minBoundaryDist = [] (const size_t xIdx, const size_t yIdx) {
            const size_t minX = min(xIdx + 1, grid.numX() - xIdx);
            const size_t minY = min(yIdx + 1, grid.numY() - yIdx);
            return min(minX, minY);
        }

        // TODO: consider a way to iterate over DistanceMap's data without a grid
        // and explicit for loops...
        for (size_t yIdx = 0; yIdx < grid.numY(); ++yIdx) {
            for (size_t xIdx = 0; xIdx < grid.numX(); ++xIdx) {
                distMapRef(xIdx, yIdx) = minBoundaryDist(xIdx, yIdx);
            }
        }
    }
};

class ObstacleDistanceDetector
{
    public:
    // TODO: consider returning the modified DistanceMap to follow functional programming /
    // enable piping
    // TODO: make an Obstacles class to make type polymorphic
    //  + getOuterCells()- method to get outer-most grid cells
    static void detect(const Grid& grid, const std::vector<Circle>& obstacles, DistanceMap& distMapRef)
    {
        for (const Circle& obs : obstacles) {
            // create a queue
            // get outer-cells in current obstacle
            // add outer-cells
            // perform BFS until no longer overwrite DistanceMap values with lower values
        }
    }

    private:
};

class Map
{
    public:
    // NOTE: we assume the robot radius an integer value, which can be zero as a special case
    Map(const size_t nx, const size_t ny, const size_t robotRadius) : m_grid(nx, ny), m_robotRadius(robotRadius)
    {
        // initialize distance map based on distance to outer boundaries??
        // BoundaryDistanceDetector::detect()
    }

    void addObstacles(const std::vector<Circle>& obstacles)
    {
        // update obstacle distance map
        // ObstacleDistanceDetector::detect(obstacles)
    }

    private:
    Grid m_grid;

    // TDOO: consider whether we need a Robot class
    const size_t m_robotRadius;
};