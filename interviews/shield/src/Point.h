#include <cmath>

class Point
{
    public:
    Point(const size_t x, const size_t y) : m_x(x), m_y(y)
    {
        // do nothing
    }

    size_t distance(const Point& other)
    {
        const int dx = static_cast<int>(m_x) - static_cast<int>(other.m_x);
        const int dy = static_cast<int>(m_y) - static_cast<int>(other.m_y);
        return static_cast<size_t>(sqrt(dx*dx + dy*dy));
    }

    private:
    size_t m_x;
    size_t m_y;
}

class Grid
{
    public:
    Grid(const size_t nx, const size_t ny) : m_nx(nx), m_ny(ny)
    {
        // the grid should have non-zero dimensions in both axes
        assert(m_nx > 0);
        assert(m_ny > 0);
    }

    // TODO: add convenience overloaded operators to map from 2D to 1D indexing
    // TODO: is this correct?
    size_t operator()()(const size_t xIdx, const size_t yIdx)
    {
        return xIdx + yIdx * m_nx;
    }

    private:
    size_t m_nx;
    size_t m_ny;

}