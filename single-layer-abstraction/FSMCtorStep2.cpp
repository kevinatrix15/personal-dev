
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

  genWeights();

  m_totalTimer.stop();
}

/******************************************************************************
 * PRIVATE METHODS ************************************************************
 *****************************************************************************/

void FineSolidMap::genWeights()
{
  const auto [xFractions, yFractions, zFractions] = genXYZFractions();

  for (const auto& zFrac : zFractions) {
    for (const auto& yFrac : yFractions) {
      for (const auto& xFrac : xFractions) {
        const Weights weights = calcWeights(xFrac, yFrac, zFrac);

        assert(weightsSumToOne(weights));

        m_weights.emplace_back(std::move(weights));
      }
    }
  }
}

FineSolidMap::XYZFractions FineSolidMap::genXYZFractions()
{
  const std::vector<real_type> xFractions =
      getInnerNodeFractions(getXRefinedPtsPerCell());
  const std::vector<real_type> yFractions =
      getInnerNodeFractions(getYRefinedPtsPerCell());
  const std::vector<real_type> zFractions =
      getInnerNodeFractions(getZRefinedPtsPerCell());
  return XYZFractions{
      std::move(xFractions), std::move(yFractions), std::move(zFractions)};
}

std::vector<real_type> FineSolidMap::getInnerNodeFractions(
    const size_t nodesPerCell)
{
  std::vector<real_type> fractions(nodesPerCell);
  for (size_t i = 0; i < nodesPerCell; ++i) {
    fractions[i] =
        static_cast<real_type>(i) / static_cast<real_type>(nodesPerCell);
  }
  return fractions;
}

std::pair<real_type, real_type> calcOuterNeighborRatios(const real_type frac)
{
  std::pair<real_type, real_type> ratios;
  ratios.first = 1.0 - frac;
  ratios.second = frac;
  return ratios;
}

FineSolidMap::Weights FineSolidMap::calcWeights(const real_type xFrac,
                                                const real_type yFrac,
                                                const real_type zFrac)
{
  const auto [left, right] = calcOuterNeighborRatios(xFrac);
  const auto [back, front] = calcOuterNeighborRatios(yFrac);
  const auto [down, up] = calcOuterNeighborRatios(zFrac);

  return FineSolidMap::Weights{// bottom of cube
                               left * back * down,
                               right * back * down,
                               left * front * down,
                               right * front * down,
                               // top of cube
                               left * back * up,
                               right * back * up,
                               left * front * up,
                               right * front * up};
}

[[maybe_unused]] bool FineSolidMap::weightsSumToOne(
    const FineSolidMap::Weights& weights)
{
  return 1.0 == std::accumulate(weights.begin(), weights.end(), 0.0);
}

/* Key Takeaways:
 * - Became difficult to manage naming of many helper functions
 * - Indicates this class has too many responsibilities and should be
 *   refactored
 * - This should be relatively easy given how modular everything now is
 * - Get clear benefit with each level of refactoring
 * - Side benefit- found additional optimization opportunities
 * - Documentation is less needed as code describes itself (pending good naming)
