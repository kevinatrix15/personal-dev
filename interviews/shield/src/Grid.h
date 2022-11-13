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

        const size_t startY = c.y();
        const size_t endY = c.y() + r;
        for (size_t yIdx = startY; yIdx <= endY; ++yIdx) {
            const size_t xMaxSq = r * r - yIdx * yIdx;
            const size_t startX = c.x();
            size_t xIdx = 0;
            while (xIdx * xIdx < xMaxSq) {
                // TODO: only visit if within domain
                // Quadrant I
                if (xIdx <= xLimit && yIdx <= yLimit) {
                    callback(xIdx, yIdx);
                }
                // Quadrant II
                if (c.x() >= xIdx && yIdx <= yLimit) {
                    callback(c.x() - xIdx, yIdx);
                }
                // Quadrant III
                if (xIdx <= xLimit && c.y() >= yIdx) {
                    callback(xIdx, c.y() - yIdx);
                }
                // Quadrant IV
                if (c.x() >= xIdx && c.y() >= yIdx) {
                    callback(c.x() - xIdx, c.y() - yIdx);
                }
            }
        }
    }
};