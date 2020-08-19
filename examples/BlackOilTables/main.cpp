/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2019 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2019 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2019 Total, S.A
 * Copyright (c) 2019-     GEOSX Contributors
 * All right reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#include <chrono>
#include <vector>
#include "MultiphaseSystem/BlackOilMultiphaseSystem.hpp"
//#include <cstdlib>
//#include <vld.h> //Visual leak detector

int main(int argc, const char * argv[])
{

  using namespace PVTPackage;

  /// ------------------------------ BO RAW DATASET

  //Surface Density [Kg/m3]
  double SurfaceOilDensity = 800.;
  double SurfaceWaterDensity = 1022.;
  double SurfaceGasDensity = 0.9907;

  //Molecular Weight [Kg/mol]
  double OilMw = 114e-3;
  double WaterMw = 18e-3;
  double GasMw = 16e-3;

  /// ------------------------------  END BO RAW DATASET
  auto PVTSystem = BlackOilMultiphaseSystem(
    { PHASE_TYPE::OIL, PHASE_TYPE::GAS, PHASE_TYPE::LIQUID_WATER_RICH },
    { "pvto.txt", "pvtg.txt", "pvtw.txt" }
    { SurfaceOilDensity, SurfaceGasDensity, SurfaceWaterDensity },
    { OilMw, GasMw, WaterMw })

  //Domain
  const size_t NBlocks = static_cast<size_t>(1e1);
  std::vector<double> Pressure(NBlocks), Temperature(NBlocks);
  std::vector<std::vector<double>> Feed(NBlocks, {0.7,0.2,0.1});
  srand(0);
  for (size_t nb = 0; nb != NBlocks; ++nb)
  {
    Pressure[nb] = 14000000; //rand() % 200000 + 100000;
    Temperature[nb] = 300;//rand() % 400 + 300;
  }

  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();


  for (size_t nb = 0; nb != NBlocks; ++nb)
  {
    PVTSystem.Update(Pressure[nb], Temperature[nb], Feed[nb]);
  }

  end = std::chrono::system_clock::now();

  auto PhaseRepartitionProperties = PVTSystem.get_MultiphaseSystemProperties();
  auto OilProperties = PVTSystem.get_PhaseProperties(PHASE_TYPE::OIL);

  double elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

  std::cout << "elapsed time: " << elapsed_seconds / 1000 << "s\n";

  return 0;
}


