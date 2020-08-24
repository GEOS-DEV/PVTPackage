/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2018-2020 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2020 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2020 Total, S.A
 * Copyright (c) 2020-     GEOSX Contributors
 * All right reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#include "MultiphaseSystem/ComponentProperties.hpp"
#include <chrono>
#include <vector>
#include "MultiphaseSystem/DeadOilMultiphaseSystem.hpp"
//#include <cstdlib>
//#include <vld.h> //Visual leak detector

int main(int argc, const char * argv[])
{

  using namespace PVTPackage;



  /// ------------------------------ BO RAW DATASET

  //PVDO
  std::vector<std::vector<double>> PVDO =
    {
      //P[Pa]	Bo[m3/sm3]	Visc(Pa.s)
      { 2000000,	1.02,	0.000975	},
      { 5000000,	1.03,	0.00091		},
      { 10000000,	1.04,	0.00083		},
      { 20000000,	1.05,	0.000695	},
      { 30000000,	1.07,	0.000594	},
      { 40000000,	1.08,	0.00051		},
      { 50000000.7,	1.09,	0.000449	}
    };

  //PVTW
  std::vector<double> PVTW =
    {
      //	Pref[bar]	Bw[m3/sm3]	Cp[1/bar]	Visc[cP]
      30600000.1,		1.03,		0.00000000041,	0.0003
    };

  //PVDG
  std::vector<std::vector<double>> PVDG =
    {
      //	Pg(Pa)		Bg(m3/sm3)	Visc(Pa.s)
      {	 3000000,		0.04234,	0.00001344	},
      {	 6000000,		0.02046,	0.0000142	},
      {	 9000000,		0.01328,	0.00001526	},
      {  12000000,		0.00977,	0.0000166	},
      {  15000000,		0.00773,	0.00001818	},
      {  18000000,		0.006426,	0.00001994	},
      {  21000000,		0.005541,	0.00002181	},
      {  24000000,		0.004919,	0.0000237	},
      {  27000000,		0.004471,	0.00002559	},
      {  29500000,		0.004194,	0.00002714	},
      {  31000000,		0.004031,	0.00002806	},
      {  33000000,		0.00391,	0.00002832	},
      {  53000000,		0.003868,	0.00002935	}
    };

  //Surface Density [Kg/m3]
  double SurfaceOilDensity = 800.;
  double SurfaceWaterDensity = 1022.;
  double SurfaceGasDensity = 0.9907;

  //Molecular Weight [Kg/mol]
  double OilMw = 114e-3;
  double WaterMw = 18e-3;
  double GasMw = 16e-3;

  /// ------------------------------  END BO RAW DATASET
  auto PVTSystem = DeadOilMultiphaseSystem(
    { PHASE_TYPE::OIL, PHASE_TYPE::GAS, PHASE_TYPE::LIQUID_WATER_RICH },
    PVDO, PVDG, PVTW,
    { SurfaceOilDensity, SurfaceGasDensity, SurfaceWaterDensity },
    { OilMw, GasMw, WaterMw });

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

  double elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
    (end - start).count();

  std::cout << "elapsed time: " << elapsed_seconds / 1000 << "s\n";

  return 0;
}


