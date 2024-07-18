/*
 * ------------------------------------------------------------------------------------------------------------
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (c) 2016-2024 Lawrence Livermore National Security LLC
 * Copyright (c) 2018-2024 Total, S.A
 * Copyright (c) 2018-2024 The Board of Trustees of the Leland Stanford Junior University
 * Copyright (c) 2018-2024 Chevron 
 * Copyright (c) 2019-     GEOS/GEOSX Contributors
 * All rights reserved
 *
 * See top level LICENSE, COPYRIGHT, CONTRIBUTORS, NOTICE, and ACKNOWLEDGEMENTS files for details.
 * ------------------------------------------------------------------------------------------------------------
 */

#include "MultiphaseSystem/ComponentProperties.hpp"
#include "MultiphaseSystem/BlackOilMultiphaseSystem.hpp"

#include "pvt/pvt.hpp"

#include <chrono>
#include <vector>

//#include <cstdlib>
//#include <vld.h> //Visual leak detector

int main(int argc, const char * argv[])
{

  using namespace PVTPackage;



  /// ------------------------------ BO RAW DATASET

  //PVTO
  std::vector<std::vector<double>> PVTO =
    {
      //Rs[sm3/sm3]	Pbub[Pa]	Bo[m3/sm3]	Visc(Pa.s)
      {  2,	2000000,	1.02,	0.000975	},
      {  5,	5000000,	1.03,	0.00091		},
      { 10,	10000000,	1.04,	0.00083		},
      { 15,	20000000,	1.05,	0.000695	},
      {		90000000,	1.03,	0.000985	},
      { 30,	30000000,	1.07,	0.000594	},
      { 40,	40000000,	1.08,	0.00051		},
      {		50000000,	1.07,	0.000549	},
      {		90000000,	1.06,	0.00074		},
      { 50,	50000000.7,	1.09,	0.000449	},
      {		90000000.7,	1.08,	0.000605	}
    };

  //PVTW
  std::vector<double> PVTW =
    {
      //	Pref[bar]	Bw[m3/sm3]	Cp[1/bar]	Visc[cP]
      30600000.1,		1.03,		0.00000000041,	0.0003
    };

  //PVTG
  std::vector<std::vector<double>> PVTG =
    {
      //	Pg(Pa)			Rv(sm3/sm3)	Bg(m3/sm3)	Visc(Pa.s)

      {	3000000,		0.000132,	0.04234,	0.00001344	},
      {					0,			0.04231,	0.00001389	},
      {	6000000,		0.000124,	0.02046,	0.0000142	},
      {					0,			0.02043,	0.0000145	},
      {	9000000,		0.000126,	0.01328,	0.00001526	},
      {					0,			0.01325,	0.00001532	},
      {  12000000,		0.000135,	0.00977,	0.0000166	},
      {					0,			0.00973,	0.00001634	},
      {  15000000,		0.000149,	0.00773,	0.00001818	},
      {					0,			0.00769,	0.00001752	},
      {  18000000,		0.000163,	0.006426,	0.00001994	},
      {					0,			0.006405,	0.00001883	},
      {  21000000,		0.000191,	0.005541,	0.00002181	},
      {					0,			0.005553,	0.00002021	},
      {  24000000,		0.000225,	0.004919,	0.0000237	},
      {					0,			0.004952,	0.00002163	},
      {  27000000,		0.000272,	0.004471,	0.00002559	},
      {					0,			0.004511,	0.00002305	},
      {  29500000,		0.000354,	0.004194,	0.00002714	},
      {					0,			0.004225,	0.00002423	},
      {  31000000,		0.000403,	0.004031,	0.00002806	},
      {					0.000354,	0.004059,	0.00002768	},
      {  33000000,		0.000354,	0.00391,	0.00002832	},
      {					0,			0.003913,	0.00002583	},
      {  53000000,		0.000479,	0.003868,	0.00002935	},
      {					0.000354,	0.0039,		0.00002842	},
      {					0,			0.003903,	0.00002593	}
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
  auto PVTSystem = BlackOilMultiphaseSystem(
    { pvt::PHASE_TYPE::OIL, pvt::PHASE_TYPE::GAS, pvt::PHASE_TYPE::LIQUID_WATER_RICH },
    PVTO, PVTG, PVTW,
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

  auto PhaseRepartitionProperties = PVTSystem.getMultiphaseSystemProperties();
  auto OilProperties = PVTSystem.getPhaseProperties( pvt::PHASE_TYPE::OIL );

  double elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>
    (end - start).count();

  std::cout << "elapsed time: " << elapsed_seconds / 1000 << "s\n";

  return 0;
}


