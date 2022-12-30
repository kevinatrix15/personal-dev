/******************************************************************************
 * CONSTRUCTORS / DESTRUCTOR **************************************************
 *****************************************************************************/

FineSolidMap::FineSolidMap(const UniformGrid& grid,
                           const std::vector<real_type>& zCoords,
                           const size_t layerOffset,
                           const size_t xyRefinement,
                           const size_t zRefinement,
                           const real_type solidusTemperature)
    : m_layerOffset(static_cast<dim_type>(layerOffset)),
      m_xyRefinement(xyRefinement),
      m_zRefinement(zRefinement),
      m_weights(),
      m_fineGrid((grid.getNumPtsX() - 1) * xyRefinement + 1,
                 (grid.getNumPtsY() - 1) * xyRefinement + 1,
                 grid.getMaxX() - grid.getMinX(),
                 grid.getMaxY() - grid.getMinY(),
                 grid.getMeshPadding()),
      m_fineZCoords((zCoords.size() - layerOffset - 1) * zRefinement + 1),
      m_solidusTemperature(solidusTemperature),
      m_grid(grid),
      m_zCoords(zCoords),
      m_solid(m_fineGrid.getNumPts() * m_fineZCoords.size(), NONSOLID_VALUE),
      m_totalTimer(),
      m_shiftTimer(),
      m_updateTimer(),
      m_fillTimer()
{
  m_totalTimer.start();
  assert(!m_zCoords.empty());
  ASSERT_LESS(m_layerOffset, m_zCoords.size());
  ASSERT_GREATER(m_zRefinement, 0UL);
  
  // build a fine zCoords array
  m_fineZCoords = buildFineZValues(m_zCoords, m_layerOffset, m_zRefinement);

  const size_t xPtsPerCell = getXRefinedPtsPerCell();
  const size_t yPtsPerCell = getYRefinedPtsPerCell();
  const size_t zPtsPerCell = getZRefinedPtsPerCell();
  for (size_t z = 0; z < zPtsPerCell; ++z) {
    const auto zFrac =
        static_cast<real_type>(z) / static_cast<real_type>(m_zRefinement);
    for (size_t y = 0; y < yPtsPerCell; ++y) {
      const auto yFrac =
          static_cast<real_type>(y) / static_cast<real_type>(m_xyRefinement);
      for (size_t x = 0; x < xPtsPerCell; ++x) {
        const auto xFrac =
            static_cast<real_type>(x) / static_cast<real_type>(m_xyRefinement);
        const real_type down = 1.0 - zFrac;
        const real_type up = zFrac;
        const real_type back = 1.0 - yFrac;
        const real_type front = yFrac;
        const real_type left = 1.0 - xFrac;
        const real_type right = xFrac;

        std::array<real_type, 8> weights{// bottom of cube
                                         left * back * down,
                                         right * back * down,
                                         left * front * down,
                                         right * front * down,
                                         // top of cube
                                         left * back * up,
                                         right * back * up,
                                         left * front * up,
                                         right * front * up};

        ASSERT_EQUAL(1.0,
                     weights[0] + weights[1] + weights[2] + weights[3] +
                         weights[4] + weights[5] + weights[6] + weights[7]);

        m_weights.emplace_back(std::move(weights));

        ASSERT_EQUAL(m_weights.size() - 1, calculateWeightIndex(x, y, z));
      }
    }
  }

  m_totalTimer.stop();
}

