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

#include <algorithm>

namespace PVTPackage
{

DeadOilFlash::DeadOilFlash( std::vector< std::vector< double > > const & PVDO,
                            double oilSurfaceMassDensity,
                            double oilSurfaceMolecularWeight,
                            std::vector< std::vector< double > > const & PVDG,
                            double gasSurfaceMassDensity,
                            double gasSurfaceMolecularWeight,
                            std::vector< double > const & PVTW,
                            double waterSurfaceMassDensity,
                            double waterSurfaceMolecularWeight )
  :
  m_oilPhaseModel( pvt::PHASE_TYPE::OIL, PVDO, oilSurfaceMassDensity, oilSurfaceMolecularWeight ),
  m_gasPhaseModel( pvt::PHASE_TYPE::GAS, PVDG, gasSurfaceMassDensity, gasSurfaceMolecularWeight ),
  m_waterPhaseModel( PVTW, waterSurfaceMassDensity, waterSurfaceMolecularWeight )
{
}

DeadOilFlash::DeadOilFlash( std::vector< std::vector< double > > const & PVDO,
                            double oilSurfaceMassDensity,
                            double oilSurfaceMolecularWeight,
                            std::vector< std::vector< double > > const & PVDG,
                            double gasSurfaceMassDensity,
                            double gasSurfaceMolecularWeight )
  :
  m_oilPhaseModel( pvt::PHASE_TYPE::OIL, PVDO, oilSurfaceMassDensity, oilSurfaceMolecularWeight ),
  m_gasPhaseModel( pvt::PHASE_TYPE::GAS, PVDG, gasSurfaceMassDensity, gasSurfaceMolecularWeight ),
  m_waterPhaseModel()  
{
}

DeadOilFlash::DeadOilFlash( std::vector< std::vector< double > > const & PVDO,
                            double oilSurfaceMassDensity,
                            double oilSurfaceMolecularWeight,
                            std::vector< double > const & PVTW,
                            double waterSurfaceMassDensity,
                            double waterSurfaceMolecularWeight )
  :
  m_oilPhaseModel( pvt::PHASE_TYPE::OIL, PVDO, oilSurfaceMassDensity, oilSurfaceMolecularWeight ),
  m_gasPhaseModel( pvt::PHASE_TYPE::GAS ),  
  m_waterPhaseModel( PVTW, waterSurfaceMassDensity, waterSurfaceMolecularWeight )
{
}
  
DeadOil_PhaseModel const & DeadOilFlash::getOilPhaseModel() const
{
  return m_oilPhaseModel;
}

DeadOil_PhaseModel const & DeadOilFlash::getGasPhaseModel() const
{
  return m_gasPhaseModel;
}

BlackOil_WaterModel const & DeadOilFlash::getWaterPhaseModel() const
{
  return m_waterPhaseModel;
}

bool DeadOilFlash::computeEquilibrium( DeadOilFlashMultiphaseSystemProperties & sysProps ) const
{
  const auto & pressure = sysProps.getPressure();

  bool const containsGas = std::find( sysProps.getPhases().cbegin(),
                                      sysProps.getPhases().cend(),
                                      pvt::PHASE_TYPE::GAS ) != sysProps.getPhases().cend();
  bool const containsWater = std::find( sysProps.getPhases().cbegin(),
                                        sysProps.getPhases().cend(),
                                        pvt::PHASE_TYPE::LIQUID_WATER_RICH ) != sysProps.getPhases().cend();

  if( containsGas == containsWater )
  {

  }
  
  // OIL
  const DeadOil_PhaseModel & oilPhaseModel = getOilPhaseModel();  
  auto const oilProps = oilPhaseModel.computeProperties( pressure );
  sysProps.setOilModelProperties( oilProps );

  // GAS
  if( containsGas )
  {
    const DeadOil_PhaseModel & gasPhaseModel = getGasPhaseModel();
    auto const gasProps = gasPhaseModel.computeProperties( pressure );
    sysProps.setGasModelProperties( gasProps );
  }

  // WATER
  if( containsWater )
  {
    const BlackOil_WaterModel & waterPhaseModel = getWaterPhaseModel();
    auto const waterProps = waterPhaseModel.computeProperties( pressure );
    sysProps.setWaterModelProperties( waterProps );
  }

  return true;
}

}


