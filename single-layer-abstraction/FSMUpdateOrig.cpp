/******************************************************************************
 * PUBLIC METHODS *************************************************************
 *****************************************************************************/

void FineSolidMap::update(const SparseColumnMap& temps,
                          const IMeltPoolPoints& volume)
{
  m_totalTimer.start();
  m_updateTimer.start();

  std::unordered_set<temp_cube_struct> tempGrid;
  // go through each column of temperatures, and insert all adjacent cubes
  // into tempGrid for any points in the meltpool and above solidus
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

  // perform interpolation onto fine grid -- doing this in parallel is
  // problematic as some cubes overlap, and writing to a bit vector in parallel
  // can very problematic
  const size_t xPtsPerCell = getXRefinedPtsPerCell();
  const size_t yPtsPerCell = getYRefinedPtsPerCell();
  const size_t zPtsPerCell = getZRefinedPtsPerCell();
  for (const temp_cube_struct& cube : tempGrid) {

    TrilinearCube interpCube(std::array<real_type, 8>{cube.temps[0],
                                                      cube.temps[1],
                                                      cube.temps[2],
                                                      cube.temps[3],
                                                      cube.temps[4],
                                                      cube.temps[5],
                                                      cube.temps[6],
                                                      cube.temps[7]});

    for (size_t z = 0; z < zPtsPerCell; ++z) {
      const size_t fineZ = cube.z * m_zRefinement + z;
      for (size_t y = 0; y < yPtsPerCell; ++y) {
        const size_t fineY = cube.y * m_xyRefinement + y;
        for (size_t x = 0; x < xPtsPerCell; ++x) {
          const size_t fineX = cube.x * m_xyRefinement + x;
          const auto& wts = m_weights[calculateWeightIndex(x, y, z)];
          const real_type temp = interpCube.weightedValue(wts);

          // ensure corner come out to exact values
          assert(!(x == 0 && y == 0 && z == 0) || temp == cube.temps[0]);
          assert(!(x == m_xyRefinement && y == 0 && z == 0) ||
                 temp == cube.temps[1]);
          assert(!(x == 0 && y == m_xyRefinement && z == 0) ||
                 temp == cube.temps[2]);
          assert(!(x == m_xyRefinement && y == m_xyRefinement && z == 0) ||
                 temp == cube.temps[3]);
          assert(!(x == 0 && y == 0 && z == m_zRefinement) ||
                 temp == cube.temps[4]);
          assert(!(x == m_xyRefinement && y == 0 && z == m_zRefinement) ||
                 temp == cube.temps[5]);
          assert(!(x == 0 && y == m_xyRefinement && z == m_zRefinement) ||
                 temp == cube.temps[6]);
          assert(!(x == m_xyRefinement && y == m_xyRefinement &&
                   z == m_zRefinement) ||
                 temp == cube.temps[7]);

          if (temp > m_solidusTemperature) {
            const size_t fineIdx = getFineIndex(fineX, fineY, fineZ);
            ASSERT_LESS(fineIdx, m_solid.size());
            m_solid[fineIdx] = SOLID_VALUE;
          }
        }
      }
    }
  }

  m_updateTimer.stop();
  m_totalTimer.stop();
}

