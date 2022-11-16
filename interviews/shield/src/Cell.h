#pragma once

#include <cassert>
#include <cmath>
#include <ostream>

class Cell
{
    public:
    Cell() : m_x(0U), m_y(0U)
    {
        // do nothing
    }

    // TODO (kbriggs): eventually, consider making this a templated class for flexibility. Keeping
    // size_t for current use case for simplicity.
    Cell(const size_t x, const size_t y) : m_x(x), m_y(y)
    {
        // do nothing
    }

    size_t x() const
    {
        return m_x;
    }

    size_t y() const
    {
        return m_y;
    }

    size_t& x()
    {
        return m_x;
    }

    size_t& y()
    {
        return m_y;
    }

    // TODO: consider making this distSq to avoid taking sqrt
    double distance(const Cell& other) const
    {
        const double dx = static_cast<double>(m_x) - static_cast<double>(other.m_x);
        const double dy = static_cast<double>(m_y) - static_cast<double>(other.m_y);
        return sqrt(dx*dx + dy*dy);
    }

    bool operator==(const Cell& other) const
    {
        return m_x == other.m_x && m_y == other.m_y;
    }

    bool operator!=(const Cell& other) const
    {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Cell& p)
    {
        os << "(" << p.x() << ", " << p.y() << ")";
        return os;
    };

    private:
    size_t m_x;
    size_t m_y;
};