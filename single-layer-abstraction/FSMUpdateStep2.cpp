/******************************************************************************
 * PUBLIC METHODS *************************************************************
 *****************************************************************************/

void FineSolidMap::update(const SparseColumnMap& temps,
                          const IMeltPoolPoints& volume)
{
  m_totalTimer.start();
  m_updateTimer.start();

  const FineSolidMap::TempCubesSet tempGrid = collectMeltedCubes(temps, volume);

  setSolidifiedNodes(temps);

  m_updateTimer.stop();
  m_totalTimer.stop();
}

/******************************************************************************
* PRIVATE METHODS *************************************************************
******************************************************************************/

FineSolidMap::TempCubesSet FineSolidMap::collectMeltedCubes(
    const SparseColumnMap& temps, const IMeltPoolPoints& volume)
{
  // go through each column of temperatures, and insert all adjacent cubes
  // into tempGrid for any points in the meltpool and above solidus
  FineSolidMap::TempCubesSet tempGrid;
  for (index_type sparseIdx = 0; sparseIdx < temps.size(); ++sparseIdx) {
    const dim_type nodeY = temps.getYBySparse(sparseIdx);
    const dim_type nodeX = temps.getXBySparse(sparseIdx);
    const dim_type startY = nodeY > 0 ? nodeY - 1 : 0;
    const dim_type endY = nodeY + 1 < temps.getNumPtsY() ? nodeY + 1 : nodeY;
    const dim_type startX = nodeX > 0 ? nodeX - 1 : 0;
    const dim_type endX = nodeX + 1 < temps.getNumPtsX() ? nodeX + 1 : nodeX;

    for (dim_type globalLayer = m_layerOffset;
         globalLayer < temps.getNumLayers();
         ++globalLayer) {
      const real_type temp = temps.getValueBySparse(sparseIdx, globalLayer);
      // the nodes actual position
      const dim_type localLayer = getLocalLayer(globalLayer);

      const size_t dense3d =
          (globalLayer * temps.getNumPtsY() + nodeY) * temps.getNumPtsX() +
          nodeX;

      if (temp > m_solidusTemperature && volume.isIn(dense3d)) {
        // the range of cubes to search
        const dim_type startZ = localLayer > 0 ? localLayer - 1 : 0;
        const dim_type endZ =
            localLayer + 1 < temps.getNumLayers() - m_layerOffset
                ? localLayer + 1
                : localLayer;

        // go through each adjacent cube and add it to the list
        for (dim_type locZ = startZ; locZ < endZ; ++locZ) {
          for (dim_type y = startY; y < endY; ++y) {
            for (dim_type x = startX; x < endX; ++x) {
              temp_cube_struct cube{x, y, locZ, {}};
              if (tempGrid.count(cube) == 0) {
                cube.temps = temps.getCube(x, y, getGlobalLayer(locZ));
                tempGrid.emplace(cube);
              }
            }
          }
        }
      }
    }
  }
  return tempGrid;
}

void FineSolidMap::setSolidifiedNodes(
    const SparseColumnMap& temps, const FineSolidMap::TempCubesSet& tempGrid)
{
  // perform interpolation onto fine grid -- doing this in parallel is
  // problematic as some cubes overlap, and writing to a bit vector in parallel
  // can very problematic
  for (const temp_cube_struct& cube : tempGrid) {

    TrilinearCube interpCube(std::array<real_type, 8>{cube.temps[0],
                                                      cube.temps[1],
                                                      cube.temps[2],
                                                      cube.temps[3],
                                                      cube.temps[4],
                                                      cube.temps[5],
                                                      cube.temps[6],
                                                      cube.temps[7]});

    const auto [xFineIds, yFineIds, zFineIds] = genXYZFineIndices();

    for (size_t z = 0; z < zFineIds.size(); ++z) {
      for (size_t y = 0; y < yFineIds.size(); ++y) {
        for (size_t x = 0; x < xFineIds.size(); ++x) {
          const auto& wts = getLocalWeights(x, y, z);
          const real_type temp = interpCube.weightedValue(wts);

          assert(coincidentTempsEqual(x, y, z, temp, cube));

          solidifyIfMelted(temp, xFineIds[x], yFineIds[y], zFineIds[z]);
        }
      }
    }
  }
}

FineSolidMap::Weights getLocalWeights(const size_t x,
                                      const size_t y,
                                      const size_t z)
{
  return m_weights[calculateWeightIndex(x, y, z)];
}

void solidifyIfMelted(const real_type temp,
                      const size_t xFine,
                      const size_t yFine,
                      const size_t zFine)
{
  if (temp > m_solidusTemperature) {
    const size_t fineIdx = getFineIndex(xFine, yFine, zFine);
    ASSERT_LESS(fineIdx, m_solid.size());
    m_solid[fineIdx] = SOLID_VALUE;
  }
}

XYZIndices FineSolidMap::genXYZFineIndices(const temp_cube_struct& cube)
{
  const std::vector<size_t> xIndices =
      getFineGlobalIndices(getXRefinedPtsPerCell(), m_xyRefinement, cube.x);
  const std::vector<size_t> yIndices =
      getFineGlobalIndices(getYRefinedPtsPerCell(), m_xyRefinement, cube.y);
  const std::vector<size_t> zIndices =
      getFineGlobalIndices(getZRefinedPtsPerCell(), m_zRefinement, cube.z);
  return XYZIndices{
      std::move(xIndices), std::move(yIndices), std::move(zIndices)};
}
