#pragma once

#include "Grid.h"
#include "Point.h"

#include <algorithm>
#include <ostream>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>

enum class cell_state: uint8_t
{
    FREE,
    OBJECT,
    PADDED
};

// TODO: can Concepts be used below???
// The below is necessary due to issue discussed here:
// https://stackoverflow.com/questions/61157866/error-cannot-bind-non-const-lvalue-reference-of-type-bool-to-an-rvalue-of-ty
template<typename T>
using U = std::conditional_t<std::is_same_v<T, bool>, char, T>;
class DataMap : public GridIndexer
{
    public:
    DataMap(const std::pair<size_t, size_t>& shape) : GridIndexer(shape), m_data(size())
    {
        // do nothing
    }

    DataMap(const std::pair<size_t, size_t>& shape, const U& initVal) :
        GridIndexer(shape), m_data(size(), initVal)
    {
        // do nothing
    }

    U at(const size_t xIdx, const size_t yIdx) const
    {
        return m_data[GridIndexer::idxFrom(xIdx, yIdx)];
    }

    U at(const Point& p) const
    {
        return m_data[GridIndexer::idxFrom(p)];
    }

    U& at(const size_t xIdx, const size_t yIdx)
    {
        return m_data[GridIndexer::idxFrom(xIdx, yIdx)];
    }

    U& at(const Point& p)
    {
        return m_data[GridIndexer::idxFrom(p)];
    }

    friend std::ostream& operator<<(std::ostream& os, const DataMap& dataMap)
    {
        for (size_t yIdx = 0; yIdx < dataMap.numY(); ++yIdx) {
            for (size_t xIdx = 0; xIdx < dataMap.numX(); ++xIdx) {
                // TODO: decide how to handle casting here as U may not have
                // a valid compatible conversion for <<
                os << static_cast<int>(dataMap.at(xIdx, yIdx)) << " ";
            }
            os << std::endl;
        }
        return os;
    };

    private:
    std::vector<U> m_data;
};

#if 0
// TODO: consider dissolving this into ConfigurationSpace class as it doesn't add much value...
class CellStateMap : public GridIndexer
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

    friend std::ostream& operator<<(std::ostream& os, const CellStateMap& states)
    {
        for (size_t yIdx = 0; yIdx < states.m_grid.numY(); ++yIdx) {
            for (size_t xIdx = 0; xIdx < states.m_grid.numX(); ++xIdx) {
                os << static_cast<int>(states(xIdx, yIdx)) << " ";
            }
            os << std::endl;
        }
        return os;
    };

    private:
    Grid m_grid;
    std::vector<cell_state> m_states;
};
#endif

class ConfigurationSpace : public GridIndexer
{
    public:
    // NOTE: we assume the robot radius an integer value, which can be zero as a special case
    // TODO: construct with a grid object
    ConfigurationSpace(const Grid& grid, const size_t robotRadius) :
        GridIndexer(grid.numX(), grid.numY()), m_grid(grid), m_robotRadius(robotRadius),
        m_cellStates(std::make_pair(grid.numX(), grid.numY()), cell_state::FREE)
    {
        // set the cell state to 'padded' to account for robot radius
        assignBoundaryCellStates();
    }

    void addObstacles(const std::vector<Circle>& obstacles)
    {
        // TODO: inspect obstacles to ensure within grid??
        // add obstacles
        // add padding around obstacles
        for (const auto& obstacle : obstacles) {
            // extend obstacles with robot radius
            const Circle padded(obstacle.center(), obstacle.radius() + m_robotRadius);
            GridCircle::visit(padded, m_grid,[this](const size_t xIdx, const size_t yIdx) {
                m_cellStates.at(xIdx, yIdx) = cell_state::OBJECT;
            }
            );
        }
    }

    bool isAccessible(const Point& p) const
    {
        return contains(p) && m_cellStates.at(p) == cell_state::FREE;
    }

    std::vector<Point> getAccessibleNbrs(const Point& p) const
    {
        std::vector<Point> nbrs;
        const size_t minX = p.x() > 0 ? p.x() - 1 : p.x();
        const size_t minY = p.y() > 0 ? p.y() - 1 : p.y();
        const size_t maxX = p.x() < m_grid.numX() - 1 ? p.x() + 1 : p.x();
        const size_t maxY = p.y() < m_grid.numY() - 1 ? p.y() + 1 : p.y();
        for (size_t yIdx = minY; yIdx <= maxY; ++yIdx) {
            for (size_t xIdx = minX; xIdx <= maxX; ++xIdx) {
                if (m_cellStates.at(xIdx, yIdx) == cell_state::FREE) {
                    nbrs.emplace_back(Point(xIdx, yIdx));
                }
            }
        }
        return nbrs;
    }

    friend std::ostream& operator<<(std::ostream& os, const ConfigurationSpace& space)
    {
        return os << space.m_cellStates;
    };

    private:
    Grid m_grid;
    // robot radius, in cells
    size_t m_robotRadius;
    DataMap<cell_state> m_cellStates;

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
                // TODO: fix this after refactoring () overload
                m_cellStates.at(xIdx, yIdx) = cell_state::PADDED;
            }
        }
    }

};