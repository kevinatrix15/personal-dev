#pragma once

#include "Cell.h"

#include <cassert>
#include <functional>
#include <memory>

class GridIndexer
{
    public:
    GridIndexer(const size_t nx, const size_t ny) : m_nx(nx), m_ny(ny)
    {
        // do nothing
    }

    GridIndexer(const std::pair<size_t, size_t>& shape) : GridIndexer(shape.first, shape.second)
    {
        // do nothing
    }

    GridIndexer(const GridIndexer& other) : m_nx(other.m_nx), m_ny(other.m_ny)
    {
      // do nothing
    }

    size_t idxFrom(const size_t xIdx, const size_t yIdx) const
    {
        assert(xIdx < m_nx);
        assert(yIdx < m_ny);
        return xIdx + yIdx * m_nx;
    }

    size_t idxFrom(const Cell& c) const
    {
        return idxFrom(c.x(), c.y());
    }

    size_t size() const
    {
        return m_nx * m_ny;
    }

    size_t numX() const
    {
        return m_nx;
    }

    size_t numY() const
    {
        return m_ny;
    }

    std::pair<size_t, size_t> shape() const
    {
        return std::make_pair(numX(), numY());
    }

    // NOTE: we are using Cell's here to store the x and y indices for convenience.
    bool contains(const Cell& c) const
    {
        return c.x() < numX() && c.y() < numY();
    }

    private:
    size_t m_nx;
    size_t m_ny;
};

// TODO: collapse this into GridIndexer and rename where used
class Grid : public GridIndexer
{
    public:
    Grid(const size_t nx, const size_t ny) : GridIndexer(nx, ny)
    {
        // the grid should have non-zero dimensions in both axes
        assert(numX() > 0);
        assert(numY() > 0);
    }
};

class Circle
{
    public:
    // NOTE: we allow a circle with a zero radius
    // TODO: consider storing data as int, allowing for negative circle center
    Circle(const Cell& center, const size_t radius) :
        m_center(center), m_radius(radius)
    {
    }

    Cell center() const
    {
        return m_center;
    }

    size_t radius() const
    {
        return m_radius;
    }

    private:
    Cell m_center;
    size_t m_radius;
};

using Callback = std::function<void(const size_t xIdx, const size_t yIdx)>;
class GridCircle
{
    public:

    static void visit(const Circle& circle, const GridIndexer& grid, Callback callback)
    {
        const size_t r = circle.radius();
        const Cell c = circle.center();

        const size_t xLimit = grid.numX() - 1;
        const size_t yLimit = grid.numY() - 1;

        auto insideLeft{ [&](const size_t xIdx) { return c.x() >= xIdx; } };
        auto insideRight{[&](const size_t xIdx) {return xIdx + c.x() <= xLimit;}};
        auto insideBottom{[&](const size_t yIdx) {return c.y() >= yIdx;}};
        auto insideTop{[&](const size_t yIdx) {return yIdx + c.y() <= yLimit;}};

        // TODO: consider replacing this with a simple brute force loop over bounding
        // box, checking each point is within the circle. This will avoid all of the
        // excessive checking for each point to be within the domain
        for (size_t yIdx = 0; yIdx <= r; ++yIdx) {
            const size_t xMaxSq = r * r - yIdx * yIdx;
            size_t xIdx = 0;
            while (xIdx * xIdx < xMaxSq) {
                // TODO: only visit if within domain
                // Quadrant I
                if (insideRight(xIdx) && insideTop(yIdx)) {
                    callback(c.x() + xIdx, c.y() + yIdx);
                }
                // Quadrant II
                if (insideLeft(xIdx) && insideTop(yIdx)) {
                    callback(c.x() - xIdx, c.y() + yIdx);
                }
                // Quadrant III
                if (insideLeft(xIdx) && insideBottom(yIdx)) {
                    callback(c.x() - xIdx, c.y() - yIdx);
                }
                // Quadrant IV
                if (insideRight(xIdx) && insideBottom(yIdx)) {
                    callback(c.x() + xIdx, c.y() - yIdx);
                }
                ++xIdx;
            }
        }
    }
};