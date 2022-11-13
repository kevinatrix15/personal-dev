#pragma once

#include "Point.h"

#include <cassert>
#include <functional>
#include <memory>

class Grid
{
    public:
    Grid(const size_t nx, const size_t ny) : m_nx(nx), m_ny(ny)
    {
        // the grid should have non-zero dimensions in both axes
        assert(m_nx > 0);
        assert(m_ny > 0);
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

    // TODO: add convenience overloaded operators to map from 2D to 1D indexing
    // TODO: test this for correctness
    size_t operator()(const size_t xIdx, const size_t yIdx) const
    {
        assert(xIdx < m_nx);
        assert(yIdx < m_ny);
        return xIdx + yIdx * m_nx;
    }

    private:
    size_t m_nx;
    size_t m_ny;
};

class Circle
{
    public:
    // NOTE: we allow a circle with a zero radius
    // TODO: consider storing data as int, allowing for negative circle center
    Circle(const Point& center, const size_t radius) :
        m_center(center), m_radius(radius)
    {
    }

    Point center() const
    {
        return m_center;
    }

    size_t radius() const
    {
        return m_radius;
    }

    private:
    Point m_center;
    size_t m_radius;
};

using Callback = std::function<void(const size_t xIdx, const size_t yIdx)>;
class GridCircle
{
    public:
    #if 0
    static void visitRing(const Point& center, const size_t r0, const size_t r1, const Grid& grid)
    {
        const size_t startY = center.y() + r0;
        const size_t endY = center.y() + r1;
        for (size_t yIdx = startY; yIdx <= endY; ++yIdx) {
            const size_t startX = 
        }
    }
    #endif

    static void visit(const Circle& circle, const Grid& grid, Callback callback)
    {
        const size_t r = circle.radius();
        const Point c = circle.center();

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