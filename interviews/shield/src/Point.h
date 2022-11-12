#include <cassert>
#include <cmath>

namespace MESH
{
class Point
{
    public:
    Point() : m_x(0U), m_y(0U)
    {
        // do nothing
    }

    // TODO (kbriggs): eventually, consider making this a templated class for flexibility. Keeping
    // size_t for current use case for simplicity.
    Point(const size_t x, const size_t y) : m_x(x), m_y(y)
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

    size_t distance(const Point& other) const
    {
        const int dx = static_cast<int>(m_x) - static_cast<int>(other.m_x);
        const int dy = static_cast<int>(m_y) - static_cast<int>(other.m_y);
        return static_cast<size_t>(sqrt(dx*dx + dy*dy));
    }

    private:
    size_t m_x;
    size_t m_y;
};

class Circle
{
    public:
    // NOTE: we allow a circle with a zero radius
    Circle(const Point& center, const size_t radius) : m_center(center), m_radius(radius)
    {
    }

    // TODO: need a method to get all cells within a circle (or even better- outer-most cells)

    private:
    Point m_center;
    size_t m_radius;
};
} // namespace MESH