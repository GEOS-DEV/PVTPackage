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

#include "DeadOilFlash.hpp"

#include "MultiphaseSystem/PhaseModel/BlackOil/BlackOil_WaterModel.hpp"
#include "MultiphaseSystem/PhaseModel/BlackOil/DeadOil_PhaseModel.hpp"

namespace PVTPackage
{

bool DeadOilFlash::computeEquilibrium( DeadOilFlashMultiphaseSystemProperties & sysProps )
{
  const auto & pressure = sysProps.getPressure();

  const DeadOil_PhaseModel & oilPhaseModel = sysProps.getOilPhaseModel();
  const DeadOil_PhaseModel & gasPhaseModel = sysProps.getGasPhaseModel();
  const BlackOil_WaterModel & waterPhaseModel = sysProps.getWaterPhaseModel();

  // OIL
  auto const oilProps = oilPhaseModel.computeProperties( pressure );
  sysProps.setOilModelProperties( oilProps );

  // GAS
  auto const gasProps = gasPhaseModel.computeProperties( pressure );
  sysProps.setGasModelProperties( gasProps );

  // WATER
  auto const waterProps = waterPhaseModel.computeProperties( pressure );
  sysProps.setWaterModelProperties( waterProps );

  return true;
}

}


