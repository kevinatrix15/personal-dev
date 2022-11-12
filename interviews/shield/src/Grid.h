#include <cassert>
#include <memory>

namespace MESH
{
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
} // namespace MESH