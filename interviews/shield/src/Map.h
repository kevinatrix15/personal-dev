#include "Grid.h"
#include "Point.h"

#include <algorithm>
#include <numeric>
#include <vector>

// TODO: get rid of namespaces...
using namespace MESH;

#if 0
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
#endif

enum class cell_state: uint8_t
{
    FREE,
    OBJECT,
    PADDED
};

class CellStateMap
{
    public:
    explicit CellStateMap(const Grid& grid) : m_grid(grid), m_states(grid.size(), cell_state::FREE)
    {
        // do nothing
    }

    cell_state operator()(const size_t xIdx, const size_t yIdx) const
    {
        return m_states[m_grid(xIdx, yIdx)];
    }

    cell_state& operator()(const size_t xIdx, const size_t yIdx)
    {
        return m_states[m_grid(xIdx, yIdx)];
    }

    // TODO: consider adding a method to print output to stream for writing / visualizing
    // friend std::ostream& operator<<(std::ostream &stream, vect3D const &p) { return stream << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::flush; };


    private:
    Grid m_grid;
    std::vector<cell_state> m_states;
};

#if 0
// TODO: consider replacing below class with lambda function / functional programming.
class BoundaryCellStateAssigner
{
    public:
    // TODO: consider returning the modified DistanceMap to follow functional programming /
    // enable piping (with move assignment operations)
    static void assign(const Grid& grid, CellStateMap& stateMapRef)
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
#endif

class ConfigurationSpace
{
    public:
    // NOTE: we assume the robot radius an integer value, which can be zero as a special case
    ConfigurationSpace(const size_t nx, const size_t ny, const size_t robotRadius) :
        m_grid(nx, ny), m_robotRadius(robotRadius), m_cellStates(m_grid)
    {
        // initialize distance map based on distance to outer boundaries??
        // BoundaryDistanceDetector::detect()
        assignBoundaryCellStates();
    }

    void addObstacles(const std::vector<Circle>& obstacles)
    {
        // TODO: inspect obstacles to ensure within grid
        // add obstacles
        // add padding around obstacles
    }

    private:
    Grid m_grid;
    // robot radius, in cells
    size_t m_robotRadius;
    CellStateMap m_cellStates;

    void assignBoundaryCellStates()
    {
        // TODO: add visitRowsAndCols() to Grid class, providing a range
        // TODO: consider moving the below into the Grid class for specialized iteration
        std::vector<size_t> allCols(m_grid.numX());
        std::iota(allCols.begin(), allCols.end(), 0);

        std::vector<size_t> allRows(m_grid.numY());
        std::iota(allRows.begin(), allRows.end(), 0);

        std::vector<size_t> bottomRows(m_robotRadius);
        std::iota(bottomRows.begin(), bottomRows.end(), 0);

        std::vector<size_t> topRows(m_robotRadius);
        std::iota(topRows.begin(), topRows.end(), m_grid.numY() - m_robotRadius);

        std::vector<size_t> leftCols(m_robotRadius);
        std::iota(leftCols.begin(), leftCols.end(), 0);

        std::vector<size_t> rightCols(m_robotRadius);
        std::iota(rightCols.begin(), rightCols.end(), m_grid.numX() - m_robotRadius);

        // bottom rows
        padRowsAndCols(bottomRows, allCols);

        // top rows
        padRowsAndCols(topRows, allCols);

        // left cols
        padRowsAndCols(allRows, leftCols);

        // right cols
        padRowsAndCols(allRows, rightCols);
    }

    // TODO: consider using c++ ranges for this
    void padRowsAndCols(const std::vector<size_t>& rows, const std::vector<size_t>& cols)
    {
        // TODO: replace with for_each and lambda?
        for (const size_t yIdx : rows) {
            for (const size_t xIdx : cols) {
                m_cellStates(xIdx, yIdx) = cell_state::PADDED;
            }
        }
    }

};