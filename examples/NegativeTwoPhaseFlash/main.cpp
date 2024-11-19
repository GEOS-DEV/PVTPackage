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
#include "MultiphaseSystem/CompositionalMultiphaseSystem.hpp"

#include "pvt/pvt.hpp"

#include <chrono>
#include <vector>

int main()
{

  using namespace PVTPackage;

  //Component properties
  std::vector<std::string> Labels = { "N2","C10","C20","H2O" };
  auto Pc = { 34e5,25.3e5,14.6e5,220.5e5 };
  auto Tc = { 126.2,622.0,782.0,647.0 };
  auto Omega = { 0.04,0.443,0.816,0.344 };
  auto Mw = { 28e-3,134e-3,275e-3,18e-3 };
  auto nbc = Pc.size();
  const ComponentProperties CompProps(nbc, Labels, Mw, Tc, Pc, Omega);

  auto PVTSystem = CompositionalMultiphaseSystem(
    { pvt::PHASE_TYPE::OIL,PHASE_TYPE::GAS },
    { EOS_TYPE::PENG_ROBINSON,EOS_TYPE::PENG_ROBINSON },
    COMPOSITIONAL_FLASH_TYPE::NEGATIVE_OIL_GAS,
    CompProps);

  //Domain
  const size_t NBlocks = static_cast<size_t>(1e1);
  std::vector<double> Pressure(NBlocks), Temperature(NBlocks);
  std::vector<std::vector<double>> Feed(NBlocks,std::vector<double>(nbc,0.25));
  srand(0);
  for (size_t nb = 0; nb != NBlocks; ++nb)
  {
    Pressure[nb] = 1e5; //rand() % 200000 + 100000;
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

  double elapsed_seconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

  std::cout << "elapsed time: " << elapsed_seconds / 1000 << "s\n";

  return 0;
}


